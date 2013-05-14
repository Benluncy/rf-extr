#ifndef __FEATURE_GENERATE_H__
#define __FEATURE_GENERATE_H__
#include "svm_common.h"



int initFindStart();
int freeFindStart(); 
int initFindEnd();
int freeFindEnd();

int loadModel(const char *modelfile,int start);
int freeModel(int start);

WORD doPredictStart(const char *fileName,MODEL *model);
int predictCtl(const char* fileName,int isDir);

int getTotalFile();
int getCorrectFile();



#endif // __FEATURE_GENERATE_H__
