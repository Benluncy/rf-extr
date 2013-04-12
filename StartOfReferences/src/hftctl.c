#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include "minEditDistance.h"
#include "virtualcontent.h"
#include "hftnameandpp.h"
#include "persistence.h"
#include "tokens.h"

#include "hftctl.h"


int fileNum = 0;
featureDataContainer myFeatureDataContainer;
featureData myFeatureData;
int id = 1;

/**
 * Start FilterData Info Handle
 */
int initFilterData()
{
	pFilterData = NULL;
	return 1;	
}

int cleanFilterData()
{
	_FilterData head = pFilterData;
	_FilterData p;
	while(head != NULL)
	{
		p = head;
		head = head->next;
		free(p);
	}
	return 1;
}


/**
 * 	int accept ;  // acc =1 accept , acc = 0 , reject 
 *	char *cmpStr;
 *	int cmpLen;
 */
int insertFilterData(int accept,const char *cmpStr,int cmpLen)
{
	_FilterData p = pFilterData;
	
	if(pFilterData == NULL)
	{
		pFilterData = (_FilterData)malloc(sizeof(FilterData));
		pFilterData->acc = accept;
		pFilterData->cmpLen = cmpLen;
		//head->cmpStr = strdup(cmpStr);
		sprintf(pFilterData->cmpStr,"%s",cmpStr);
		pFilterData->next = NULL;
		//printf("head, done \n");
		fflush(NULL);
	}else
	{
		//printf("inserting ...");
		while(p->next != NULL){ p=p->next;}
		p->next = (_FilterData)malloc(sizeof(FilterData));
		p = p->next;
		p->acc =accept;
		p->cmpLen = cmpLen;
		sprintf(p->cmpStr,"%s",cmpStr);
		p->next = NULL;
		//printf("done\n");
	}
	return 1;
}

/**
 * Judge Is Accpeted
 */
int isAccpted(const char *str,int threshold,int *fitLen)
{
	_FilterData p = pFilterData;
	int accMin = threshold + 1;
	int rejMin = threshold + 1;
	int accMinOffset=0; 
	int rejMinOffset=0;
	int i;
	int tmpMin;
	int tmpMinOffset;
	int minLen;
	int maxLen;
	int ed;
	//int succflag = 0;
	while(p != NULL)
	{
		minLen = p->cmpLen - threshold;
		maxLen = p->cmpLen + threshold+1;
		tmpMin = threshold + 1;
		for(i=minLen;i<=maxLen;i++)
		{
			if((ed = editDistanceS(p->cmpStr,p->cmpLen,str,i)) <= threshold)
			{
			//debug info
			/*
			succflag = 1;
			printf("{\n[%s][",p->cmpStr);
			int z;
			for(z=0;z<i;z++)
			{
				putchar(str[z]);
			}
			printf("]\n[%d],[%d]\n",p->cmpLen,i);
			printf("ed: %d\n}\n",editDistanceS(p->cmpStr,p->cmpLen,str,i));
			*/
			// end of debug info
				if(tmpMin > ed)
				{
					tmpMin = ed;
					tmpMinOffset = i;
				}
			}
		}
		
		if(p->acc == 1 && accMin > tmpMin)
		{
			accMinOffset = tmpMinOffset;
			accMin = tmpMin;
			//printf("acc -- ");
		}else if(p->acc == 0 && rejMin > tmpMin) 
		{
			rejMinOffset = tmpMinOffset;
			rejMin = tmpMin;
		}
		//rejMinOffset = rejMin < tmpMin ? rejMinOffset : tmpMinOffset;
		p = p-> next;
	}
	*fitLen = accMin <= rejMin ? 
			(accMin <= threshold ? accMinOffset : 0 ): 
			(rejMin <= threshold ? rejMinOffset : 0 );
	/*
	if(succflag == 1)
	{
		printf("\naccMin:%d rejMin:%d threshold:%d\n",accMin,rejMin,threshold);
		getchar();
	}
	*/
	return (accMin <= rejMin) && (accMin <= threshold );
}



/**
 * Data File Handle
 */
inline int isData(char ch)
{
	if(MIDDLEOF(ch,'A','Z') || MIDDLEOF(ch,'a','z') || MIDDLEOF(ch,'0','9')	)
		return 1;
	else return 0;
}


