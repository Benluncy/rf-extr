#ifndef __FEATURE_GENERATE_H__
#define __FEATURE_GENERATE_H__

int initFindStart();
int freeFindStart(); 
int initFindEnd();
int freeFindEnd();

int predictStartOffset(const char *fileName);
int predictEndOffset(const char *fileName);


#endif // __FEATURE_GENERATE_H__
