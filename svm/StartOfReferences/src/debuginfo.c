#include <stdio.h>
#include "debuginfo.h"
#include "virtualcontent.h"

int printfContext(int refOffset)
{
	int x;
	
	printf("\n>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\n");
	for(x=refOffset-100;x<refOffset;x++) putchar(*(getPcontent()+x));
	printf("\n================================================================\n");
	for(x=refOffset;x<refOffset+100;x++) putchar(*(getPcontent()+x));
	printf("\n<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<\n");
	//getchar(); getchar();
	return 1;
}