//int stackData(StackInfo *myStack,const char *content,int len,const char *toCompare,int cLen,int threshold)
int stackData(StackInfo *myStack,
		AcceptStr acceptStr,
		const char *content,
		int len,
		int threshold)
{
	int i;
	//int k;
	//int minLen = cLen-threshold;
	//int maxLen = cLen+threshold+1;
	int fitLen;
	myStack->top = 0;
	int unlock = 1;
	//int min;
	//int minid;
	//int ed;
	for(i=0;i<len;i++)
	{
		//if(( MIDDLEOF(content[i],'A','Z') || MIDDLEOF(content[i],'a','z')
		//	|| MIDDLEOF(content[i],'0','9') ))
		if(isData(content[i]))
		{
			if(unlock)
			{
				//(const char *str,int threshold,int *fitLen)
				if(acceptStr(content+i,threshold,&fitLen))
				{
					//printf("accept ... ");
					i += fitLen;
					if(myStack->top < MAX_STACK)
					{
						myStack->data[myStack->top] = i;
						myStack->top ++ ;
					}
				}else
				{
					i += fitLen;
				}
				unlock = 0;
			}
		}else
		{
			unlock = 1;
		}
	}
	return 1;
}


unsigned int getReferenceHeadOffset()
{
	int i;
	int mypclen = getPclen();
	unsigned int myTags;
	unsigned int nowTag;
	for(i=0;i<mypclen;i++)
	{
		myTags = *(getTags()+i);
		while((nowTag=tokenPop(&myTags)) > 0)
		{
			if(nowTag == 1)
			{
				return i;
			}
		}
		
	}
	return 0;
}

int printfContext(int refOffset)
{
	int x;
	
	printf("\n>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\n");
	for(x=refOffset-100;x<refOffset;x++) putchar(*(getPcontent()+x));
	printf("\n================================================================\n");
	for(x=refOffset;x<refOffset+100;x++) putchar(*(getPcontent()+x));
	printf("\n<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<\n");
	//getchar(); getchar();
	return 1;
}



inline int allZero(StackInfo info[],int len)
{
	int i;
	for(i=0;i<len;i++)
	{
		if(info[i].top >0 ) return 0;
	}
	return 1;
}

inline int maxTop(StackInfo info[],int len)
{
	int i = 0;
	int max = -1;
	int maxid = -1;
	for(i=0;i<len;i++)
	{
		if(info[i].top <= 0 ) continue;
		if(info[i].data[info[i].top-1] > max)
		{
			max = info[i].data[info[i].top-1];
			maxid = i;
		}
	}
	return maxid;
}


int rateWrite(FILE *fp,int start,double rate)
{
	int iRate = rate * 5;
	for(int i=0;i<5;i++)
	{
		fprintf(fp,"%d:%d ",start+i,iRate == i ? 1 : 0 );
	}
	return 1;
}

