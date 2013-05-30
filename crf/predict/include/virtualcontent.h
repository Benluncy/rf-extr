#ifndef __VD_H__
#define __VD_H__


int parseFile(const char * fileName);
int initContent(void);
int cleanContent(void);


char *getContent(void);
char *getPcontent(void);
unsigned int *getOffset(void);
unsigned int *getTags(void);
int getClen(void);
int getPclen(void);

//inline
int getTokenId(char *tkname,unsigned int len);

#endif // __VD_H__

