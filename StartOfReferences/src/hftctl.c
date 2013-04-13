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

//for debug
#include "debuginfo.h"

_FilterData pFilterData;

FILE *fpTrain; // train.txt
FILE *fpTest; // test.txt

int fileNum = 0;
featureDataContainer mfdc; // myFeatureDataContainer
featureData mfd; // myFeatureData
OffsetCallback functionList[6]={hasPPafterTheOffset,hasPPafterTheOffset2,
			hasYearafterTheOffset,hasNameafterTheOffset0,
			hasNameafterTheOffset1,hasNameafterTheOffset2,};



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

int rankWrite(FILE *fp,int start,double rank)
{
	for(int i=0;i<4;i++)
	{
		fprintf(fp,"%d:%d ",start+i,rank == i ? 1 : 0);
	}
	fprintf(fp,"%d:%d ",start+4,rank >=5 ? 1 : 0);
	return 1;
}

inline int offsetStat(int offset,int *totalData,int *beforeData,
					OffsetCallback offsetFunction)
{
	*totalData = 0;
	*beforeData = 0;
	int nowOffset = 0;
	//while((nowOffset=offsetFunction(nowOffset,getPclen())) != 0) (*totalData)++;
	while((nowOffset=offsetFunction(nowOffset,getPclen())) != 0)
	{
		(*totalData)++;
		if(nowOffset < offset) (*beforeData)++;
	}
	return 1;
	
}

inline int offsetBetweenStat(int offsetEnd,int offsetStart,int *dataBetween,
					OffsetCallback offsetFunction)
{
	*dataBetween = 0;
	if(offsetEnd < offsetStart)
	{
		int tmp = offsetEnd;
		offsetEnd = offsetStart;
		offsetStart = tmp ; 
	}
	while((offsetStart=offsetFunction(offsetStart,getPclen())) != 0)
	{
		if(offsetStart >= offsetEnd) break;
		(*dataBetween) ++ ;
	}
	return 1;
}