int gen26ToEnd(FILE *fp,int offset)
{
	//26 ~ 31
	fprintf(fp,"26:%d ",(hasPPafterTheOffset(offset,200)?1:0));
	fprintf(fp,"27:%d ",(hasPPafterTheOffset2(offset,200)?1:0));
	fprintf(fp,"28:%d ",(hasYearafterTheOffset(offset,200)?1:0));
	fprintf(fp,"29:%d ",(hasNameafterTheOffset0(offset,100)?1:0));
	fprintf(fp,"30:%d ",(hasNameafterTheOffset1(offset,100)?1:0));
	fprintf(fp,"31:%d ",(hasNameafterTheOffset2(offset,200)?1:0));

	fprintf(fp,"32:%d ",(hasPPafterTheOffset(offset,100)?1:0));
	fprintf(fp,"33:%d ",(hasPPafterTheOffset2(offset,100)?1:0));
	fprintf(fp,"34:%d ",(hasYearafterTheOffset(offset,30)?1:0));
	fprintf(fp,"35:%d ",(hasNameafterTheOffset0(offset,30)?1:0));
	fprintf(fp,"36:%d ",(hasNameafterTheOffset1(offset,30)?1:0));
	fprintf(fp,"37:%d ",(hasNameafterTheOffset2(offset,30)?1:0));	
	//32
	//fprintf(fp,"32:%f ",(double)offset/getPclen()); // percent of the location
	rateWrite(fp,42,(double)offset/getPclen());
	
	//33,34
	int totalPP = 0;
	int ppnumBeforeOffset = 0;
	int nowOffset = 0;
	while((nowOffset=hasPPafterTheOffset(nowOffset,getPclen())) != 0) totalPP++;
	while((nowOffset=hasPPafterTheOffset(nowOffset,getPclen())) != 0)
	{
		if(nowOffset > offset) break;
		ppnumBeforeOffset++;
	}
	//fprintf(fp,"33:%f ",totalPP == 0 ? 0 : (double)ppnumBeforeOffset/totalPP);
	rateWrite(fp,47,totalPP == 0 ? -1 : (double)ppnumBeforeOffset/totalPP);
	
	nowOffset = 0;
	ppnumBeforeOffset = 0;
	while((nowOffset=hasPPafterTheOffset2(nowOffset,getPclen())) != 0) totalPP++;
	while((nowOffset=hasPPafterTheOffset2(nowOffset,getPclen())) != 0)
	{
		if(nowOffset > offset) break;
		ppnumBeforeOffset++;
	}
	//fprintf(fp,"34:%f ",totalPP == 0 ? 0 : (double)ppnumBeforeOffset/totalPP);
	rateWrite(fp,52,totalPP == 0 ? -1 : (double)ppnumBeforeOffset/totalPP);
	
	// 35
	int totalYear = 0;
	int yearnumBeforeOffset = 0;
	nowOffset = 0;
	while((nowOffset=hasYearafterTheOffset(nowOffset,getPclen()))) totalYear++;
	while((nowOffset=hasYearafterTheOffset(nowOffset,getPclen())))
	{
		if(nowOffset > offset) break;
		yearnumBeforeOffset ++;
	}
	//fprintf(fp,"35:%f ",totalYear == 0 ? 0 : (double)yearnumBeforeOffset/totalYear);
	rateWrite(fp,57,totalYear == 0 ? -1 : (double)yearnumBeforeOffset/totalYear);
	
	// 36 ~ 38 
	int totalName = 0;
	int namenumBeforeOffset = 0;
	nowOffset = 0;
	while((nowOffset = hasNameafterTheOffset0(nowOffset,getPclen()))) totalName++;
	while((nowOffset = hasNameafterTheOffset0(nowOffset,getPclen())))
	{
		if(nowOffset > offset) break;
		namenumBeforeOffset ++;
	}
	//fprintf(fp,"36:%f ",totalName == 0 ? 0 : (double)namenumBeforeOffset/totalName);
	rateWrite(fp,62,totalName == 0 ? -1 : (double)namenumBeforeOffset/totalName);
	
	
	nowOffset = 0;
	namenumBeforeOffset = 0;
	while((nowOffset = hasNameafterTheOffset1(nowOffset,getPclen()))) totalName++;
	while((nowOffset = hasNameafterTheOffset1(nowOffset,getPclen())))
	{
		if(nowOffset > offset) break;
		namenumBeforeOffset ++;
	}
	//fprintf(fp,"37:%f ",totalName == 0 ? 0 : (double)namenumBeforeOffset/totalName);
	rateWrite(fp,67,totalName == 0 ? -1 : (double)namenumBeforeOffset/totalName);
	
	nowOffset = 0;
	namenumBeforeOffset = 0;
	while((nowOffset = hasNameafterTheOffset2(nowOffset,getPclen()))) totalName++;
	while((nowOffset = hasNameafterTheOffset2(nowOffset,getPclen())))
	{
		if(nowOffset > offset) break;
		namenumBeforeOffset ++;
	}
	//fprintf(fp,"38:%f ",totalName == 0 ? 0 : (double)namenumBeforeOffset/totalName);
	rateWrite(fp,72,totalName == 0 ? -1 : (double)namenumBeforeOffset/totalName);
	
	//printf("[%d]",hasPPafterTheOffset(maxoffset,100)?1:0);
	//printf("[%d]",hasYearafterTheOffset(maxoffset,100)?1:0);
	/*
	if(isPositive)
	{
		printf("[true:%d:%d:%d%d%d]",hasPPafterTheOffset(maxoffset,200),
					hasYearafterTheOffset(maxoffset,200),
					hasNameafterTheOffset0(maxoffset,100),
					hasNameafterTheOffset1(maxoffset,100),
					hasNameafterTheOffset2(maxoffset,200));
		fflush(NULL);
	}
		*/
	return 1;
}



