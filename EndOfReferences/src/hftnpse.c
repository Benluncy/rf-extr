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

inline int ch2int(char ch)
{
	int val;
	if(ch =='I' || ch == 'l')
		val = 1;
	else 
		val = ch - '0';
	//if(val > 9 || val < 0) return -1;
	if(val > 9 || val < 0) return 0;
	return val; 
}

inline int isPageNumber(const char *content,int limit)
{
	int a = 0;
	int b = 0;
	if(limit < 3 ) return 0;
	//if(editDistanceP("nen",3,content,3)<=0)
	//	return 3;
	if(limit > 9 ) limit = 9;
	
	switch(limit)
	{
	case 9:
		if(editDistanceP("nnnnennnn",9,content,9)<=1) // 1?
		{
			a = ch2int(content[0])*1000 + ch2int(content[1])*100 + 
				ch2int(content[2])*10+ch2int(content[3]);
			b = ch2int(content[5])*1000 + ch2int(content[6])*100 + 
				ch2int(content[7])*10+ch2int(content[8]);
			if(b>a) return 9;
		}
			
	case 8:
		if(editDistanceP("nnnennnn",8,content,8)<=0) // 1?
		{
			a = ch2int(content[0])*1000 + ch2int(content[1])*100 + 
				ch2int(content[2])*10+ch2int(content[3]);
			b = ch2int(content[5])*1000 + ch2int(content[6])*100 + 
				ch2int(content[7])*10+ch2int(content[8]);
			if(b>a) return 8;
		}
			
	case 7:
		if(editDistanceP("nnnennn",7,content,7)<=0) // 1?
		{
			a = ch2int(content[0])*100 + ch2int(content[1])*10 + ch2int(content[2]);
			b = ch2int(content[0])*100 + ch2int(content[1])*10 + ch2int(content[2]);
			if(b > a) return 7;
		}
	case 6:
	case 5:
		if(editDistanceP("nnenn",5,content,5)<=0) // 1?
		{
			a = ch2int(content[0])*10 + ch2int(content[1]);
			b = ch2int(content[3])*10 + ch2int(content[4]);
			if(b > a) return 5;
		}
	case 4:
		if(fitPattern('n',content[0])&&(content[1]=='-')&&
			fitPattern('n',content[2])&&fitPattern('n',content[3]))
		{
			a = ch2int(content[0]);
			b = ch2int(content[2])*10 + ch2int(content[3]);
			if(b > a) return 4;
		}
	case 3:
		if(fitPattern('n',content[0])&&(content[1]=='-')&&fitPattern('n',content[2]))
		{
			a = ch2int(content[0]);
			b = ch2int(content[2]);
			if(b > a) return 3;
		}	
	default :
		return 0;
	}
	return 0;
}
/*
inline int isPageNumberS(const char *content,int limit)
{
	if(limit < 3 ) return 0;
	if(editDistanceP("nen",3,content,3)<=0)
		return 3;
	if(editDistanceP("nen",3,content,4)<=1)
		return 4;
	if(editDistanceP("nenn",4,content,4)<=1)
		return 4;
		
	if(editDistanceP("nnenn",5,content,4)<=1)
		return 4;
	if(editDistanceP("nenn",4,content,6)<=1)
		return 4;
	
	if(limit < 5) return 0;
	if(editDistanceP("nnenn",5,content,5)<=1)
		return 5;
	if(editDistanceP("nnenn",5,content,6)<=1)
		return 6;
	

	if(editDistanceP("nnnennn",7,content,6)<=1)
		return 6;
	if(editDistanceP("nnennn",6,content,6)<=1)
		return 6;
		
	if(limit < 7) return 0;
	if(editDistanceP("nnnennn",7,content,7)<=1)
		return 7;
	if(editDistanceP("nnnennn",7,content,8)<=1)
		return 8;
	
	if(editDistanceP("nnnnennnn",9,content,8)<=1)
		return 8;
	if(editDistanceP("nnnennnn",8,content,8)<=1)
		return 8;	
	if(limit < 9) return 0;
	if(editDistanceP("nnnnennnn",9,content,9)<=1)
		return 9;
	if(editDistanceP("nnnnennnn",9,content,10)<=1)
		return 9;
	
	return 0;
}*/

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
				return i+pageOffset+1;
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
			if(content[i+1] == 'p'  || content[i+1] == 'P' || content[i+1] == '.')
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
					return i+j+pageOffset+1;
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
					return i+j+pageOffset+1;
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
		if(content[i]<'A' || content[i]>'Z') continue;
		while(fitPattern('a',content[i]))
		{
			i++;
			if(i > offend-3) return 0;
		}
		if(i-tag>=1 &&(','==content[i]))
		{
			i++;
			while(content[i]==' ') {
				i++;
				if(i > offend - 2) return 0;
			}
			if(content[i]<'A' || content[i]>'Z') continue;
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
		if(content[i]<'A' || content[i]>'Z') continue;
		while(fitPattern('a',content[i]))
		{
			i++;
			if(i > offend-3) return 0;
		}
		if(i-tag>=1 &&(','==content[i] || '.'==content[i] || ' '==content[i]))
		{
			i++;
			while(content[i]==' ') {
				i++;
				if(i > offend - 2) return 0;
			}
			if(content[i]<'A' || content[i]>'Z') continue;
			if(fitPattern('a',content[i])&&fitPattern('e',content[i+1]))
			{
				return i+2;
			}else
			{
				while(!fitPattern('e',content[i]) && content[i] != ' ') i++;
				return i;
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
		if(content[i]<'A' || content[i]>'Z') continue;
		while(fitPattern('a',content[i]))
		{
			i++;
			if(i > offend-3) return 0;
		}
		if(i-tag>=1 &&(fitPattern('e',content[i])|| content[i] == ' '))
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
			}else
			{
				while(!fitPattern('e',content[i]) && content[i] != ' ') i++;
				return i;
			}
		}
	}
	return 0;
}


