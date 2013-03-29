#ifndef __MINIMUM_EDIT_DISTANCE_H__
#define __MINIMUM_EDIT_DISTANCE_H__

#ifndef __cplusplus
typedef int bool;
enum true_or_false{false,true};
#endif //__cplusplus


//inline int editDistance(const char *t,const char * s);
//inline int editDistanceS(const char *t,const char * s); // not sinsitive
inline int editDistance(const char *t,int tlen,const char * s,int slen);
inline int editDistanceS(const char *t,int tlen,const char * s,int slen);

/**
 * author fang
 */
inline int editDistanceT(const char *t,int tlen,const char * s,int slen,int threshold);


#endif // __MINIMUM_EDIT_DISTANCE_H__
