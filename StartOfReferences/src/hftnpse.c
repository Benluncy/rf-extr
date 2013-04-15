#include "hftnpse.h"
#include "minEditDistance.h"
#include "virtualcontent.h"
#include "hftctl.h"
#include <stdio.h>
#include <string.h>
inline void defineStartAndEnd(int *offset,int *offend,int limit)
{
	int tmp;
	*offend = *offset + limit;
	if(*offset > *offend)
	{
		tmp = *offset;
		*offset = *offend;
		*offend = tmp;
	}
	if(*offset < 0) *offset = 0;
	if(*offend > getPclen()) *offend = getPclen();
	
}

inline int isPageNumber(const char *content,int limit)
{
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

int hasPPafterTheOffset(int offset,int limit)
{
	//int j;
	int i;	
	int offend;
	char *content = getPcontent();
	defineStartAndEnd(&offset,&offend,limit);
	int pageOffset;
	for(i=offset;i<offend;i++)
	{
		if(fitPattern('n',content[i]))
			if((pageOffset = isPageNumber(content+i,offend-i)) != 0)
				return i+pageOffset;
	}
	return 0;
}

int hasPPafterTheOffset2(int offset,int limit)
{
	int j;
	int i;	
	int offend;
	char *content = getPcontent();
	defineStartAndEnd(&offset,&offend,limit);
	int pageOffset;
	for(i=offset;i<offend;i++)
	{
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
	//int j;
	int i;	
	int offend;
	char *content = getPcontent();
	defineStartAndEnd(&offset,&offend,limit);
	for(i=offset;i<offend-3;i++) // 19xx i_max = 1
	{
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
	//int j;
	int i;	
	int offend;
	char *content = getPcontent();
	defineStartAndEnd(&offset,&offend,limit);
	int tag = 0;
	for(i=offset;i<offend;i++)
	{
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
	//int j;
	int i;	
	int offend;
	char *content = getPcontent();
	defineStartAndEnd(&offset,&offend,limit);
	int tag = 0;
	for(i=offset;i<offend;i++)
	{
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
	//int j;
	int i;	
	int offend;
	char *content = getPcontent();
	defineStartAndEnd(&offset,&offend,limit);
	int tag = 0;
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

int hasSeqOfTheOffset(int offset,int limit)
{
	int offend;
	char *content = getPcontent();
	defineStartAndEnd(&offset,&offend,limit);
	for(int i=offset;i<offend;i++)
	{
		if(content[i]=='['||content[i]=='|')
		{
			for(int j=i+1;j<offend && j < i+4;j++)
			{
				if(content[j] == ']' || content[j] == '|')
				{
					if((j-i)==1) break;
					
					//printf("${");
					//for(int k=i;k<=j;k++) putchar(content[k]);
					//printf("}");
					return j;
				}
				if((content[j]<'0' || content[j] >'9') 
					&& content[j] != 'i' && content[j] != 'l')
					break;
			}
		}
	}
	return 0;
}


int hasSeqOfTheOffset2(int offset,int limit)
{
	int offend;
	char *content = getPcontent();
	defineStartAndEnd(&offset,&offend,limit);
	int fail = 0;
	for(int i=offset;i<offend;i++)
	{
		if(content[i]=='['||content[i]=='|')
		{
			for(int j=i+1;j<offend && j < i+6;j++)
			{
				switch(content[j])
				{
					case ']':
					case '|':
					case '!':
						//printf("${");
						//for(int k=i;k<=j;k++) putchar(content[k]);
						//printf("}");
						if((j-i)==1)
						{
							fail = 1;
							break;
						}
						return j;
					case '(':
					case ')':
					case ',':
					case '~':
					case '@':
					case '#':
					case '%':
					case '$':
					case '`':
					case '*':
						fail = 1;
						break;
				}
				if(fail == 1)
				{
					fail = 0;
					break;
				}
			}
		}
	}
	return 0;
}

