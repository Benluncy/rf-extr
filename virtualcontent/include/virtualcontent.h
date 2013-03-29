#ifndef __VD_H__
#define __VD_H__


int parseFile(const char * fileName);
int initContent();
int cleanContent();


char *getContent();
char *getPcontent();
unsigned int *getOffset();
unsigned int *getTags();
int getClen();
int getPclen();



#endif // __VD_H__

