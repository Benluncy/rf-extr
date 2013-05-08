#ifndef __EFTCTL_H__
#define __EFTCTL_H__
#include "svm_common.h"

//int genEndSampleCtl(const char* fileName,int isDir);
int genEndSampleCtlW(const char* fileName,WORD *w,int limit);


void openLogFile();
void closeLogFile();

#endif // __EFTCTL_H__
