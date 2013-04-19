/**
 * The MIT License (MIT)
 *
 *
 * Copyright (C) 2013 Yu Jing (yujing5b5d@gmail.com)
 *
 * Permission is hereby granted, free of charge, to any person obtaining
 * a copy of this software and associated documentation files (the "Software"),
 * to deal in the Software without restriction, including without limitation
 * the rights to use, copy, modify, merge, publish, distribute,sublicense,
 * and/or sell copies of the Software, and to permit persons to whom the
 * Software is furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included
 * in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS
 * OR IMPLIED,INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL
 * THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE,ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 *
 */
 
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "virtualcontent.h"
#include "tokens.h"

#define MAX_TOKEN_LENGTH 20

const char *test_file="data/Database.txt";

static char *content;
static int clen; //content's length

static int pclen; // pure content's length
static char *pcontent; // pure content
static unsigned int *offset; // offset is for pcontent 
static unsigned int *tags; // record the tag

char *getContent(){ return content;}
char *getPcontent(){ return pcontent;}
unsigned int *getOffset(){ return offset; }
unsigned int *getTags(){ return tags; }
int getClen() { return clen;}
int getPclen() { return pclen; }


int readFileToParse(const char * fileName)
{
	unsigned int flen;
	FILE *fp = fopen(fileName,"r");
	if(fp == NULL)
	{
		fprintf(stderr,"open file %s error\n",fileName);
		return 0;
	}
	// get length 
	fseek(fp,0L,SEEK_END); 
	flen=ftell(fp); 
	fseek(fp,0L,SEEK_SET);
	// end : get length 
	
	//init space 
	clen = flen;
	content = (char *)calloc(flen+1,sizeof(char)); // +1 for '\0'
	pcontent = (char *)calloc(flen+1,sizeof(char));
	offset = (unsigned int *)calloc(flen,sizeof(unsigned int));
	tags = (unsigned int *)calloc(flen,sizeof(unsigned int));
	memset(content,0,flen);
	memset(pcontent,0,flen);
	memset(offset,0,flen*sizeof(unsigned int));
	memset(tags,0,flen*sizeof(unsigned int));

	fread(content,flen,1,fp);//read content
	
	fclose(fp);
	return 1;
}

inline int getTokenId(char *tkname,unsigned int len)
{
	int i;
	//printf("[");
	//for(i=0;i<len;i++) putchar(tkname[i]);
	//printf("]\n");
	for(i = 0 ; i < tkRepo.max ; i++)
	{
		if(tkRepo.ac[i])
		{
			if(len != strlen(tkRepo.tokens[i])) continue;
			if(strncmp(tkRepo.tokens[i],tkname,len) == 0)
			{
				return token2Id(tkRepo.tokens[i]);
			}
		}
	}
	return 0;
}

int listStack(unsigned int repo)
{
	return 0;
	int i;
	printf("in stack : ");
	while((i=tokenPop(&repo)) != 0)
	{
		printf("%10s\t",id2Token(i));
	}
	printf("\n");
	return 1;
}

