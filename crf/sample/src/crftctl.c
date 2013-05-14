#include "crftctl.h"
#include "virtualcontent.h"
#include "dirTraversal.h"
#include "hftctl.h" // int getReferenceHeadOffset(void);
#include "eftfun.h" // int getReferenceEndOffset(void);
#include "strHandle.h" //spilitContent
#include "tokens.h"
#include "minEditDistance.h"
#include "crftfun.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define SINGLEWORDLEN 256

FILE *fpTrain; // train.txt
FILE *fpTest; // test.txt



int filteredTokenId(int offset)
{
	int mypclen = getPclen();
	unsigned int myTags;
	unsigned int nowTag;
	if(offset >= mypclen) return 0;
	myTags = *(getTags()+offset);
	while((nowTag=tokenPop(&myTags)) > 0)
	{
		if(nowTag != 1 && nowTag != 2) // ingore RefA && SinRef
		{
			return nowTag;
		}	
	}		
	return 0;
}


int genCRFSampleCtl(const char* fileName,int isDir)
{
	static int id = 1;
	FILE *fp; // local train
	int trainOrTest;
	int refAreaStart;
	int refAreaEnd;
	char *content;
        if(isDir)
        {
                printf("ignore dir:%s\n",fileName);
                return 1;
        }
	if(rand()%2) //train is 50% and test is 50%
	{
		// train
		fp = fpTrain;
		trainOrTest = 1;
	}else
	{
		// test
		fp = fpTest;
		trainOrTest = 0;
	}
	
	
	//
        printf("[%d] %s:%s",id,(trainOrTest?"train":"test"),fileName);
        fflush(NULL);
        
        // parse tag or etc ,move data to RAM
 	initContent();
	if(!parseFile(fileName))
	{
		fprintf(stderr,"[[error parsing file : #%s#]]",fileName);
		getchar();
		return 0;
	}
	
	// start and end
	refAreaStart = getReferenceHeadOffset();
	refAreaEnd = getReferenceEndOffset();
	content = getPcontent();
	//char *content = getPcontent();
	//int offend = getPclen();
	
	char str[SINGLEWORDLEN];
	int offset=0;
	int offsum = refAreaStart;
	char predeli;
	char mpredeli = ' ';
	char nextdeli;
	int nowtoken = 0;
	int lasttoken = 0;
	int lastdigitvalue = 0;
	
	int lastSpflg = 0;
	int flagtime = 0;
	int nowSpflg = 0;
	
	int edskwdlen = 0;
	
	int inquot = 0;
	int quottime = 0;
	
	//spilitContent(char *dest,int dlen,const char *src,int len)
	while((offset = spilitContent(str,SINGLEWORDLEN,content+offsum,
			refAreaEnd-offsum,&predeli,&nextdeli)) != 0)
	{
		int slen = strlen(str);
		int dval;
		nowtoken = filteredTokenId(offsum+(offset+1)/2);
		offsum += offset;
		if(isBlank(predeli)) predeli = mpredeli;

		//printf("[%s] \t[%c,%c]  %d-%d ~\"%c\"\n",str,predeli,
		//				nextdeli,offset,offsum,content[offsum]);
		fprintf(fp,"%s\t",str);
		// features are here
		
		// 1: last delimiter
		// 2: last useful delimiter
		// 3: next delimiter
		fprintf(fp,"%c\t%c\t%c\t",isBlank(predeli)?'B':predeli, //last delimiter 1
					isBlank(predeli)?(isBlank(mpredeli)?'B':mpredeli):'B', // 2 last useful delimiter
					isBlank(nextdeli)?'B':nextdeli); // next delimiter 3
		
		
		// str itself
		// 4: len of str
		fprintf(fp,"%d\t",slen);// 4		
		
		// digit features
		fprintf(fp,"%d\t",digitlen(str,slen)); // digit length  5
		fprintf(fp,"%d\t",puredigit(str,slen)); // is pure digit? 6
		
		dval = valofdigit(str,slen);
		fprintf(fp,"%d\t",dval); // value of digit 7
		
		fprintf(fp,"%d\t",yearlike(str,slen)); // year like? 8
		
		// string features
		fprintf(fp,"%d\t",strfeature(str,slen)); // which type of string ? 9
		
		fprintf(fp,"%d\t",monthlike(str,slen)); // month like? 10
		
		fprintf(fp,"%d\t",vollkwd(str,slen));//volumn like? 11
		
		
		if(edsFlag(str,slen)) edskwdlen = 1;

		fprintf(fp,"%d\t",edskwdlen); // near eds ? 12
		
		
		// some advance 
		fprintf(fp,"%d\t",dval > lastdigitvalue && lastdigitvalue != 0); // 13 page?
		
		
		if((nowSpflg=specialFlag(str,slen))!= 0 )
		{
			lastSpflg = nowSpflg;
			flagtime = 1;
		}
		
		fprintf(fp,"%d\t",nowSpflg); // 14
		
		fprintf(fp,"%d\t%d\t",flagtime?lastSpflg:0,flagtime); // 15 16
		
		//17 quto
		if(predeli == '\"')
		{
			inquot = inquot?0:1;
			if(inquot) quottime = 1;
		}else
		{
			quottime ++;
			if(quottime >= 10)
			{
				quottime = 0;
				inquot = 0;
			}
		}
		
		fprintf(fp,"%d\t",inquot); // 17 

		
		// name like
		
		
		
		if(edskwdlen)
		{
			edskwdlen++;
			if(edskwdlen >10) edskwdlen = 0;
		}
		
		if(flagtime)
		{
			flagtime ++;
			if(flagtime>5 ) flagtime = 0;
		}
		
		
		
		fprintf(fp,"%d\t",pagekwd(str,slen)); // page like?
		
		// result :
		
		if(lasttoken == 3 && editDistanceS("and",3,str,3)==0) // author and author
			fprintf(fp,"%s\n",id2Token(3));
		else
			fprintf(fp,"%s\n",nowtoken == 0 ? "OTH":id2Token(nowtoken));
		if(!isBlank(nextdeli)) mpredeli = nextdeli;
		
		// for next time
		// real value
		lasttoken = nowtoken;
		
		// features
		lastdigitvalue = dval;
	}
	
	
	id++;
        cleanContent();
        printf(" ok\n");
        return 1;
}


int genCRFSample(const char *path)
{
	FILE *fp;
	srand((unsigned int)time(NULL));
	
	//open train.data
	if((fp = fopen("res/train.data","w")) == NULL)
	{
		fprintf(stderr,"error in opening train.data\n");
		return 0;
	}
	setCRFTrainFile(fp);
	
	//open test.data
	if((fp = fopen("res/test.data","w")) == NULL)
	{
		fprintf(stderr,"error in opening train.data\n");
		return 0;
	}
	setCRFTestFile(fp);
	
	// do traversal
	dirTraversal(path,1,genCRFSampleCtl);
	
	
	//close train.data
	fclose(getCRFTrainFile());
	setCRFTrainFile(NULL);
	//close test.data
	fclose(getCRFTestFile());
	setCRFTestFile(NULL);
	return 1;
}


void setCRFTrainFile(FILE *fp){ fpTrain = fp;}
void setCRFTestFile(FILE *fp){fpTest = fp;}
FILE * getCRFTrainFile(){return fpTrain;}
FILE * getCRFTestFile(){return fpTest;}


