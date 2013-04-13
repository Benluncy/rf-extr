#ifndef __MINIMUM_EDIT_DISTANCE_H__
#define __MINIMUM_EDIT_DISTANCE_H__

#ifndef __cplusplus
typedef int bool;
enum true_or_false{false,true};
#endif //__cplusplus




//inline int editDistance(const char *t,const char * s);
//inline int editDistanceS(const char *t,const char * s); // not sinsitive
int editDistance(const char *t,int tlen,const char * s,int slen);
int editDistanceS(const char *t,int tlen,const char * s,int slen);

/**
 * author fang
 */
int editDistanceT(const char *t,int tlen,const char * s,int slen,int threshold);

/**
 * pattern 
 * 
 * "aaaeaaa" equals "abc-def" 
 */
int editDistanceP(const char *t,int tlen,const char * s,int slen);


//inline
int isSame(char a,char b);

// is ascii or num
int isData(char ch);

/**
 * a is pattern
 * b is data
 *
 * about a,
 * : a Ascii
 * : n Number
 * : d Data(ascii or number)
 * : e dElimiter
 * : s Space
 */
int fitPattern(char a,char b);

#endif // __MINIMUM_EDIT_DISTANCE_H__

