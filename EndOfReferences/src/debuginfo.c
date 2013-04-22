#include <stdio.h>
#include "debuginfo.h"
#include "virtualcontent.h"

int printfContext(int offset)
{
	int x;
	
	printf("\n>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\n");
	for(x=offset-100;x<offset;x++) putchar(*(getPcontent()+x));
	printf("\n================================================================\n");
	for(x=offset;x<offset+100;x++) putchar(*(getPcontent()+x));
	printf("\n<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<\n");
	//getchar(); getchar();
	return 1;
}

int printfContextS(int offset,const char *str)
{
	int status;
	printf("$TITIL(%s){\n",str);
	status = printfContext(offset);
	printf("}\n");
	return status;
}


