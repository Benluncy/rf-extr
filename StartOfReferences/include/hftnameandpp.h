#ifndef __HFTNAME_AND_PP_H__
#define __HFTNAME_AND_PP_H__
//for debug
void setContent(const char * content);

//
int isPageNumber(const char *content,int limit);

int hasPPafterTheOffset(int offset,int limit);
int hasPPafterTheOffset2(int offset,int limit);

int hasYearafterTheOffset(int offset,int limit);

int hasNameafterTheOffset0(int offset,int limit);
int hasNameafterTheOffset1(int offset,int limit);
int hasNameafterTheOffset2(int offset,int limit);

//int hasSeqOfTheOffset(int offset,int limit);
#endif //__HFTNAME_AND_PP_H__
