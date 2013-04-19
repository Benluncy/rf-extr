#ifndef __EFTFUN_H__
#define __EFTFUN_H__
#include <stdio.h>
#include "persistence.h"
// data set
int cleanEndKWDContainer();
int insertEndKWD(const char *key);

//util
int hasDifferneces(int dest,int src);
unsigned int getReferenceEndOffset(void);

// set and get
featureDataContainer *getEndFeatureDataContainer(void);
featureData *getEndFeatureData(void);


// calculated data
int getLastYearOffset(unsigned int startOffset);

// main function 
int basicFilter(featureDataContainer *container,unsigned int startOffset);
int combineOffsets(featureDataContainer *container);
int makeSequenceForCombinedOffsets(featureDataContainer *container);



//file write
int genNextDataForEndfeature(FILE *fp,featureData fd,int step);

int prepareDensityData(void);

#endif // __EFTFUN_H__


