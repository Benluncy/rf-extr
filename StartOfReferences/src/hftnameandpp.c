#include "hftnameandpp.h"
#include "minEditDistance.h"
#include "virtualcontent.h"
#include "hftctl.h"
#include <stdio.h>
#include <string.h>

inline int isPageNumber(const char *content,int limit)
{
	//int editDistanceP(const char *t,int tlen,const char * s,int slen);
	//int i;
	//printf("[");
	//for(i=0;i<10;i++)
	//	putchar(content[i]);
	//printf("]");
	if(limit < 3 ) return 0;
	if(editDistanceP("nen",3,content,3)<=0)
		return 3;
	if(limit < 5) return 0;
	if(editDistanceP("nnenn",5,content,5)<=1)
		return 5;
	if(limit < 7) return 0;
	if(editDistanceP("nnnennn",7,content,7)<=1)
		return 7;
	if(limit < 9) return 0;
	if(editDistanceP("nnnnennnn",9,content,9)<=1)
		return 9;
	return 0;
}
/**
 * int offset : offset of pcontent
 * int limit : 
 *
 */
int hasPPafterTheOffset(int offset,int limit)
{
	int i;
	//int j;
	int offend = offset + limit;
	offend = offend >= getPclen() ? getPclen()-1 : offend; 
	//char *content = getPcontent();
	char *content = getPcontent();
	int pageOffset;
	int tmpForSwap;
	if(limit < 0)
	{
		tmpForSwap = offend;
		offend = offset;
		offset = tmpForSwap;
	}
	for(i=offset;i<offend;i++)
	{
		if(fitPattern('n',content[i]))
			if((pageOffset = isPageNumber(content+i,offend-i)) != 0)
				return i+pageOffset;
		//putchar(content[i]);
		//fflush(NULL);
		/*
		if(content[i]=='p' || content[i] == 'P')
		{
			if(content[i+1] == 'p'  || content[i] == 'P')
			{
				for(j=3;!fitPattern('n',content[i+j]);j++)
				{
					if(j > 4 && 
						(fitPattern('a',content[i+j]) ||
							 fitPattern('e',content[i+j])))
						continue;
				}
				if(isPageNumber(content+i+j)) return 1;
			}else if(editDistanceS("page",4,content+i,4) <= 1) // threshold == 1
			{
				for(j=4;!fitPattern('n',content[i+j]);j++)
				{
					if(j > 6 && 
						(fitPattern('a',content[i+j]) ||
							 fitPattern('e',content[i+j])))
						continue;
				}	
				if(isPageNumber(content+i+j)) return 1;
			}
		}*/
	}
	return 0;
}

int hasPPafterTheOffset2(int offset,int limit)
{
	int i;
	int j;
	int offend = offset + limit;
	offend = offend >= getPclen() ? getPclen()-1 : offend; 
	//char *content = getPcontent();
	char *content = getPcontent();
	int pageOffset;
	int tmpForSwap;
	if(limit < 0)
	{
		tmpForSwap = offend;
		offend = offset;
		offset = tmpForSwap;
	}
	for(i=offset;i<offend;i++)
	{
		//if(fitPattern('n',content[i]))
		//	if((pageOffset = isPageNumber(content+i)) != 0)
		//		return i+pageOffset;
		if(content[i]=='p' || content[i] == 'P')
		{
			if(content[i+1] == 'p'  || content[i] == 'P')
			{
				for(j=3;!fitPattern('n',content[i+j]);j++)
				{
					if(i+j > offend-2) return 0;
					if(j > 4 && 
						(fitPattern('a',content[i+j]) ||
							 fitPattern('e',content[i+j])))
						continue;
				}
				if((pageOffset = isPageNumber(content+i+j,offend-i-j))!= 0) 
					return i+j+pageOffset;
			}else if(editDistanceS("page",4,content+i,4) <= 1) // threshold == 1
			{
				for(j=4;!fitPattern('n',content[i+j]);j++)
				{
					if(i+j > offend-2) return 0;
					if(j > 6 && 
						(fitPattern('a',content[i+j]) ||
							 fitPattern('e',content[i+j])))
						continue;
				}	
				if((pageOffset = isPageNumber(content+i+j,offend-i-j))!= 0) 
					return i+j+pageOffset;
			}
		}
	}
	return 0;
}

