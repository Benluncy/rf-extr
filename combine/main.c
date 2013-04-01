#include <stdio.h>
#include <string.h>
//#include <stdlib.h>
#include "minEditDistance.h"
#include "virtualcontent.h"
#include "dirTraversal.h"
#include "tokens.h"
int fileNum = 0;

#define MAX_STACK 2000

struct offsetStack
{
	int top;
	int data[MAX_STACK];
}myStack;

#define MIDDLEOF(a,b,c)  (a >= b && a <= c)

#define _OFFSET_DIFF(a,b) (a > b? (a - b) :( b - a))

int isData(char ch)
{
	if(MIDDLEOF(ch,'A','Z') || MIDDLEOF(ch,'a','z') || MIDDLEOF(ch,'0','9')	)
		return 1;
	else return 0;
}


int StackData(const char *content,int len,const char *toCompare,int cLen,int threshold)
{
	int i,j;
	//int k;
	int minLen = cLen-threshold;
	int maxLen = cLen+threshold;
	myStack.top = 0;
	int unlock = 1;
	int min;
	int minid;
	int ed;
	for(i=0;i<len;i++)
	{
		if(( MIDDLEOF(content[i],'A','Z') || MIDDLEOF(content[i],'a','z')
			|| MIDDLEOF(content[i],'0','9') ))
		{
			if(unlock)
			{
				min = threshold + 1;
				minid = minLen;
				for(j=minLen;j<maxLen;j++)
				{
					//if(editDistanceT(toCompare,cLen,content+i,j,threshold) >= 0)
					if((ed = editDistanceS(toCompare,cLen,content+i,j)) <= threshold)
					{
						//printf("great ! OOF[%d][%d] ",i,editDistanceS(toCompare,cLen,content+i,j));
						//printf("\n~~[[");
						//for(k=0;k<j;k++)
						//{
						//	putchar(*(content+k+i));
						//}
						//printf("]]\n");
						
						//while(!isData(*(content+j+i))) j++; 
						
						if(min > ed)
						{
								min = ed;
								minid = j;
						}
					

						//else
						//{
						//	return 0;
						//}
				
					}
					
				}
				if(min < threshold + 1)
				{
					i += minid;
					if(myStack.top < MAX_STACK)
					{
						myStack.data[myStack.top] = i;
						myStack.top ++ ;
					
						//i += j;
						//break;
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
	unsigned int myTags;
	unsigned int nowTag;
	//unsigned int mytags = getTags();
	printf("##reference stack:");
	for(i=0;i<mypclen;i++)
	{
		//memset(info,0,sizeof(info));
		//nowTag = mytags[i];
		//if(*(getTags()+i) > 0)
		//{
			//printf("%50s\n",getPcontent()+i);
			//return *(getOffset()+i);
		//	return i;
		//}
		
		//putchar(pcontent[i]);
		//printf("now tag: %d\n",nowTag);
		//while((tmp = tokenPop(&nowTag)) > 0 )
		//{
		//	sprintf(info,"%s:%s",info,id2Token(tmp));
		//}
		//fprintf(fp,"[%c:%d:%s]",mypcontent[i],myoffset[i],info);
		myTags = *(getTags()+i);
		while((nowTag=tokenPop(&myTags)) > 0)
		{
			printf("!%d!",nowTag);
			if(nowTag == 1)
			{
				return i;
			}
		}
		
	}
	printf("end refernces stack:\n");
	return 0;
}

int printfContext(int refOffset)
{
	int x;
	
	printf("\n>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\n");
	for(x=refOffset-100;x<refOffset;x++)
	{
		putchar(*(getPcontent()+x));
	}		
	printf("\n================================================================\n");
	for(x=refOffset;x<refOffset+100;x++)
		putchar(*(getPcontent()+x));

	printf("\n<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<\n");
	//getchar();
	//getchar();
	return 1;
}

FILE *fp;

int bingo = 0;
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
	//int last;
	int s;
	int x;
	unsigned int refOffset;
	

	//last = 0;
	fprintf(fp,"+1 ");
	int tmp = 0;
	for(s = 0 ; s < 5 ; s ++)
	{
		//BIBLIOGRAPHY
		//CONFERENCES
		StackData(getPcontent(), getPclen(),"REFERENCES",strlen("REFERENCES"),s);

		refOffset = getReferenceAreaOffset();
		
		x = 1;
		sample = 0;

		printf("STEP Zero\n");
		printfContext(refOffset);
		
		while(myStack.top > 0)
		{
			printf("STEP %d\n",x);
			myStack.top --;
			printf("CMP:[%d::%d]\n",myStack.data[myStack.top],refOffset);
			
			printfContext(myStack.data[myStack.top]);
			
			printf("!!![[offset: %d]]\n",_OFFSET_DIFF(myStack.data[myStack.top],refOffset));
			
			//if(myStack.data[myStack.top] == refOffset)
			if(_OFFSET_DIFF(myStack.data[myStack.top],refOffset)<10)
			{
				printf("fine!\n&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&\n");
				sample = x;
				//break;
				bingo ++;
				tmp++;
				break;
			}else if(_OFFSET_DIFF(myStack.data[myStack.top],refOffset)<30)
			{
				printf("not fine!\n&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&\n");
				getchar();
				getchar();
			}
			x++;
			//getchar();
		}
		fprintf(fp,"%d:%d ",s,sample);
		printf("\n@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@\n");
		//getchar();
	}
	

	// problems:
	// [data/A decomposition method for the analysis and design of finite state protocols.txt]
	//  wJ~J~mgCES
	printf("[%s]'s value: %d ..\n",fileName,tmp);
	
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
        //int i;
        if(fp == NULL)
        {
        	fprintf(stderr,"error opening .. \n");
        	return -1;
        }
        //for(i=0;i<100;i++)
        dirTraversal("data/",1,readFile);
        printf("done(%d/%d)\n",bingo,fileNum);
        fclose(fp);
        return 0;
}