// XXXX, X.
//if(!parseFile("string.txt")) printf("EO");
//int offset = hasLocationafterTheOffset(0,1000);
//printf("is: %s",getPcontent()+offset);
//adfadfas [Masdfadf, MA] adfasdf
int hasLocationafterTheOffset(int offset,int limit)
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
		if(content[i]<'A' || content[i]>'Z') continue;
		while(fitPattern('a',content[i]))
		{
			i++;
			if(i > offend-3) return 0;
		}
		if(i-tag>=1 &&(fitPattern('e',content[i])|| content[i] == ' '))
		{
			i++;
			while(content[i]==' ') {
				i++;
				if(i > offend - 2) return 0;
			}
			if(content[i]>='A' && content[i]<='Z' && 
				content[i+1]>='A' && content[i+1]<='Z' )
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
		if(content[i]=='['||content[i]=='|' || content[i] == 'r')
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
		}else if(fitPattern('n',content[i]))
		{
			i++;
			while(fitPattern('n',content[i])) i++;
			if(content[i] == '.' || content[i] == ',') return i+1;
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
		if(content[i]=='['||content[i]=='|' || content[i] == 'r')
		{
			for(int j=i+1;j<offend && j < i+6;j++)
			{
				switch(content[j])
				{
					case ']':
					case '|':
					case '!':
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
		}else if(fitPattern('n',content[i]))
		{
			i++;
			while(fitPattern('n',content[i])) i++;
			if(content[i] == '.' || content[i] == ',') return i+1;
		}
	}
	return 0;
}

//int strncmp(const char *s1, const char *s2, size_t n);
#ifndef TESTMATSTR
#define TESTMATSTR(a,b) {if(strncmp(a,b,strlen(a)) == 0 ) return i + strlen(a);}
#endif
int hasSpecialKeyWords(int offset,int limit)
{
	int i;
	int offend;
	char *content = getPcontent();
	defineStartAndEnd(&offset,&offend,limit);
	for(i=offset;i<offend;i++)
	{
		if(content[i-1]==' ')
		{
			TESTMATSTR("ISO",content+i);
			TESTMATSTR("IEEE",content+i);
			TESTMATSTR("ACM",content+i);
			TESTMATSTR("Inc.",content+i);
			TESTMATSTR("Proc.",content+i);
			TESTMATSTR("Ph.D. Thesis",content+i);
			TESTMATSTR("http://",content+i);
			if(content[i]=='V'||content[i]=='v')
			{
				i++;
				TESTMATSTR("ol.",content+i);
			}
			TESTMATSTR("volumn",content+i);
			TESTMATSTR("Volumn",content+i);
				
		}
	}
	return 0;
}

/*
	int pLen;
	int nLen;
	int prev[10];
	int next[10];
 */
int getCloseKWD(int offset,CloseKWD *closeKWD,CKWDfun ckfunc)
{
	int offend = getPclen();
	int nowOffset;
	closeKWD->pLen = 0;
	closeKWD->nLen = 0;
	// calculate before
	int i;
	int x = 0;
	//printf("<");
	for(i=0;i<offset;i++)
	{
		if((nowOffset = ckfunc(i,offset-i)) > 0)
		{
			x ++ ;
			//printf("(%d",nowOffset);
			i = nowOffset ;
			if(closeKWD->pLen)
			{
				//printf("D%d^%d)",(closeKWD->pLen-1)%10,closeKWD->prev[(closeKWD->pLen-1)%10]);
				if((nowOffset - closeKWD->prev[(closeKWD->pLen-1)%10]) > 10)
				{
					closeKWD->prev[(closeKWD->pLen)%10] = nowOffset;
					closeKWD->pLen ++;
				}
			}else
			{
				closeKWD->prev[(closeKWD->pLen)%10] = i;
				//printf("N%d^%d-%d)",i,(closeKWD->pLen)%10,);
				closeKWD->pLen ++;
			}
			
		}else
			break;
	}
	//printf(">");
	//printf("<LL>%d^%d#",closeKWD->pLen,x);
	if(closeKWD->pLen > 10)
	{
		closeKWD->pStart = closeKWD->pLen % 10;
		closeKWD->pLen = 10;
	}else
	{
		closeKWD->pStart = 0;	
	}
	// calculate next
	for(int i=offset;i<offend;i++)
	{
		if((nowOffset = ckfunc(i,offend-i)) > 0)
		{
			i = nowOffset;
			if(closeKWD->nLen)
			{
				if((nowOffset - closeKWD->next[closeKWD->nLen-1]) > 10)
				{
					closeKWD->next[closeKWD->nLen] = nowOffset;
					closeKWD->nLen ++;
				}
			}else
			{
				closeKWD->next[closeKWD->nLen] = i;
				closeKWD->nLen ++;
			}
			
		}else
			break;
		if(closeKWD->nLen >= 10) break;
	}
	
	return 1;
}


double asciiCodeDensity(int offset,int limit)
{
	int i;	
	int offend;
	int num = 0;
	int length;
	char *content = getPcontent();
	defineStartAndEnd(&offset,&offend,limit);
	length = offend - offset;
	for(i=offset;i<offend;i++)
	{
		if(fitPattern('a',content[i])) num++;
	}
	return (double)num/length;
}


double dataDensity(int offset,int limit)
{
	int i;	
	int offend;
	int num = 0;
	int length;
	char *content = getPcontent();
	defineStartAndEnd(&offset,&offend,limit);
	length = offend - offset;
	for(i=offset;i<offend;i++)
	{
		if(fitPattern('d',content[i])) num++;
	}
	return (double)num/length;
}








