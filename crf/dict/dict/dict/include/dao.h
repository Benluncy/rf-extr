#ifndef __DAO_H__
#define __DAO_H__

int dbConnect(void);
void dbFree(void);
int dbLock(void);
int dbUnlock(void);

int dbCreateNameDict(const char *name);
int dbCreateValDict(const char *name);
int dbCreateTable(void);

int dbAddNameDict(const char *dict,const char *name);
int dbAddValDict(const char *dict,const char *name,double val);


//int dbHandleGetNameDict(void *ch, int argc, char **argv, char **column);
//int dbHandleGetValDict(void *ch, int argc, char **argv, char **column);


int dbGetNameDict(const char *dict,const char *name);
int dbGetNameLikeDict(const char *dict,const char *name);
double dbGetValDict(const char *dict,const char *name);
double dbGetValLikeDict(const char *dict,const char *name);
#endif //  __DAO_H__
