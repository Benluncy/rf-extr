#ifndef __HEADFEATURES_H__
#define __HEADFEATURES_H__
#include <stdio.h>
//definitions

#define FEATURE_SIZE 5
#define MAX_STACK 2000

#define MIDDLEOF(a,b,c)  (a >= b && a <= c) // a is at the range of  [b,c]
#define VALUESDIFF(a,b) (a > b? (a - b) :( b - a)) //calculate difference distance of a and b

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

_FilterData pFilterData;

FILE *fpTrain; // train.txt
FILE *fpTest; // test.txt


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

// for debug
int printfContext(int refOffset);

//

//stack
int allZero(StackInfo info[],int len);
int maxTop(StackInfo info[],int len);

// sample generater
int generateSample(const char* fileName,int isDir);

// fileNum
int getFileNum();

#endif //__HEADFEATURES_H__
