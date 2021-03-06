#include "common.h"
#include "virtualcontent.h"
#include "minEditDistance.h"
#include "hftctl.h"
#include "eftfun.h"
#include <stdio.h>


//TODO MAY USELESS ... 
//int getLastElement(int (*callback)(int offset,int limit));

int haveDifferneces(int dest,int src) // extend
{
	if(src<0) src = 0;
	if(dest<0) dest = 0;
	if(src>=getPclen()) src = getPclen()-1;
	if(dest>=getPclen()) dest = getPclen()-1;

	if(INABSDIFF(dest,src)) return 0;

	int th = 0;
	char *content = getPcontent();

	if(dest < src)
	{
		int tmp = src;
		src = dest;
		dest = tmp;
	}
	for(int i=src;i<dest-1;i++)
	{
		/*
		// no ascii code
		//if(!(content[i]>='a'&&content[i]<='z')||(content[i]>='A'&&content[i]<='Z')) th++;
		if(content[i] != ' ' && content[i] != '\r' && content[i] != '\n' &&fitPattern()) th++;
		
		if(th>2) return 1;
		*/
		
		if(fitPattern('a',content[i])&&fitPattern('a',content[i+1])) 
		{
			//return 1;
			th++;
			if(th > 0) return 1;
			i+=2;
		}
	}
	return 0;
}

int haveDiffernecesD(int dest,int src) // extend
{
	if(src<0) src = 0;
	if(dest<0) dest = 0;
	if(src>=getPclen()) src = getPclen()-1;
	if(dest>=getPclen()) dest = getPclen()-1;

	if(EXINABSDIFF(dest,src)) return 0;

	int th = 0;
	char *content = getPcontent();

	if(dest < src)
	{
		int tmp = src;
		src = dest;
		dest = tmp;
	}
	for(int i=src;i<dest-1;i++)
	{
		/*
		// no ascii code
		//if(!(content[i]>='a'&&content[i]<='z')||(content[i]>='A'&&content[i]<='Z')) th++;
		if(content[i] != ' ' && content[i] != '\r' && content[i] != '\n' &&fitPattern()) th++;
		
		if(th>2) return 1;
		*/
		
		if(fitPattern('a',content[i])&&fitPattern('a',content[i+1])) 
		{
			//return 1;
			th++;
			if(th > 0) return 1;
			i+=2;
		}
	}
	return 0;
}

int haveDiffernecesH(int dest,int src)
{
	if(src>=getPclen()) src = getPclen()-1;
	if(src<0) src = 0;
	if(dest<0) dest = 0;
	
	if(dest>=getPclen()) dest = getPclen()-1;
	char *content = getPcontent();

	if(dest < src)
	{
		int tmp = src;
		src = dest;
		dest = tmp;
	}
	if((dest - src) <= thresholdForDifferneces) return 0;
	for(int i=src;i<dest;i++)
	{
		// no ascii code
		//if(!(content[i]>='a'&&content[i]<='z')||(content[i]>='A'&&content[i]<='Z')) th++;
		if(content[i] != ' ' && content[i] != '\r' && content[i] != '\n' 
			&& content[i] != '\t') return 1;
	}
	return 0;
}

int haveDiffernecesE(int dest,int src) // extend
{
	if(src<0) src = 0;
	if(dest<0) dest = 0;
	if(src>=getPclen()) src = getPclen()-1;
	if(dest>=getPclen()) dest = getPclen()-1;

	if(EXINABSDIFF(dest,src)) return 0;

	int th = 0;
	char *content = getPcontent();

	if(dest < src)
	{
		int tmp = src;
		src = dest;
		dest = tmp;
	}
	for(int i=src;i<dest-1;i++)
	{
		/*
		// no ascii code
		//if(!(content[i]>='a'&&content[i]<='z')||(content[i]>='A'&&content[i]<='Z')) th++;
		if(content[i] != ' ' && content[i] != '\r' && content[i] != '\n' &&fitPattern()) th++;
		
		if(th>2) return 1;
		*/
		
		if(fitPattern('d',content[i])&&fitPattern('d',content[i+1])) 
		{
			//return 1;
			th++;
			if(th > 3) return 1;
			i+=2;
		}
	}
	return 0;
}
/*
int haveDiffernecesH(int dest,int src)
{
	if(src>=getPclen()) src = getPclen()-1;
	if(src<0) src = 0;
	if(dest<0) dest = 0;
	
	if(dest>=getPclen()) dest = getPclen()-1;
	int th = 0;
	char *content = getPcontent();

	if(dest < src)
	{
		int tmp = src;
		src = dest;
		dest = tmp;
	}
	if((dest - src) <= thresholdForDifferneces) return 0;
	for(int i=src;i<dest;i++)
	{
		// no ascii code
		if(content[i] != ' ' && content[i] != '\r' && content[i] != '\n') return 1;
	}
	return 0;
}
*/


int allMarked(int a[],int len)
{
	int i;
	for(i=0;i<len;i++)
	{
		if(a[i]==0) return 0;
	}
	return 1;
}







int powerWriteW(WORD *w,int start,int value,int len)
{
	int i;
	value = value < 0 ? -value : value;
	for(i=0;value>0;i++)
	{
		value /= 2;
	}
	//printf("[pw:%d]",i);
	rankWriteW(w,start,i,len);
	return start+len;
}

int powerWriteNoMoreW(WORD *w,int start,int value,int len)
{
	int i;
	value = value < 0 ? -value : value;
	for(i=0;value>0;i++)
	{
		value /= 2;
	}
	//printf("[pw:%d]",i);
	//printf("write:%d",i);
	rankWriteNoMoreW(w,start,i,len);
	return start+len;
}

int rankWriteNoMoreW(WORD *w,int start,int rank,int len)
{
	
	for(int i=0;i<len;i++)
	{
		//fprintf(fp,"%d:%d ",start+i,rank == i+1 ? 1 : 0);
		w[start+i].wnum = start+i+1;
		w[start+i].weight = (rank == i+1) ? 1 : 0;
	}
	return start+len;
}


// rank == 1,2,3,4,5+
int rankWriteW(WORD *w,int start,int rank,int len)
{
	
	for(int i=0;i<len-1;i++)
	{
		//fprintf(fp,"%d:%d ",start+i,rank == i+1 ? 1 : 0);
		w[start+i].wnum = start+i+1;
		w[start+i].weight = rank == i+1 ? 1 : 0;
	}
	//fprintf(fp,"%d:%d ",start+len-1,rank >= len ? 1 : 0);
	w[start+len-1].wnum = start+len;
	w[start+len-1].weight = rank >= len ? 1 : 0;
	return start+len;
}

int rateWriteW(WORD *w,int start,double rate)
{
	int iRate = rate * 5;
	for(int i=0;i<5;i++)
	{
		//fprintf(fp,"%d:%d ",start+i,iRate == i ? 1 : 0 );
		w[start+i].wnum = start+i+1;
		w[start+i].weight = iRate == i ? 1 : 0;
	}
	return start+5;
}



int setFtValue(WORD *w,int start,int val)
{
	w[start].wnum=start;
	w[start].weight = (double)val;
	return start+1;
}


int setFtValueF(WORD *w,int start,double val)
{
	w[start].wnum=start;
	w[start].weight = val;
	return start+1;	
}


CPINT refo;

void storeREFO()
{
	refo.a = getReferenceHeadOffset();
	refo.b = getReferenceEndOffset();
}


CPINT getREFO()
{
	return refo;
}














