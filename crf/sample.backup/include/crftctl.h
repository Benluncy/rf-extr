#ifndef __CRFT_CTL_H__
#define __CRFT_CTL_H__
#include <stdio.h>


int genCRFSample(const char *path);
int genCRFSampleCtl(const char* fileName,int isDir);

void setCRFTrainFile(FILE *fp);
void setCRFTestFile(FILE *fp);
FILE * getCRFTrainFile();
FILE * getCRFTestFile();



#endif //__CRFT_CTL_H__
