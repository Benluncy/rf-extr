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


int ftEnQueue(pCNSQ Q,int *currentOffset,char *mpredeli)
{
	if(isFullQueue(Q)) return 0;

	int refAreaEnd = getReferenceEndOffset();
	char *content = getPcontent();
	char str[SINGLEWORDLEN];
	
	CrfNodeSnapshot crfNodeSnapshot;
	
	//spilitContent(char *dest,int dlen,const char *src,int len)
	if((crfNodeSnapshot.offset = spilitContent(str,SINGLEWORDLEN,
			content+(*currentOffset),
			refAreaEnd-(*currentOffset),
			&(crfNodeSnapshot.predeli),
			&(crfNodeSnapshot.nextdeli))) != 0)
	{
		int slen = strlen(str);
		sprintf(crfNodeSnapshot.str,"%s",str);
		crfNodeSnapshot.slen = slen;
		//int dval;
		int tkcheck;
		for(int i=(*currentOffset);i<crfNodeSnapshot.offset+(*currentOffset);i++)
		{
			if(!isDelimiter(content[i]))
			{
				tkcheck = i;
				break;
			}
		}

		crfNodeSnapshot.token = filteredTokenId(tkcheck);//offsum+(offset+1)/2
		
		printf("offset:%d",crfNodeSnapshot.offset);
		*currentOffset += crfNodeSnapshot.offset;

		sprintf(crfNodeSnapshot.str,"%s",str);
		
		crfNodeSnapshot.mpredeli = isBlank(crfNodeSnapshot.predeli)?(*mpredeli):' ';
		crfNodeSnapshot.digitl = digitlen(str,slen);
		crfNodeSnapshot.puredigit = puredigit(str,slen);
		crfNodeSnapshot.dval = valofdigit(str,slen);
		crfNodeSnapshot.strtype = strfeature(str,slen);
		crfNodeSnapshot.yearlike = yearlike(str,slen);
		crfNodeSnapshot.monthlike = monthlike(str,slen);
		crfNodeSnapshot.volumnlike = vollkwd(str,slen);
		crfNodeSnapshot.pagelike = pagekwd(str,slen);
		crfNodeSnapshot.edsflag = edsFlag(str,slen);
		crfNodeSnapshot.speflag = specialFlag(str,slen);
		crfNodeSnapshot.procflg =  procFlag(str,slen);
		crfNodeSnapshot.nameLike = hasNameafterTheOffset0((*currentOffset)
							-crfNodeSnapshot.offset-1,
							crfNodeSnapshot.offset+1);
		crfNodeSnapshot.isNameDict = isNameInDict(str);
		crfNodeSnapshot.rLastNameDict = rateLastNameInDict(str);
		crfNodeSnapshot.isCountryDict = isCountryInDict(str);
		crfNodeSnapshot.isFunWordDict = isFunWordInDict(str);
		crfNodeSnapshot.isPlaceNameDict = isPlaceNameInDict(str);
		crfNodeSnapshot.isPubliserDict = isPublisherInDict(str);
		crfNodeSnapshot.isArticle = isArticle(str,slen);
		crfNodeSnapshot.deptflag = deptFlag(str);
		crfNodeSnapshot.uniflag = uniFlag(str);
		crfNodeSnapshot.ltdflag = ltdFlag(str);

		if(!isBlank(crfNodeSnapshot.nextdeli)) *mpredeli = crfNodeSnapshot.nextdeli;
		
		//*currentOffset = crfNodeSnapshot.offset;
		enQueue(Q,crfNodeSnapshot);
		return 1;
	}else
		return 0;
}


pCrfNodeSnapshot ftDeQueue(pCNSQ Q)
{
	if(isEmptyQueue(Q)) return NULL;
	return deQueue(Q);
}

int genCRFSampleCtl(const char* fileName,int isDir)
{
	static int id = 1;
	FILE *fp; // local train
	int trainOrTest;
	int refAreaStart;
	int refAreaEnd;
	int currentOffset;
	char *content;
	char mpredeli=' ';
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
	clearQueue(&preCNSQ);
	clearQueue(&nextCNSQ);
	pCrfNodeSnapshot pCNS;
	
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
	
	//
	refAreaStart = getReferenceHeadOffset();
	refAreaEnd = getReferenceEndOffset();
	currentOffset = refAreaStart;

	printf("\npre : enqueue \n"); fflush(NULL);	
	//make queue full
	while(ftEnQueue(&nextCNSQ,&currentOffset,&mpredeli));

	printf("end : enqueue\n"); fflush(NULL);

	printf("pre : real parse\n"); fflush(NULL);
	
	while((pCNS = ftDeQueue(&nextCNSQ)) != NULL)
	{
		printf("in : real parse %d\n",currentOffset); fflush(NULL);
	
		//features write
		// 0: string it self
		// 1: last delimiter
		// 2: last useful delimiter
		// 3: next delimiter		
		// 4: len of str
		// 5: digit length
		// 6: is pure digit ?
		// 7: value of digit
		// 8: which type of string ?
		// 9: year like?
		// 10: month like?
		// 11: volumn like? 
		// 12: page like? 
		// 13: eds flag ? 
		// 14: value of digit (value - last value)
		// 15: some special flag 
		
		// 16: In 
		// 17: proc
		// 18: in quot ? in/out/none
		// 19: name like?
		
		// 20: isNameInDict
		// 21: rLastNameDict > 0 ||isNameDict
		// 22: rateLastNameInDict
		
		
		// 23: isCountryInDict
		
		// 24: isFunWordInDict
		
		// 25: isPlaceNameInDict
		
		// 26: isPublisherInDict
		
		// 27: isMonthInDict
		
		// 28 : a,an,the,
		
		// 29 : dept. xx department of ...
		
		// 30 : university
		
		// 31 : Inc.
		
		// 32 : http
		
		//enQueue && store
		enQueueWithDrop(&preCNSQ,*pCNS);
		ftEnQueue(&nextCNSQ,&currentOffset,&mpredeli);
	}
	
	printf("end : real parse\n"); fflush(NULL);
	//
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


