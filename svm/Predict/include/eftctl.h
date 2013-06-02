#ifndef __EFTCTL_H__
#define __EFTCTL_H__
#include "svm_common.h"
#include "common.h"

//int genEndSampleCtl(const char* fileName,int isDir);
//int genEndSampleCtlW(const char* fileName,WORD *w,int limit);
int genEndSampleCtlW(const char* fileName,WORD features[ITNUM][FTWIDE],long offlist[ITNUM],int startOffset);
//int genStartSampleCtlW(const char* fileName,WORD features[ITNUM][FTWIDE],long offlist[ITNUM],int startOffset);


void openLogFile();
void closeLogFile();

#endif // __EFTCTL_H__
