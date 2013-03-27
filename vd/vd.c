#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "vd.h"
#include "tokens.h"


const char *test_file="data/Database.txt";

static char *content;
static int clen; //content's length
static int pclen; // pure content's length
static char *pcontent; // pure content
static unsigned int *offset;
static unsigned int *tags;

// unsigned int tt = 0;
// tokenPush(&tt,(unsigned int)i);
// tokenPop(&tt);
int parseFile(const char * fileName)
{
	tokenInit();
	FILE *fp = fopen(fileName,"r");
	if(fp == NULL) return 0;
	
	
	
	fclose(fp);
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

char *getContent(){ return *content;}
unsigned int *getTags(){ return *tags;}


int main(int argc,char *argv[])
{
	//char *cp; // content;
	//int *tp; // token
	parseFile(test_file);
	printf("it works");
	return 0;
}
