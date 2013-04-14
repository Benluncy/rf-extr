#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define LINELEN 2048

int main(int argc,char *argv[])
{
	FILE *fpTestfile = fopen("test.txt","r");
	FILE *fpPredictfile = fopen("svm_predictions","r");
	FILE *fpResultfile = fopen("diff.log","w");
	if(argc > 1)
	{
		if(fpTestfile != NULL) fclose(fpTestfile);
		fopen(argv[1],"r"); 
	}
	if(fpTestfile==NULL||fpPredictfile==NULL||fpResultfile==NULL)
	{
		fprintf(stderr,"open error");
		return -1;
	}
	char paperName[LINELEN];
	char strTestFileContent[LINELEN];
	char strPredictFileContent[LINELEN];
	memset(paperName,0,LINELEN);
	int xx;
	int diff = 0;
	int total = 0;
	while(fgets(strPredictFileContent,LINELEN,fpPredictfile) != NULL)
	{
		total ++ ;
		do
		{
			fgets(strTestFileContent,LINELEN,fpTestfile);
			xx ++;
			if(strTestFileContent[0] == '#')
			{
				//for(i=0;i<LINELEN;i++) if(strTestFileContent[i] == '(') break;
				//for(j=i+1;j<LINELEN;j++) if(strTestFileContent[j] == ')') break;
				//strncpy(paperName,strTestFileContent+i+1,j-i-1);
				//paperName[j-i-1] = '\0';
				//printf("paperName: [%s]",paperName);
				//getchar();
				strncpy(paperName,strTestFileContent,LINELEN);
				xx = 0;
			}
		}while(strTestFileContent[0] == '#');
		if((strTestFileContent[0]=='-') ^ (strPredictFileContent[0]=='-'))
		{
			diff++;
			fprintf(fpResultfile,"%s",paperName);
			fprintf(fpResultfile,"%d:%s%s\n",xx,strPredictFileContent,strTestFileContent);
		}
	}
	printf("diff / total : %d / %d (%f)\n",diff,total,(double)(total - diff)/total);
	fclose(fpTestfile);
	fclose(fpPredictfile);
	fclose(fpResultfile);
	return 0;
}

