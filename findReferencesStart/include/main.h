#ifndef __MAIN_H__
#define __MAIN_H__

// definitions 

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



// functions to export 

// about filter data
int initFilterData();
int cleanFilterData();
int insertFilterData(int accept,const char *cmpStr,int cmpLen);


// isAccpted
int isAccpted(const char *str,int threshold,int *fitLen);


// 
//inline int isData(char ch); // [A,Z]||[a,z]||[0,9]

// data handle
int stackData(StackInfo *myStack,int (*callback)(const char *str,int threshold,int *fitLen),
							const char *content,int len,int threshold);


#endif // __MAIN_H__

