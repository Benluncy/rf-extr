#include <stdio.h>
#include "vd.h"

const char tokenStr[][10] = {"RefA","SinRef"}; 

int parseFile(const char * fileName)
{
	FILE *fp = fopen(fileName,"r");
	fclose(fp);
}

const char *test_file="data/Database.txt";

int main(int argc,char *argv[])
{
	printf("it works");
	return 0;
}
