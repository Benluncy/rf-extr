#include <stdio.h>
#include <string.h>
//#include <stdlib.h>
#include "minEditDistance.h"
#include "virtualcontent.h"
#include "dirTraversal.h"
int fileNum = 0;

#define MAX_STACK 2000

struct offsetStack
{
	int top;
	int data[MAX_STACK];
}myStack;


#define MIDDLEOF(a,b,c)  (a >= b && a <= c) 
int StackData(const char *content,int len,const char *toCompare,int cLen,int threshold)
{
	int i,j;
	int k;
	int minLen = cLen-threshold;
	int maxLen = cLen+threshold;
	myStack.top = 0;
	int unlock = 1;
	for(i=0;i<len;i++)
	{
		if(( MIDDLEOF(content[i],'A','Z') || MIDDLEOF(content[i],'a','z')
			|| MIDDLEOF(content[i],'0','9') ))
		{
			if(unlock)
			{
				for(j=minLen;j<maxLen;j++)
				{
					//if(editDistanceT(toCompare,cLen,content+i,j,threshold) >= 0)
					if(editDistanceS(toCompare,cLen,content+i,j) < threshold)
					{
						printf("great ! OOF[%d][%d] ",i,editDistanceS(toCompare,cLen,content+i,j));
						for(k=0;k<j;k++)
						{
							putchar(*(content+k+i));
						}
						putchar('\n');
					
						if(myStack.top < MAX_STACK)
						{
							myStack.data[myStack.top] = i;
							myStack.top ++ ;
							i+= j;
							break;
						}else
						{
							return 0;
						}
				
					}
					
				}
				unlock = 0;
			}
		}else
		{
			unlock = 1;
		}
	}
	return 1;
}

unsigned int getReferenceAreaOffset()
{
	int i;
	int mypclen = getPclen();
	//unsigned int mytags = getTags();
	for(i=0;i<mypclen;i++)
	{
		//memset(info,0,sizeof(info));
		//nowTag = mytags[i];
		if((getTags()+i) > 0)
		{
			return *(getOffset()+i);
		
		}
		//putchar(pcontent[i]);
		//printf("now tag: %d\n",nowTag);
		//while((tmp = tokenPop(&nowTag)) > 0 )
		//{
		//	sprintf(info,"%s:%s",info,id2Token(tmp));
		//}
		//fprintf(fp,"[%c:%d:%s]",mypcontent[i],myoffset[i],info);
	}
	return 0;
}

FILE *fp;

int readFile(const char* fileName,int isDir)
{
        if(isDir)
        {
                printf("ignore dir:%s\n",fileName);
                return 1;
        }

        printf("file to parse:%s\n",fileName);
        
 	initContent();
	if(!parseFile(fileName))
	{
		fprintf(stderr,"[[error parsing file : #%s#]]",fileName);
		getchar();
		getchar();
		return 0;
	}
	
	//int i;
	//unsigned int tmp;
	//unsigned int nowTag;
	
	//int myclen = getClen(); //content's length
	//int mypclen =  getPclen(); // pure content's length
	//char *mycontent = getContent();;
	//char *mypcontent = getPcontent(); // pure content
	//unsigned int *myoffset getOffset(); // offset is for pcontent 
	//unsigned int *mytags getTags(); // record the tag
	
	
	int sample;
	int s;
	int x;
	unsigned int refOffset;
	fprintf(fp,"+1 ");

	for(s = 0 ; s < 5 ; s ++)
	{
		StackData(getPcontent(), getPclen(),"REFERENCES",strlen("REFERENCES"),s);
		refOffset = getReferenceAreaOffset();
		x = 1;
		sample = 0;

		while(myStack.top > 0)
		{
			myStack.top --;
			printf("=>%d - %d\n",myStack.data[myStack.top],refOffset);
			if(myStack.data[myStack.top] == refOffset)
			{
				sample = x;
				break;
			}
			x++;
		}
		
		fprintf(fp,"%d:%d ",s,sample);
	}
	fprintf(fp,"\n");
	
	printf("ok :%s len %d: plen : %d\n",fileName,getClen(),getPclen());	
	//printf("\n\n**********************************************************************************\n");

        //printf("\n\n");
        cleanContent();
        fileNum++;
        //getchar();
        //getchar();
        return 1;
}


int main(void)
{
	fp = fopen("sample.txt","w");
        //char ch;
        if(fp == NULL)
        {
        	fprintf(stderr,"error opening .. \n");
        	return -1;
        }
        
        dirTraversal("data/",1,readFile);
        printf("done(%d)\n",fileNum);
        fclose(fp);
        return 0;
}