int hasYearafterTheOffset(int offset,int limit)
{
	int i;
	int offend = offset + limit;
	offend = offend >= getPclen() ? getPclen()-1 : offend; 
	//char *content = getPcontent();
	char *content = getPcontent();
	int tmpForSwap;
	if(limit < 0)
	{
		tmpForSwap = offend;
		offend = offset;
		offset = tmpForSwap;
	}
	for(i=offset;i<offend-3;i++) // 19xx i_max = 1
	{
		//putchar(content[i]);
		//fflush(NULL);
		if((content[i]=='1' && content[i+1]=='9')||
			(content[i]=='2' && content[i+1]=='0'))
		{
			if(fitPattern('n',content[i+2])&&fitPattern('n',content[i+3])) 
				return i+4;
		}
	}
	return 0;
}


// Find String like :
// XXXX, X.
int hasNameafterTheOffset0(int offset,int limit)
{
	int i;
	//int j;
	int offend = offset + limit;
	int tag;
	offend = offend >= getPclen() ? getPclen()-1 : offend; 
	//char *content = getPcontent();
	char *content = getPcontent();
	int tmpForSwap;
	if(limit < 0)
	{
		tmpForSwap = offend;
		offend = offset;
		offset = tmpForSwap;
	}
	for(i=offset;i<offend;i++)
	{
		//putchar(content[i]);
		//fflush(NULL);
		if(i-tag>1) i--;
		tag = i;
		while(fitPattern('a',content[i]))
		{
			i++;
			if(i > offend-3) return 0;
		}
		if(i-tag>1 &&(','==content[i]))
		{
			i++;
			while(content[i]==' ') {
				i++;
				if(i > offend - 2) return 0;
			}
			if(fitPattern('a',content[i])&&('.' == content[i+1]))
			{
	//			printf("0『");
	//			for(j=tag;j<=i+1;j++) putchar(content[j]);
	//			printf("』");
				return i+2;
			}
		}
	}
	return 0;
}

// Find String like :
// XXXX, X.
int hasNameafterTheOffset1(int offset,int limit)
{
	int i;
	//int j;
	int offend = offset + limit;
	int tag;
	offend = offend >= getPclen() ? getPclen()-1 : offend; 
	//char *content = getPcontent();
	char *content = getPcontent();
	int tmpForSwap;
	if(limit < 0)
	{
		tmpForSwap = offend;
		offend = offset;
		offset = tmpForSwap;
	}
	for(i=offset;i<offend;i++)
	{
		//putchar(content[i]);
		//fflush(NULL);
		if(i-tag>1) i--;
		tag = i;
		while(fitPattern('a',content[i]))
		{
			i++;
			if(i > offend-3) return 0;
		}
		if(i-tag>1 &&(','==content[i] || '.'==content[i] || ' '==content[i]))
		{
			i++;
			while(content[i]==' ') {
				i++;
				if(i > offend - 2) return 0;
			}
			if(fitPattern('a',content[i])&&fitPattern('e',content[i+1]))
			{
	//			printf("1『");
	//			for(j=tag;j<=i+1;j++) putchar(content[j]);
	//			printf("』");
				return i+2;
			}
		}
	}
	return 0;
}

// Find String like :
// XXXX, X.
int hasNameafterTheOffset2(int offset,int limit)
{
	int i;
	//int j;
	int offend = offset + limit;
	int tag;
	offend = offend >= getPclen() ? getPclen()-1 : offend; 
	//char *content = getPcontent();
	char *content = getPcontent();
	int tmpForSwap;
	if(limit < 0)
	{
		tmpForSwap = offend;
		offend = offset;
		offset = tmpForSwap;
	}
	for(i=offset;i<offend;i++)
	{
		//putchar(content[i]);
		//fflush(NULL);
		if(i-tag>1) i--;
		tag = i;
		while(fitPattern('a',content[i]))
		{
			i++;
			if(i > offend-3) return 0;
		}
		if(i-tag>1 &&(fitPattern('e',content[i])|| content[i] == ' '))
		{
			i++;
			while(content[i]==' ') {
				i++;
				if(i > offend - 2) return 0;
			}
			if(fitPattern('a',content[i])&&fitPattern('e',content[i+1]))
			{
	//			printf("2『");
	//			for(j=tag;j<=i+1;j++) putchar(content[j]);
	//			printf("』");
				return i+2;
			}
		}
	}
	return 0;
}

/**
 * Data File Handle
 */
inline int isData(char ch)
{
	if(MIDDLEOF(ch,'A','Z') || MIDDLEOF(ch,'a','z') || MIDDLEOF(ch,'0','9')	)
		return 1;
	else return 0;
}



