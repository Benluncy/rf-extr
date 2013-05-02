#ifndef __EFTFUN_H__
#define __EFTFUN_H__
#include <stdio.h>
#include "persistence.h"

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

// data set
int cleanEndKWDContainer();
int insertEndKWD(const char *key);

//util
int haveDifferneces(int dest,int src);
int haveDiffernecesE(int dest,int src);
int haveDiffernecesH(int dest,int src);
int haveDiffernecesD(int dest,int src);

unsigned int getReferenceEndOffset(void);

// set and get
endFeatureDataContainer *getEndFeatureDataContainer(void);
endFeatureData *getEndFeatureData(void);


// calculated data
int getLastYearOffset(unsigned int startOffset,int limit);
int getLastPageOffset(unsigned int startOffset,int limit);
int getLastPage2Offset(unsigned int startOffset,int limit);

// main function 
int basicFilter(endFeatureDataContainer *container,unsigned int startOffset);
int combineOffsets(endFeatureDataContainer *container);
int makeSequenceForCombinedOffsets(endFeatureDataContainer *container);



//file write
int genNextDataForEndfeature(FILE *fp,endFeatureData fd,int step);

int prepareDensityData(void);


int allMarked(int a[],int len);

int powerWrite(FILE *fp,int start,int value,int len);
int powerWriteNoMore(FILE *fp,int start,int value,int len);
int rankWriteNoMore(FILE *fp,int start,int rank,int len);


void setNextElemOffset(int neo);
int chkSpecialFlag(void);

//int getMaxLen(void);
#endif // __EFTFUN_H__


