#include <stdio.h>
#include <string.h>
#include "minEditDistance.h"
#include "virtualcontent.h"
#include "dirTraversal.h"
#include "tokens.h"
#include "main.h"

int fileNum = 0;

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
	_FilterData head = pFilterData;
	_FilterData p = head;
	if(head == NULL)
	{
		head = (_FilterData)malloc(sizeof(FilterData ));
		head->acc = accept;
		head->cmpLen = cmpLen;
		//head->cmpStr = strdup(cmpStr);
		sprintf(head->cmpStr,"%s",cmpStr);
		head->next = NULL;
	}else
	{
		while(p->next != NULL) p=p->next;
		p->next = (_FilterData)malloc(sizeof(FilterData ));
		p = p->next;
		p->acc =accept;
		p->cmpLen = cmpLen;
		sprintf(p->cmpStr,"%s",cmpStr);
		p->next = NULL;
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
	while(p != NULL)
	{
		minLen = p->cmpLen - threshold;
		maxLen = p->cmpLen + threshold+1;
		tmpMin = threshold + 1;
		for(i=minLen;i<maxLen;i++)
		{
			if((ed = editDistanceS(toCompare,cLen,content+i,j)) <= threshold)
			{
				
			}
		}
		
		
		if(p->acc) accMinOffset = acc;
		p = p-> next;
	}
	fitLen = accMinOffset;
	return (accMin < rejMin) && (accMin < threshold );
}
/**
//references
				min = threshold + 1;
				minid = minLen;
				for(j=minLen;j<maxLen;j++)
				{
					//if(editDistanceT(toCompare,cLen,content+i,j,threshold) >= 0)
					if((ed = editDistanceS(toCompare,cLen,content+i,j)) <= threshold)
					{
						if(min > ed)
						{
								min = ed;
								minid = j;
						}
					}
					
				}
				if(min < threshold + 1)
				{
					i += minid;
					if(myStack->top < MAX_STACK)
					{
						myStack->data[myStack->top] = i;
						myStack->top ++ ;
					
						//i += j;
						//break;
					}
				}
*/



/**
 * Data File Handle
 */


int isData(char ch)
{
	if(MIDDLEOF(ch,'A','Z') || MIDDLEOF(ch,'a','z') || MIDDLEOF(ch,'0','9')	)
		return 1;
	else return 0;
}




//int stackData(StackInfo *myStack,const char *content,int len,const char *toCompare,int cLen,int threshold)
int stackData(StackInfo *myStack,
		AcceptStr acceptStr,
		int threshold)
{
	int i,j;
	//int k;
	//int minLen = cLen-threshold;
	//int maxLen = cLen+threshold+1;
	int fitLen;
	myStack->top = 0;
	int unlock = 1;
	int min;
	int minid;
	int ed;
	for(i=0;i<len;i++)
	{
		if(( MIDDLEOF(content[i],'A','Z') || MIDDLEOF(content[i],'a','z')
			|| MIDDLEOF(content[i],'0','9') ))
		{
			if(unlock)
			{
				//(const char *str,int threshold,int *fitLen)
				if(acceptStr(content+i,threshold,&fitLen))
				{
					i += fitLen;
					if(myStack->top < MAX_STACK)
					{
						myStack->data[myStack->top] = i;
						myStack->top ++ ;
					}
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

FILE *fp;

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


//int bingo = 0;
int readFile(const char* fileName,int isDir)
{
        if(isDir)
        {
                printf("ignore dir:%s\n",fileName);
                return 1;
        }

        printf("handling : %s . . . ",fileName);
        
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
		stackData(&info[threshold],getPcontent(), getPclen(),"REFERENCES",strlen("REFERENCES"),threshold);
	
	unsigned int count[FEATURE_SIZE];
	unsigned int status[FEATURE_SIZE];
	int maxid;
	int maxoffset;
	
	int isPositive;
	
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
		
		fprintf(fp,"%c1 ",isPositive?'+':'-');
		for(i=1;i<=FEATURE_SIZE;i++)
			fprintf(fp,"%d:%d ",i,status[i]);
		fprintf(fp,"\n");
	}
	printf("\t[done]\n");
        cleanContent();
        fileNum++;
        return 1;
}


int main(int argc,char *argv[])
{
	//int insertFilterData(int accept,const char *cmpStr,int cmpLen);
	fp = fopen("sample.txt","w");
	//char ch;
	//int i;
	insertFilterData(1,"REFERENCES",strlen("REFERENCES"));
	insertFilterData(0,"CONFERENCES",strlenn("CONFERENCES"));
	if(fp == NULL)
	{
		fprintf(stderr,"error opening .. \n");
		return -1;
	}
	//for(i=0;i<100;i++)
	dirTraversal("data/",1,readFile);
	//printf("done(%d/%d)\n",bingo,fileNum);
	printf("done \n total : %d\n",fileNum);
	fclose(fp);
	return 0;
}

