#ifndef __HFTNPSE_H__
#define __HFTNPSE_H__
//head feature name, pages,sequences,and etc.


//for debug
void setContent(const char * content);
void defineStartAndEnd(int *offset,int *offend,int limit);

//
int isPageNumber(const char *content,int limit);

int hasPPafterTheOffset(int offset,int limit);
int hasPPafterTheOffset2(int offset,int limit);

int hasYearafterTheOffset(int offset,int limit);

int hasNameafterTheOffset0(int offset,int limit);
int hasNameafterTheOffset1(int offset,int limit);
int hasNameafterTheOffset2(int offset,int limit);

int hasSeqOfTheOffset(int offset,int limit);
int hasSeqOfTheOffset2(int offset,int limit);
#endif //__HFTNPSE_H__
