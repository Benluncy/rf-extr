#ifndef __DEBUG_INFO_H__
#define __DEBUG_INFO_H__

#ifndef DEBUGFLAG
#define DEBUGFLAG 1
#endif //DEBUGFLAG


#ifndef SWAP
#define SWAP(a,b) {a=a^b;b=a^b;a=a^b;}
#endif //SWAP


// for debug
int printfContext(int offset);
int printfContextS(int offset,const char *str);
int printfContextF(int offset,const char *str,FILE *fp);

#endif // __DEBUG_INFO_H__
