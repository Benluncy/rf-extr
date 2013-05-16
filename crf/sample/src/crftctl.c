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
		int hstkcheck = 0;
		for(int i=(*currentOffset);i<crfNodeSnapshot.offset+(*currentOffset);i++)
		{
			if(!isDelimiter(content[i]) && !hstkcheck)
			{
				tkcheck = i;
				//break;
				hstkcheck = 1;
			}
			switch(content[i])
			{
				case '\"':
					crfNodeSnapshot.quotflag = 1;
					break;
				case '(':
					crfNodeSnapshot.pareSflag = 1;
					break;
				case ')':
					crfNodeSnapshot.pareEflag = 1;
					break;
				case '[':
					crfNodeSnapshot.sqbSflag = 1;
					break;
				case ']':
					crfNodeSnapshot.sqbEflag = 1;
					break;
				case '{':
					crfNodeSnapshot.braSflag = 1;
					break;
				case '}':
					crfNodeSnapshot.braEflag = 1;
					break;
				case '?':
				case ',':
				case '.':
					crfNodeSnapshot.stopflag = 1;
					break;
			}
		}

		crfNodeSnapshot.token = filteredTokenId(tkcheck);//offsum+(offset+1)/2
		
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
	//int refAreaEnd;
	int currentOffset;
	//char *content;
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
	//refAreaEnd = getReferenceEndOffset();
	currentOffset = refAreaStart;

	//make queue full
	while(ftEnQueue(&nextCNSQ,&currentOffset,&mpredeli));
	
	int quotStatus = 0;
	int quotEffect = 0;
	
	while((pCNS = ftDeQueue(&nextCNSQ)) != NULL)
	{
		pCrfNodeSnapshot lpCNS = pastNElem(&preCNSQ,1);  // last one CNS
		pCrfNodeSnapshot npCNS = nextNElem(&nextCNSQ,1); // next one CNS

		/*		
		printf("size: %d ~ %d \n",sizeQueue(&preCNSQ),sizeQueue(&nextCNSQ));
		for(int i=0;i<=CNSQSIZE;i++)
		{
			printf("past/pre  %d %d \t",i,pastNElem(&preCNSQ,i)!=NULL);
			printf("next/next %d %d \t",i,nextNElem(&nextCNSQ,i)!=NULL);
			printf("past/next %d %d \t",i,pastNElem(&nextCNSQ,i)!=NULL);
			printf("next/pre  %d %d \n",i,nextNElem(&preCNSQ,i)!=NULL);
		}
		*/
		
		//features write
		// 0: string it self
		fprintf(fp,"%s\t",pCNS->str);
		// 1: last delimiter
		fprintf(fp,"%d\t",pCNS->predeli);
		// 2: last useful delimiter
		fprintf(fp,"%d\t",pCNS->mpredeli);
		// 3: next delimiter
		fprintf(fp,"%d\t",pCNS->nextdeli);
		// 4: len of str
		fprintf(fp,"%d\t",pCNS->slen);
		// 5: digit length
		fprintf(fp,"%d\t",pCNS->digitl);
		// 6: is pure digit ?
		fprintf(fp,"%d\t",pCNS->puredigit);
		// 7: value of digit
		fprintf(fp,"%d\t",pCNS->dval);
		// 8: which type of string ?
		fprintf(fp,"%d\t",pCNS->strtype); // TODO may spilit into several tokens
		// 9: year like?
		fprintf(fp,"%d\t",pCNS->yearlike);
		// 10: month like?
		fprintf(fp,"%d\t",pCNS->monthlike);
		// 11: volumn like? 
		fprintf(fp,"%d\t",pCNS->volumnlike);
		// 12: page like? 
		fprintf(fp,"%d\t",pCNS->pagelike);
		// 13: eds flag ? 
		fprintf(fp,"%d\t",pCNS->edsflag);
		// 14: value of digit (value - last value > 0)
		fprintf(fp,"%d\t",(lpCNS == NULL)?-1:(pCNS->dval > lpCNS->dval));
		// 15: some special flag 
		fprintf(fp,"%d\t",pCNS->speflag);
		
		// 16: In 
		fprintf(fp,"%d\t",pCNS->speflag == 13);
		
		// 17: proc
		fprintf(fp,"%d\t",pCNS->speflag == 5);
		
		// 18: in quot ? in/out/none
		quotStatus = 
		
		
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
		
		
		
		
		
		// END : token
		if(lpCNS != NULL && npCNS != NULL)
		{
			if((lpCNS->token == 3 && npCNS->token == 3) || 
				(lpCNS->token == 6 && npCNS->token == 6))
				fprintf(fp,"%s\n",id2Token(lpCNS->token));
			else
				fprintf(fp,"%s\n",pCNS->token == 0 ? "OTH":id2Token(pCNS->token));	
		}else
			fprintf(fp,"%s\n",pCNS->token == 0 ? "OTH":id2Token(pCNS->token));
		
		//enQueue && store
		enQueueWithDrop(&preCNSQ,*pCNS);
		ftEnQueue(&nextCNSQ,&currentOffset,&mpredeli);
	}
	
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


