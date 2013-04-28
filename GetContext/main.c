#include "hftctl.h"
#include "eftctl.h"
#include "eftfun.h"
#include "hftnpse.h"
#include "dirTraversal.h"
#include "persistence.h"
#include "virtualcontent.h"
#include "debuginfo.h"
#include "main.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>

#include "virtualcontent.h"


int main(int argc,char *argv[])
{
	if(argc != 3)
	{
		printf("\t usage: %s <file_name> <offset>\n",argv[0]);
	}
	char *file_name = argv[1];
	int offset = atoi(argv[2]);
	printf("parse : [%d]@[%s]...\n",offset,file_name);
	initContent();
	if(!parseFile(fileName))
	{
		fprintf(stderr,"[[error parsing file : #%s#]]",fileName);
		return -1;
	}
	printfContext(targetOffset);
	cleanContent();
	return 0;
}

