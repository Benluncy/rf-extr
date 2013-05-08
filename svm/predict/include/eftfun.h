#ifndef __EFTFUN_H__
#define __EFTFUN_H__
#include <stdio.h>
#include "persistence.h"
#include "svm_common.h"



// data set
int cleanEndKWDContainer();
int insertEndKWD(const char *key);



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
//int makeSequenceForCombinedOffsets(endFeatureDataContainer *container);



//file write
//int genNextDataForEndfeature(FILE *fp,endFeatureData fd,int step);
int genNextDataForEndfeatureW(WORD *w,endFeatureData fd,int start);

int prepareDensityData(void);



void setNextElemOffset(int neo);
int chkSpecialFlag(void);

//int getMaxLen(void);
#endif // __EFTFUN_H__


