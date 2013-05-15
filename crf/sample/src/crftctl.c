#include "crftctl.h"
#include "virtualcontent.h"
#include "dirTraversal.h"
#include "hftctl.h" // int getReferenceHeadOffset(void);
#include "eftfun.h" // int getReferenceEndOffset(void);
#include "strHandle.h" //spilitContent
#include "tokens.h"
#include "minEditDistance.h"
#include "crftfun.h"
#include "hftnpse.h"
#include "dict.h"
#include "queue.h"
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
	int finalTag = 0;
	while((nowTag=tokenPop(&myTags)) > 0)
	{
		//ignored: 
		// note 10
		// RefA 1
		// SinRef 2
		// tech 13
		if(nowTag != 1 && nowTag != 2 && nowTag != 10 ) // ingore RefA && SinRef
		{
			finalTag =  nowTag;
		}	
	}		
	return finalTag;
}


int sampleEnQueue(int *currentOffset)
{
	
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
	
	CNSQ preCNSQ;
	CNSQ nextCNSQ;
	
	
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

	int procflg = 0;	
	
	int inquot = 0;
	int quottime = 0;
	
	int httpEffect = 0;
	
	//spilitContent(char *dest,int dlen,const char *src,int len)
	while((offset = spilitContent(str,SINGLEWORDLEN,content+offsum,
			refAreaEnd-offsum,&predeli,&nextdeli)) != 0)
	{
		int slen = strlen(str);
		int dval;
		int tkcheck;
		for(int i=offsum;i<offset+offsum;i++)
		{
			if(!isDelimiter(content[i]))
			{
				tkcheck = i;
				break;
			}
		}
		nowtoken = filteredTokenId(tkcheck);//offsum+(offset+1)/2
		offsum += offset;

		if(mpredeli != '/' && nextdeli != '/' && predeli != '/' &&
			mpredeli != '.' && nextdeli != '.' && predeli != '.') httpEffect = 0;
		/////////////////////////////////////////////////////////////////////////////////

		//printf("[%s] \t[%c,%c]  %d-%d ~\"%c\"\n",str,predeli,
		//				nextdeli,offset,offsum,content[offsum]);
		fprintf(fp,"%s\t",str);
		// features are here
		
		// 1: last delimiter
		// 2: last useful delimiter
		// 3: next delimiter
		//fprintf(fp,"%c\t%c\t%c\t",isBlank(predeli)?'B':predeli, //last delimiter 1
		fprintf(fp,"%d\t%d\t%d\t",isBlank(predeli)?'B':predeli, //last delimiter 1
					isBlank(predeli)?(isBlank(mpredeli)?'B':mpredeli):'B', // 2 last useful delimiter
					isBlank(nextdeli)?'B':nextdeli); // next delimiter 3
		
		if(!isBlank(nextdeli)&&nextdeli!='-') mpredeli = nextdeli;
		if(isBlank(predeli)) predeli = mpredeli;
		//if(mpredeli=='-') mpredeli = ' '; // '-' only effect one time
		
		// str itself
		// 4: len of str
		fprintf(fp,"%d\t",slen);// 4		
		
		// digit features
		int digitl = digitlen(str,slen);
		fprintf(fp,"%d\t",digitl); // digit length  5
		fprintf(fp,"%d\t",puredigit(str,slen)); // is pure digit ? 6 
		
		dval = valofdigit(str,slen);
		fprintf(fp,"%d\t",dval); // value of digit 7

		// string features
		fprintf(fp,"%d\t",strfeature(str,slen)); // which type of string ? 8
				
		fprintf(fp,"%d\t",yearlike(str,slen)); // year like? 9
		

		
		fprintf(fp,"%d\t",monthlike(str,slen)); // month like? 10
		
		fprintf(fp,"%d\t",vollkwd(str,slen));//volumn like? 11
		
		fprintf(fp,"%d\t",pagekwd(str,slen)); // page like? 12
		
		fprintf(fp,"%d\t",edsFlag(str,slen)); // eds flag ? 13
		
				
		// 14 value of digit (value - last value) 14
		fprintf(fp,"%s\t",(dval > lastdigitvalue)?"yes":"no"); 
		
		int speflg = specialFlag(str,slen);
	
		
		// some special flag // 15
		fprintf(fp,"%d\t",speflg); 
		
		
		// 16 In 17 proc
		procflg =  procFlag(str,slen);
		fprintf(fp,"%s\t",procflg==1?"in":"none");
		fprintf(fp,"%s\t",procflg==2?"proc":"none");
		
		
		if(predeli == '\"')
		{
			inquot = inquot==1?-1:1;
			if(inquot==1) quottime = 1;
		}else
		{
			if(quottime !=  0) quottime ++;
			if(quottime >= 10)
			{
				quottime = 0;
				inquot = 0;
			}
		}
		
		 // 18
		fprintf(fp,"%s\t",quottime==0?"none":(inquot==1?"in":(inquot==-1?"out":"none")));
		
		if(inquot == -1) inquot = 0;

		
		// name like ? 
		fprintf(fp,"%s\t",hasNameafterTheOffset0(offsum-offset-1,offset+1)==0?"no":"yes"); // 19
		
		int isNameDict = isNameInDict(str);
		int rLastNameDict = rateLastNameInDict(str);
		int isCountryDict = isCountryInDict(str);
		int isFunWordDict = isFunWordInDict(str);
		int isPlaceNameDict = isPlaceNameInDict(str);
		int isPubliserDict = isPublisherInDict(str);
		int isMonthDict = isMonthInDict(str);
		//20,21 int isNameInDict(const char *str);
		fprintf(fp,"%s\t",isNameDict?"yes":"no");
		fprintf(fp,"%s\t",rLastNameDict > 0 ||isNameDict?"yes":"no");
		//22 int rateLastNameInDict(const char *str);
		fprintf(fp,"%d\t",rLastNameDict);
		//23 int isCountryInDict(const char *str);
		fprintf(fp,"%s\t",isCountryDict?"yes":"no");
		//24 int isFunWordInDict(const char *str);
		fprintf(fp,"%s\t",isFunWordDict?"yes":"no");
		//25 int isPlaceNameInDict(const char *str);
		fprintf(fp,"%s\t",isPlaceNameDict?"yes":"no");
		//26 int isPublisherInDict(const char *str);
		fprintf(fp,"%s\t",isPubliserDict?"yes":"no");
		//27 int isMonthInDict(const char *str);
		fprintf(fp,"%s\t",isMonthDict?"yes":"no");
		
		
		//28 : a,an,the,
		fprintf(fp,"%s\t",isArticle(str,slen)?"yes":"no");
		
		
		//29 : dept. xx department of ...
		fprintf(fp,"%s\t",deptFlag(str)?"yes":"no");
		
		//30 : university , ltd.
		fprintf(fp,"%s\t",uniLtdFlag(str)?"yes":"no");
		
		//31 : Inc.
		fprintf(fp,"%s\t",speflg==4?"yes":"no");
		
		if(speflg==7) httpEffect = 1;
		//32 : http
		fprintf(fp,"%s\t",httpEffect?"yes":"no");
		
		
		////////////////////////////////////////////////////////////////////////////
		
		// result :
		int tcheck;
		for(int i=offsum;i<offsum+30;i++)
		{
			if(!isDelimiter(content[i]))
			{
				tcheck = i;
				break;
			}
		}
		if(lasttoken == 3 && (filteredTokenId(tcheck) == 3)) // author and author
		{
			fprintf(fp,"%s\n",id2Token(3));
		}else if(lasttoken == 6 && (filteredTokenId(tcheck) == 6)) // editor and editor
		{
			fprintf(fp,"%s\n",id2Token(6));
		}else
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


