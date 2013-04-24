#include <stdio.h>
#include "debuginfo.h"
#include "virtualcontent.h"

#define NODEBUG 1

int printfContext(int offset)
{
	if(NODEBUG) return 1;
	int x;
	
	printf("\n>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\n");
	for(x=(offset<=100)?0:(offset-100);x<offset;x++) putchar(*(getPcontent()+x));
	printf("\n================================================================\n");
	for(x=offset;x<offset+100 && x < getPclen();x++) putchar(*(getPcontent()+x));
	printf("\n<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<\n");
	fflush(NULL);
	//getchar(); getchar();
	return 1;
}

int printfContextS(int offset,const char *str)
{
	if(NODEBUG) return 1;
	int status;
	printf("\n$TITIL(%s)@[%d]{\n",str,offset);
	status = printfContext(offset);
	printf("}\n");
	return status;
}


