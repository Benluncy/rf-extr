#include <stdio.h>
#include <string.h>
#include "minEditDistance.h"
#include "dirTraversal.h"
#include "sg.h"

int readFile(const char* fileName,int isDir)
{
	if(isDir)
	{
		printf("ignore dir:%s\n",fileName);
		return 1;
	}
	FILE *fp = fopen(fileName,"r");
	char ch;
	if(fp == NULL) return 0;
	printf("file:%s\n",fileName);
	while((ch = fgetc(fp)) != EOF)
	{
		putchar(ch);
	}
	printf("\n\n");
	return 1;
}


int main(void)
{
	dirTraversal("test",1,readFile);
	printf("done\n");
	return 0;
}
