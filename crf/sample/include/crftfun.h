#ifndef __CRFT_FUN_H__
#define __CRFT_FUN_H__

int digitlen(const char *str,int len);

int puredigit(const char *str,int len);

int valofdigit(const char *str,int len);

int yearlike(const char *str,int len);

int strfeature(const char *str,int len);

int monthlike(const char *str,int len);

int vollkwd(const char *str,int len);

int edsFlag(const char *str,int len);

int pagekwd(const char *str,int len);

int specialFlag(const char *str,int len);

int procFlag(const char *str,int len);

int deptFlag(const char *str);

int uniLtdFlag(const char *str);

int isArticle(const char *str,int len);

#endif// __CRFT_FUN_H__
