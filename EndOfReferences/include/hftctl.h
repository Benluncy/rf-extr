#ifndef __HFTCTL_H__
#define __HFTCTL_H__
#include <stdio.h>
//definitions

#define FEATURE_SIZE 5
#define MAX_STACK 2000

#ifndef MIDDLEOF
#define MIDDLEOF(a,b,c)  (a >= b && a <= c) // a is at the range of  [b,c]
#endif
#ifndef VALUESDIFF
#define VALUESDIFF(a,b) (a > b? (a - b) :( b - a)) //calculate difference distance of a and b
#endif

typedef struct offsetStack
{
	int top;
	int data[MAX_STACK];
} StackInfo;

// acc = 1 accept , acc = 0 reject
typedef struct filterData
{
	int acc; 
	char cmpStr[1024];
	int cmpLen;
	struct filterData * next;
	
} FilterData;

typedef FilterData * _FilterData;
typedef int (*AcceptStr)(const char *str,int threshold,int *fitLen); // for stack data , callback
typedef int (*OffsetCallback)(int offset,int limit); // for offset callback

int offsetStat(int nowOffset,int *totalData,int *beforeData,OffsetCallback offsetFunction);
int offsetBetweenStat(int offsetEnd,int offsetStart,int *dataBetween,OffsetCallback offsetFunction);

//feature
int initFilterData(void);
int cleanFilterData(void);
int insertFilterData(int accept,const char *cmpStr,int cmpLen);

//judge
int isAccpted(const char *str,int threshold,int *fitLen);



//
int stackData(StackInfo *myStack,AcceptStr acceptStr,const char *content,int len,int threshold);

//in fact , it is only the offset at start of references area
unsigned int getReferenceHeadOffset(void);



//

//stack
int allZero(StackInfo info[],int len);
int maxTop(StackInfo info[],int len);

// sample generater
//int generateSample(const char* fileName,int isDir);
int genStartSampleCtl(const char* fileName,int isDir);

// set && get
int getFileNum();

void setTrainFile(FILE *fp);
void setTestFile(FILE *fp);
FILE * getTrainFile(void);
FILE * getTestFile(void);


#endif //__HFTCTL_H__



