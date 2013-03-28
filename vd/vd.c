#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "vd.h"
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
	if(fp == NULL) return 0;

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

inline int getTokenId(char *tkname,int len)
{
	int i;
	printf("[");
	for(i=0;i<len;i++) putchar(tkname[i]);
	printf("]\n");
	for(i = 0 ; i < tkRepo.max ; i++)
	{
		if(tkRepo.ac[i])
		{
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
	int i;
	printf("in stack : ");
	while((i=tokenPop(&repo)) != 0)
	{
		printf("%d\t",i);
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
	
	nowTag = 0;
	for(i=0;i<flen;i++)
	{
		fcontinue = 0;
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
							printf("POP\t[%d][%d?]\n",tmp2,tmp);

							if(tmp != tmp2)
							{
								//fprintf(stderr,"tmp,tmp2: [%d]:[%d]%s\n",tmp,tmp2,id2Token(tmp));
								fprintf(stderr,"~error: %s(%d) require : %d\n",id2Token(tmp),tmp,tmp2);
								tokenPush(&nowTag,tmp2);
								break;
							}else
							{
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
							printf("PUSH[%d]\n",tmp);
							listStack(nowTag);
							//printf("in : [%d][%s]\n",tmp,id2Token(tmp));
							tokenPush(&nowTag,tmp);
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
	
	pclen = j+1;
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
	#ifndef doClean
	#define doClean(s) {if(s != NULL) { free(s);s=NULL;}}
	doClean(content);
	clen = 0;
	pclen = 0;
	doClean(pcontent);
	doClean(offset);
	doClean(tags);
	#undef doClean
	#endif
	return 1;
}




int main(int argc,char *argv[])
{
	//char *cp; // content;
	//int *tp; // token
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
	
	mycontent = getContent();
	myclen = getClen();
	//#define DOT {printf(" . ");fflush(NULL);}
	#define DOT {;}
	for(i=0;i<myclen;i++)
	{
		fprintf(fp,"[%c:%d]",mycontent[i],i);
		DOT;
	}
	//#define NEXT {printf(">");fflush(NULL);}
	#define NEXT {;}
	fprintf(fp,"\n\n\n\n===================================================\n\n");
	NEXT;
	mypcontent = getPcontent();
	NEXT;
	myoffset = getOffset();
	NEXT;
	mytags = getTags();
	NEXT;
	mypclen = getPclen();
	printf("fine");
	fflush(NULL);
	for(i=0;i<mypclen;i++)
	{
		memset(info,0,sizeof(info));
		nowTag = mytags[i];
		//printf("now tag: %d\n",nowTag);
		while((tmp = tokenPop(&nowTag)) > 0 )
		{
			DOT;
			//printf("%d , %d\n",tmp,nowTag);
			printf(".");
			sprintf(info,"%s:%s",info,id2Token(tmp));
		}
		fprintf(fp,"[%c:%d:%s]",mypcontent[i],myoffset[i],info);
		DOT;
	}	
	
	cleanContent();
	printf("it works\n");
	fclose(fp);
	return 0;
}


