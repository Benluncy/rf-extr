#ifndef __COMMON_H__
#define __COMMON_H__
#include <stdio.h>
#include "svm_common.h"

//for predict
#define FTWIDE 302 //feature number
#define ITNUM 512 // item number

#define thresholdForDifferneces  2
#define exThresholdForDifferences (thresholdForDifferneces*2)

#define thresholdForGetOffsetSuggestion(x)  x*0.1
#define T4GOS(x) thresholdForGetOffsetSuggestion(x)
#define INLMT(x) (editDistanceT(\
			x,\
			strlen(x),\
			content+i,\
			strlen(x)>strlen(content+i)?\
				strlen(content+i):\
				strlen(x),\
			T4GOS(strlen(x))\
			)!= -1)

#define MINANDNZ(x,y) (x!=0?(y!=0?(x>y?y:x):(x)):y)

#define ABSDIFF(x,y) (x>y?x-y:y-x)
#define INABSDIFF(x,y) (ABSDIFF(x,y)<=thresholdForDifferneces)
#define EXINABSDIFF(x,y) (ABSDIFF(x,y)<=exThresholdForDifferences)


//util
int haveDifferneces(int dest,int src);
int haveDiffernecesE(int dest,int src);
int haveDiffernecesH(int dest,int src);
int haveDiffernecesD(int dest,int src);

int allMarked(int a[],int len);

//int powerWrite(FILE *fp,int start,int value,int len);
//int powerWriteNoMore(FILE *fp,int start,int value,int len);
//int rankWriteNoMore(FILE *fp,int start,int rank,int len);

int powerWriteW(WORD *w,int start,int value,int len);
int powerWriteNoMoreW(WORD *w,int start,int value,int len);
int rankWriteNoMoreW(WORD *w,int start,int rank,int len);
int rankWriteW(WORD *w,int start,int rank,int len);
int rateWriteW(WORD *w,int start,double rate);


int setFtValue(WORD *w,int start,int val);
int setFtValueF(WORD *w,int start,double val);

#endif // __COMMON_H__
