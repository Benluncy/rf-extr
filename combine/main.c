#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include "minEditDistance.h"
#include "virtualcontent.h"
#include "dirTraversal.h"
#include "tokens.h"
#include "main.h"

int fileNum = 0;
FILE *fpTrain; // train.txt
FILE *fpTest; // test.txt

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


unsigned int getReferenceAreaOffset()
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

int id = 1;
//int bingo = 0;
int generateSample(const char* fileName,int isDir)
{
	FILE *fp;
	int trainOrTest;
        if(isDir)
        {
                printf("ignore dir:%s\n",fileName);
                return 1;
        }
	if(rand()%3)
	{
		fp = fpTrain;
		trainOrTest = 1;
	}else
	{
		trainOrTest = 0;
		fp = fpTest;
	}
	
        printf("%s:%s . . . ",trainOrTest?"train":"test",fileName);
        
 	initContent();
	if(!parseFile(fileName))
	{
		fprintf(stderr,"[[error parsing file : #%s#]]",fileName);
		getchar();
		return 0;
	}
	
	int i;
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
	
	refOffset = getReferenceAreaOffset();
	for(i=0;i<FEATURE_SIZE;i++) count[i]=1;
	while(!allZero(info,FEATURE_SIZE))
	{
		maxid = maxTop(info,FEATURE_SIZE);
		maxoffset = info[maxid].data[info[maxid].top-1];
		isPositive = VALUESDIFF(refOffset,maxoffset) < 10 ;
		for(i=0;i<FEATURE_SIZE;i++)
		{
			status[i] = 0;
			if(VALUESDIFF(info[i].data[info[i].top-1],maxoffset)<10)
			{
				info[i].top--;
				status[i] = count[i];
				count[i]++;
			}
			
		}
		
		fprintf(fp,"%c1 qid:%d ",isPositive?'+':'-',id);
		//printf("\n");
		for(i=0;i<FEATURE_SIZE;i++)
		{
			fprintf(fp,"%d:%d ",i*5+(status[i]>5?5:status[i]),status[i]>0);
			//printf("%d:%d ",i,status[i]);
			//printf("%d:%d ",i*5+(status[i]>5?5:status[i]),status[i]>0);
		}
		//printf("\n");
		fprintf(fp,"\n");
	}
	id++;
	
	printf("\t[done]\n");
        cleanContent();
        fileNum++;
        return 1;
}


int main(int argc,char *argv[])
{
	//int insertFilterData(int accept,const char *cmpStr,int cmpLen);
	//fp = fopen("sample.txt","w");
	//char ch;
	//int i;
	fpTrain = fopen("train.txt","w"); // train.txt
	fpTest = fopen("test.txt","w"); // test.txt     
	srand((unsigned int)time(NULL));

	initFilterData();
	insertFilterData(1,"REFERENCES",strlen("REFERENCES"));
	insertFilterData(0,"CONFERENCES",strlen("CONFERENCES"));
	//insertFilterData(1,"BIBLIOGRAPHY",strlen("BIBLIOGRAPHY"));
	insertFilterData(1,"BIBLIOGRAPHIES",strlen("BIBLIOGRAPHIES"));
	//insertFilterData(0,"AUTHOR BIOGRAPHY",strlen("AUTHOR BIOGRAPHY"));
	insertFilterData(0,"AUTHOR BIOGRAPHIES",strlen("AUTHOR BIOGRAPHIES"));
	if(fpTrain == NULL || fpTest == NULL)
	{
		fprintf(stderr,"error opening sample file\n");
		return -1;
	}
	//for(i=0;i<100;i++)
	dirTraversal("data/",1,generateSample);
	//printf("done(%d/%d)\n",bingo,fileNum);
	printf("done \n total : %d\n",fileNum);
	cleanFilterData();
	fclose(fpTrain);
	fclose(fpTest);
	return 0;
}