//int bingo = 0;
int generateSample(const char* fileName,int isDir)
{
	FILE *fp;
	int trainOrTest;
	//if(strcmp("data/oraby/A model for estimating trace-sample miss ratios.txt",fileName)!=0) return 1;
        if(isDir)
        {
                printf("ignore dir:%s\n",fileName);
                return 1;
        }
	if(rand()%2) //train is 50% and test is 50%
	{
		fp = fpTrain;
		trainOrTest = 1;
	}else
	{
		trainOrTest = 0;
		fp = fpTest;
	}
	
	
	
        printf("[%d] %s:%s . . . ",id,(trainOrTest?"train":"test"),fileName);
        fflush(NULL);
        
        // parse tag or etc ,move data to RAM
 	initContent();
	if(!parseFile(fileName))
	{
		fprintf(stderr,"[[error parsing file : #%s#]]",fileName);
		getchar();
		return 0;
	}
	
	// search from db
	//int hasInfoInDB = getFeature(fileName,&myFeatureDataContainer);
	//if(hasInfoInDB)
	
	if(!getFeature(fileName,&myFeatureDataContainer))
	{
		int i;
		int j;
		int threshold;
		unsigned int refOffset;
		StackInfo info[FEATURE_SIZE];
		for(threshold=0;threshold<FEATURE_SIZE;threshold++) 
			stackData(&info[threshold],isAccpted,getPcontent(),getPclen(),threshold);
			
		unsigned int count[FEATURE_SIZE];
		unsigned int status[FEATURE_SIZE];
		int maxid;
		int maxoffset;
	
		int isPositive;
	
		fprintf(fp,"#paper %d (%s)\n",id,fileName);
		
		// DB data set
		myFeatureData.qid = id;
	
		refOffset = getReferenceHeadOffset();
		for(i=0;i<FEATURE_SIZE;i++) count[i]=1;
		int featureNumber = 0;
		
		while(!allZero(info,FEATURE_SIZE))
		{
			featureNumber ++ ;
			
			maxid = maxTop(info,FEATURE_SIZE);
			maxoffset = info[maxid].data[info[maxid].top-1];
			isPositive = VALUESDIFF(refOffset,maxoffset) < 10 ;
			
			myFeatureData.positive = isPositive;
			myFeatureData.offset = maxoffset;
			
			for(i=0;i<FEATURE_SIZE;i++)
			{
				status[i] = 0;
				myFeatureData.t[i] = 0;
				if(VALUESDIFF(info[i].data[info[i].top-1],maxoffset)<10)
				{
					info[i].top--;
					status[i] = count[i];
					count[i]++;
					
					//DB data set
					myFeatureData.t[i] = count[i];
				}
			
			}
			if(!insertFeature(fileName,myFeatureData))
			{
				fprintf(stderr,"[DB] insertFeature()(1): error --%d",__LINE__);
				getchar();
			}
		
			fprintf(fp,"%c1 qid:%d ",isPositive?'+':'-',id);
			//printf("\n");
			for(i=0;i<FEATURE_SIZE;i++)
			{
				for(j=0;j<4;j++)
				{
					fprintf(fp,"%d:%d ",i*5+j+1,status[i]==(j+1));
					//printf("%d:%d ",i*5+j+1,status[i]==(j+1));
				}
				fprintf(fp,"%d:%d ",i*5+5,status[i]>=5);
				//printf("%d:%d ",i*5+5,status[i]>=5);
				//fprintf(fp,"%d:%d ",i*5+(status[i]>5?5:status[i]),status[i]>0);
				//printf("%d:%d ",i,status[i]);
				//printf("%d:%d ",i*5+(status[i]>5?5:status[i]),status[i]>0);
			}

			gen26ToEnd(fp,maxoffset);

			
			//printf("\n");
			fprintf(fp,"\n");
		}
		
		//db insert
		if(!insertFeatureInfo(fileName,featureNumber))
		{
			fprintf(stderr,"[DB] insertFeatureInfo()(2): error %d",__LINE__);
			getchar();
		}
	}else
	{
		for(int i=0;i<myFeatureDataContainer.top;i++)
		{
			//printf("%d~",myFeatureDataContainer.data[i].qid);
			fprintf(fp,"%c1 ",myFeatureDataContainer.data[i].positive?'+':'-');
			for(int j=0;j<5;j++)
			{
				for(int k=0;k<FEATURE_SIZE;k++)
				{
					fprintf(fp,"%d:%d ",j*5+k+1,
						myFeatureDataContainer.data[i].t[j]==(k+1));
				}
			//	printf("%d ",myFeatureDataContainer.data[i].t[j]);	
			}
			
			//printf("%d %d\n",myFeatureDataContainer.data[i].positive,
			//			myFeatureDataContainer.data[i].offset);
			gen26ToEnd(fp,myFeatureDataContainer.data[i].offset);
			fprintf(fp,"\n");
		}
		
	}
	
	
	printf("\t[done]\n");
        cleanContent();
        fileNum++;
        id++;
        return 1;
}


int getFileNum(){return fileNum;}

