#include <stdio.h>
#include <string.h>

int main(int argc,char *argv[])
{
	FILE *fpTestfile = fopen("test.txt","r");
	FILE *fpPredict = fopen("svm_predictions","r");
	FILE *fpResultfile = fopen("result.log","w");
	char fileName[1024];
	char strFileContent[1024];
	memset(fileName,0,1024);
	
	
	fclose(fpTestfile);
	fclose(fpPredict);
	fclose(fpResultfile);
	return 0;
}