int gen26ToEnd(FILE *fp,featureData fd)
{
	//26 ~ 31
	int offset = fd.offset;


	//printf("prevOffset:%d nextOffset: %d",prevOffset,nextOffset);
	//getchar();
	
	int start = 26;
	int lmt = -200;
	//fp = stdout;
	for(;lmt<=200;lmt+=100)
	{
		fprintf(fp,"%d:%d ",start++,(hasPPafterTheOffset(offset,lmt)?1:0));
		fprintf(fp,"%d:%d ",start++,(hasPPafterTheOffset2(offset,lmt)?1:0));
		fprintf(fp,"%d:%d ",start++,(hasYearafterTheOffset(offset,lmt)?1:0));
		fprintf(fp,"%d:%d ",start++,(hasNameafterTheOffset0(offset,lmt)?1:0));
		fprintf(fp,"%d:%d ",start++,(hasNameafterTheOffset1(offset,lmt)?1:0));
		fprintf(fp,"%d:%d ",start++,(hasNameafterTheOffset2(offset,lmt)?1:0));		
	}
	
	//32
	//fprintf(fp,"32:%f ",(double)offset/getPclen()); // percent of the location
	rateWrite(fp,start++,(double)offset/getPclen());
	
	//==========================================================================================
	//33,34
	//int totalPP = 0;
	//int ppnumBeforeOffset = 0;
	//int nowOffset = 0;
	
	for(int i=0;i<6;i++)
	{
		rateWrite(fp,start,fd.fid[i][1] == 0 ? -1 : 
					(double)fd.fid[i][0]/fd.fid[i][1]);
		start+=5; //TODO ADD 
		fprintf(fp,"%d:%f ",start++,fd.density[i][0][0]);
		fprintf(fp,"%d:%f ",start++,fd.density[i][0][1]);
		fprintf(fp,"%d:%f ",start++,fd.density[i][1][0]);
		fprintf(fp,"%d:%f ",start++,fd.density[i][1][1]);
		rankWrite(fp,start,fd.seq[i][0]);
		start+=5;
		rankWrite(fp,start,fd.seq[i][1]);
		start+=5;
	}
	
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
	if(!getFeature(fileName,&mfdc)) // get One To Five from DB
	{
		int i;
		//int j;
		int threshold;
		unsigned int refOffset;
		StackInfo info[FEATURE_SIZE];
		for(threshold=0;threshold<FEATURE_SIZE;threshold++) 
			stackData(&info[threshold],isAccpted,getPcontent(),getPclen(),threshold);
			
		unsigned int count[FEATURE_SIZE];
		//unsigned int status[FEATURE_SIZE];
		int maxid;
		int maxoffset;
	
		int isPositive;
		
		// DB data set
		mfd.qid = id;
	
		refOffset = getReferenceHeadOffset();
		for(i=0;i<FEATURE_SIZE;i++) count[i]=1;
		
		//data
		mfdc.top = 0;
		
		while(!allZero(info,FEATURE_SIZE))
		{	
			maxid = maxTop(info,FEATURE_SIZE);
			maxoffset = info[maxid].data[info[maxid].top-1];
			isPositive = VALUESDIFF(refOffset,maxoffset) < 10 ;
			
			mfd.positive = isPositive;
			mfd.offset = maxoffset;
			
			for(i=0;i<FEATURE_SIZE;i++)
			{
				//status[i] = 0;
				mfd.t[i] = 0;
				if(VALUESDIFF(info[i].data[info[i].top-1],maxoffset)<10)
				{
					info[i].top--;
					//status[i] = count[i];
					count[i]++;
					
					//DB data set
					mfd.t[i] = count[i];
				}
			
			}
			if(!insertFeature(fileName,mfd))
			{
				fprintf(stderr,"[DB] insertFeature()(1): error --%d",__LINE__);
				getchar();
			}
		
			mfdc.data[mfdc.top] = mfd;
			mfdc.top ++ ;
			
		}
		
		//db insert
		if(!insertFeatureInfo(fileName,mfdc.top))
		{
			fprintf(stderr,"[DB] insertFeatureInfo()(2): error %d",__LINE__);
			getchar();
		}
	}
	
	printf(". ");fflush(NULL);//TODO TIPS
	
	// generate feature info data
	int beforeData;
	int totalData;
	for(int i=0;i<mfdc.top;i++)
	{
		
		for(int j=0;j<6;j++)
		{
			offsetStat(mfdc.data[i].offset,
					&totalData,&beforeData,functionList[j]);
			mfdc.data[i].fid[j][0] = beforeData;
			mfdc.data[i].fid[j][1] = totalData;
		}
	}
	
	//density and etc
	
	printf(". ");fflush(NULL);//TODO TIPS
	
	// adjacencyOffset setting ... 
	for(int i=0;i<mfdc.top;i++)
	{
		mfdc.data[i].adjacencyOffset[0] = 0;
		mfdc.data[i].adjacencyOffset[1] = getPclen();
		for(int j=0;j<mfdc.top;j++)
		{
			if(i == j) continue;
			if(mfdc.data[i].offset < 
						mfdc.data[i].offset &&
					mfdc.data[i].adjacencyOffset[0] < 
						mfdc.data[i].offset)
				mfdc.data[i].adjacencyOffset[0] = 
					mfdc.data[i].offset ;
			else if(mfdc.data[i].offset > 
						mfdc.data[i].offset &&
					mfdc.data[i].adjacencyOffset[1] > 
						mfdc.data[i].offset)
				mfdc.data[i].adjacencyOffset[1] = 
					mfdc.data[i].offset ;
		}
	}
	
	
	printf(". ");fflush(NULL);//TODO TIPS
	
	int diff;
	//density setting ... 
	for(int i=0;i<mfdc.top;i++)
	{
		for(int j=0;j<6;j++)
		{
			
			mfdc.data[i].density[j][0][0] = (mfdc.data[i].offset == 0 )? 0 : (double)mfdc.data[i].fid[j][0]/mfdc.data[i].offset;
			mfdc.data[i].density[j][0][1] = ((getPclen()-mfdc.data[i].fid[j][0]) == 0)? 0 :(double)(mfdc.data[i].fid[j][1]-mfdc.data[i].fid[j][0])/(getPclen()-mfdc.data[i].fid[j][0]);
			

			offsetBetweenStat(mfdc.data[i].offset,
					mfdc.data[i].adjacencyOffset[0],
					&diff,functionList[j]);
			mfdc.data[i].density[j][1][0] = (mfdc.data[i].offset-mfdc.data[i].adjacencyOffset[0] == 0)? 0 :(double)diff/(mfdc.data[i].offset-mfdc.data[i].adjacencyOffset[0]);
			
			offsetBetweenStat(mfdc.data[i].offset,
					mfdc.data[i].adjacencyOffset[1],
					&diff,functionList[j]);
			mfdc.data[i].density[j][1][1] = ((-mfdc.data[i].offset+mfdc.data[i].adjacencyOffset[1]) == 0)?0:(double)diff/(-mfdc.data[i].offset+mfdc.data[i].adjacencyOffset[1]);
			
		}
		
	}
	
	#define quot(a,b) ((b==0)?0:a/b)
	// sequence setting ... 
	printf(". ");fflush(NULL);//TODO TIPS
	for(int i=0;i<mfdc.top;i++)
	{
		for(int k=0;k<6;k++)
		{
			mfdc.data[i].seq[k][0] = 1 ;
			mfdc.data[i].seq[k][1] = 1 ;
		}
				
		for(int j=0;j<mfdc.top;j++)
		{
			if(i==j) continue;
			for(int k=0;k<6;k++)
			{
				if(quot(mfdc.data[i].density[k][0][0],mfdc.data[i].density[k][0][1])
					> quot(mfdc.data[j].density[k][0][0],mfdc.data[j].density[k][0][1]))
					mfdc.data[i].seq[k][0] ++;
				if(quot(mfdc.data[i].density[k][1][0],mfdc.data[i].density[k][1][1])
					> quot(mfdc.data[j].density[k][1][0],mfdc.data[j].density[k][1][1]))
					mfdc.data[i].seq[k][1] ++;
			}
		}
	}
	
	
	printf(". ");fflush(NULL);//TODO TIPS
	// write sample
	fprintf(fp,"#paper %d[%s]\n",id,fileName);
	for(int i=0;i<mfdc.top;i++)
	{
		fprintf(fp,"%c1 ",mfdc.data[i].positive?'+':'-');
		for(int j=0;j<FEATURE_SIZE;j++)
		{
			for(int k=0;k<5;k++)
			{
				fprintf(fp,"%d:%d ",j*5+k+1,
					mfdc.data[i].t[j]==(k+1));
			}
		}
		gen26ToEnd(fp,mfdc.data[i]);
		fprintf(fp,"\n");
	}
		
	
	
	printf("\t[done]\n");
        cleanContent();
        fileNum++;
        id++;
        return 1;
}


int getFileNum(){return fileNum;}

void setTrainFile(FILE *fp){ fpTrain = fp;}
void setTestFile(FILE *fp){fpTest = fp;}
FILE * getTrainFile(){return fpTrain;}
FILE * getTestFile(){return fpTest;}


