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
#include <ctype.h>

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
	
	int isPublisher = 0;
	
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
		char partStr[1024];
		int psI = 0;
		for(int i=(*currentOffset);i<crfNodeSnapshot.offset+(*currentOffset);i++)
		{
			if(!isDelimiter(content[i]) && !hstkcheck)
			{
				tkcheck = i;
				//break;
				hstkcheck = 1;
			}
			if(DIGITLIKE(content[i]))
			{
				partStr[psI] = content[i];
				psI++;
				
			}else if(psI>0)
			{
				partStr[psI]='\0';
				isPublisher = isPublisher || isPublisherInDict(partStr); 
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
				case '.':
				case '!':
					crfNodeSnapshot.stopflag = 1;
					break;
				case ',':
					crfNodeSnapshot.stopflag = 2;
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
		crfNodeSnapshot.namelike = hasNameafterTheOffset0((*currentOffset)
							-crfNodeSnapshot.offset-1,
							crfNodeSnapshot.offset+1);
		crfNodeSnapshot.isNameDict = isNameInDict(str);
		crfNodeSnapshot.rLastNameDict = rateLastNameInDict(str);
		crfNodeSnapshot.isCountryDict = isCountryInDict(str);
		crfNodeSnapshot.isFunWordDict = isFunWordInDict(str);
		crfNodeSnapshot.isPlaceNameDict = isPlaceNameInDict(str);
		crfNodeSnapshot.isPubliserDict = isPublisherInDict(str) ||isPublisher;
		crfNodeSnapshot.isArticle = isArticle(str,slen);
		crfNodeSnapshot.deptflag = deptFlag(str);
		crfNodeSnapshot.uniflag = uniFlag(str);
		crfNodeSnapshot.ltdflag = ltdFlag(str);
		crfNodeSnapshot.domainflag = domainFlag(str);
		

		if(crfNodeSnapshot.puredigit)
		{
			int vh = valofdigit(str,slen/2);
			int vl = valofdigit(str+(slen/2),(slen+1)/2);
			if(vh == 0 || vl == 0) 	crfNodeSnapshot.imprnum = 0;
			else crfNodeSnapshot.imprnum = vl > vh ? 1 : -1;
		}else
			crfNodeSnapshot.imprnum = 0;
		
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
	
	
	int httpStatus = 0;
	//int httpTime = 0;
	
	int quotStatus = 0;
	int quotTime = 0;
	
	int pareStatus = 0;
	int paraTime = 0;
	
	int sqbStatus = 0;
	int sqbTime = 0;
	
	int braStatus = 0;
	int braTime = 0;
	
	int isbnEffect = 0;

	while((pCNS = ftDeQueue(&nextCNSQ)) != NULL)
	{
		// 0. PREPARE : FLAGS
		
		// 0.0 PAST ONE INFO && NEXT ONE INFO
		pCrfNodeSnapshot lpCNS = pastNElem(&preCNSQ,1);  // previous one node
		pCrfNodeSnapshot npCNS = nextNElem(&nextCNSQ,1); // next one node
		
		// 0.1 TRAVERSAL ALL PAST && NEXT INFO , GET FLAGS
		
		
		
		
		// 0.2 SOME MIX FEATURE
		// \"
		quotTime--;
		if(quotTime <= 0)
		{
			quotStatus = 0;
			quotTime = 0;
		}
		if(pCNS->quotflag == 1)
		{
			quotStatus = !quotStatus;
			if(quotStatus)
				quotTime = 10;
			else
				quotTime = -10;
		}
		//((quotTime==10||quotTime==-10)?(quotStatus?"IN":"OUT"):(quotStatus?"AT":"NA"))
		
		
		
		
		
		// combined string , combine with next string
		char combinedStr[1024];
		sprintf(combinedStr,"%s%s",pCNS->str,((npCNS==NULL)?"":npCNS->str));
		
		/*
		// 1. OUTPUT : PRINT FEATURES
		
		// basic
		fprintf(fp,"%s\t",pCNS->str); // 0: string data
		fprintf(fp,"%d\t",pCNS->slen); // 1: length of string data
		
		
		// base::string
		// 2: string type 0:AAA 1:aaa 2:Aaa 3:aAa 4:123
		fprintf(fp,"%d\t",pCNS->strtype);
		
		// 3: sufix 
		fprintf(fp,"%c%c\t",(pCNS->slen>1)?tolower(pCNS->str[pCNS->slen-2]):'X',
					tolower(pCNS->str[pCNS->slen-1]));
		
		
		// base::digit
		// 4: digit value  > 0 ?
		fprintf(fp,"%d\t",pCNS->dval > 0 );
		
		// 5: digit bigger than previours one
		fprintf(fp,"%d\t",lpCNS==NULL?-1:(lpCNS->dval == 0?-1:(pCNS->dval > lpCNS->dval)));
		
		// 6: next one is bigger than this digit 
		fprintf(fp,"%d\t",npCNS==NULL?-1:(npCNS->dval == 0?-1:(npCNS->dval > lpCNS->dval)));
		
		// 7: digit a improve digit ? 123456 456 > 123
		fprintf(fp,"%d\t",pCNS->imprnum );
		
		*/
		
		// 2. END : PRINT RESULT
		if(lpCNS != NULL && npCNS != NULL)
		{
			if((lpCNS->token == 3 && npCNS->token == 3) || 
				(lpCNS->token == 6 && npCNS->token == 6))
				fprintf(fp,"%s\n",id2Token(lpCNS->token));
			else
				fprintf(fp,"%s\n",pCNS->token == 0 ? "0":id2Token(pCNS->token));	
		}else
			fprintf(fp,"%s\n",pCNS->token == 0 ? "0":id2Token(pCNS->token));
		
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