// unsigned int tt = 0;
// tokenPush(&tt,(unsigned int)i);
// tokenPop(&tt);
int parseFile(const char * fileName)
{
	unsigned int flen;
	unsigned int i;
	unsigned int j = 0;
	unsigned int k = 0;
	unsigned int nowTag;
	int tmp;
	int tmp2;
	int fcontinue;
	tokenInit();
	readFileToParse(fileName);
	flen = clen;
	
	//int mark = strcmp("data/A correspondence matching technique of dense checkerboard pattern for one-shot geometry acquisition.txt",fileName) == 0;
	
	/*
	if(mark)
	{
	printf("\n!!be careful!!\n");
	printf("\n=====================================================================================\n");
	getchar();
	}
	*/
	
	nowTag = 0;
	for(i=0;i<flen;i++)
	{
		fcontinue = 0;
		/*
		if(mark)
		{
			putchar(content[i]);
		}
		*/
		//putchar(*(content+i));
		if(content[i] == '<')
		{
			if(content[i+1] == '/')
			{
				for(j=0;j<MAX_TOKEN_LENGTH;j++)
				{
					if(content[i+j+2] == '>')
					{
						tmp = getTokenId(content+i+2,j);
						//for(k=0;k<j;k++) putchar(content[i+2+k]);
						//putchar('\t');
						if(tmp != 0)
						{
							listStack(nowTag);
							//printf("out : [%d][%s]\n",tmp,id2Token(tmp));
							tmp2 = tokenPop(&nowTag);
							//printf("POP\t[%d][%d?]\n",tmp2,tmp);

							if(tmp != tmp2)
							{
								//fprintf(stderr,"tmp,tmp2: [%d]:[%d]%s\n",tmp,tmp2,id2Token(tmp));
								fprintf(stderr,"~error: %s(%d) require : %d\n",id2Token(tmp),tmp,tmp2);
								putchar('\n');
								printf(". . . ");
								for(j=0;j<30;j++)
									putchar(content[i+j]);
								printf(". . . ");
								putchar('\n');
								tokenPush(&nowTag,tmp2);
								return 0;
								//break;
							}else
							{
								/*
								if(mark)
								{
									printf("\n\n\nout:%s\n\n",id2Token(tmp));
									getchar();
								}
								*/
								i+=j+2;
								//continue;
								fcontinue = 1;
								break;
							}
						}
					}
				}
				if(fcontinue) continue;
			}else
			{
				for(j=0;j<MAX_TOKEN_LENGTH;j++)
				{
					if(content[i+j+1] == '>')
					{		
						tmp = getTokenId(content+i+1,j);
						//for(k=0;k<j;k++) putchar(content[i+1+k]);
						//putchar('\t');
						if(tmp != 0)
						{
							/*
							int zz;
							if(mark)
							{
							printf("\n\nCDATA[[");
							for(zz=i;zz<i+j+1;zz++) putchar(content[zz]);
							printf("]]\n\n\nin:%s\n\n",id2Token(tmp));
							
							}
							*/
							//printf("PUSH[%d]\n",tmp);
							//printf("in : [%d][%s]\n",tmp,id2Token(tmp));
							tokenPush(&nowTag,tmp);
							listStack(nowTag);
							i+=j+1;
							//continue;
							fcontinue = 1;
							break;
						}
					}
				}
				if(fcontinue) continue;
			}
			
		}
		pcontent[k] = content[i];
		offset[k] = i;
		tags[k] = nowTag;
		//putchar(pcontent[k]);
		k++;
	}
	/*
	if(mark)
	{
	printf("\n=====================================================================================\n");
	printf("\n=====================================================================================\n");
	getchar();getchar();getchar();
	getchar();
	}
	*/
	pclen = k+1;
	
	return 1;
}

int initContent()
{
	content = NULL;
	pcontent = NULL;
	offset = NULL;
	tags = NULL;
	clen = 0;
	pclen = 0;
	return 1;
}

int cleanContent()
{
	#define doClean(s) {if(s != NULL) { free(s);s=NULL;}}
	doClean(content);
	clen = 0;
	pclen = 0;
	doClean(pcontent);
	doClean(offset);
	doClean(tags);
	return 1;
}


/*

int main(int argc,char *argv[])
{
	initContent();
	parseFile(test_file);
	FILE *fp = fopen("output.txt","w");
	int i;
	char info[1024];
	unsigned int tmp;
	char *mycontent;
	int myclen; //content's length
	int mypclen; // pure content's length
	char *mypcontent; // pure content
	unsigned int *myoffset; // offset is for pcontent 
	unsigned int *mytags; // record the tag
	unsigned int nowTag;
	
	unsigned int z = 0;
	
	// environment
	mycontent = getContent();
	myclen = getClen();
	mypcontent = getPcontent();
	myoffset = getOffset();
	mytags = getTags();
	mypclen = getPclen();
	
	//
	for(i=0;i<myclen;i++)
	{
		fprintf(fp,"%c",mycontent[i],i);
		
	}
	fprintf(fp,"\n\n\n\n===================================================\n\n");

	printf("fine");
	fflush(NULL);
	//int z;
	unsigned int z2;
	int utg = 1;
	for(i=0;i<mypclen;i++)
	{
		//memset(info,0,sizeof(info));
		nowTag = mytags[i];
		if(nowTag > 0 && utg)
		{
			z = myoffset[i];
			z2 = i;
			utg = 0;
		}
		putchar(pcontent[i]);
		//printf("now tag: %d\n",nowTag);
		//while((tmp = tokenPop(&nowTag)) > 0 )
		//{
		//	sprintf(info,"%s:%s",info,id2Token(tmp));
		//}
		//fprintf(fp,"[%c:%d:%s]",mypcontent[i],myoffset[i],info);
	}	
	
	printf("\n~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n");
	fflush(NULL);

	printf("~%d:~%d",z,z2);
	for(i=z;i<z+100;i++) putchar(mycontent[i]);
	fflush(NULL);
	printf("\n~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n");
	for(i=z2;i<z2+100;i++) putchar(mypcontent[i]);
	
	cleanContent();
	printf("it works\n");
	fclose(fp);
	return 0;
}

*/
