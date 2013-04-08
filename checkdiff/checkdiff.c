#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int main(int argc,char *argv[])
{
	FILE *fpTestfile = fopen("test.txt","r");
	FILE *fpPredictfile = fopen("svm_predictions","r");
	FILE *fpResultfile = fopen("result.log","w");
	if(fpTestfile==NULL||fpPredictfile==NULL||fpResultfile==NULL)
	{
		fprintf(stderr,"open error");
		return -1;
	}
	char paperName[1024];
	char strTestFileContent[1024];
	char strPredictFileContent[1024];
	memset(paperName,0,1024);
	int xx;
	int diff = 0;
	int total = 0;
	while(fgets(strPredictFileContent,1024,fpPredictfile) != NULL)
	{
		total ++ ;
		do
		{
			fgets(strTestFileContent,1024,fpTestfile);
			xx ++;
			if(strTestFileContent[0] == '#')
			{
				//for(i=0;i<1024;i++) if(strTestFileContent[i] == '(') break;
				//for(j=i+1;j<1024;j++) if(strTestFileContent[j] == ')') break;
				//strncpy(paperName,strTestFileContent+i+1,j-i-1);
				//paperName[j-i-1] = '\0';
				//printf("paperName: [%s]",paperName);
				//getchar();
				strncpy(paperName,strTestFileContent,1024);
				xx = 0;
			}
		}while(strTestFileContent[0] == '#');
		if((strTestFileContent[0]=='-') ^ (strPredictFileContent[0]=='-'))
		{
			diff++;
			fprintf(fpResultfile,"%s\n",paperName);
			fprintf(fpResultfile,"%d:%s\n\t%s\n",xx,strPredictFileContent,strTestFileContent);
		}
	}
	printf("diff / total : %d / %d (%f)\n",diff,total,(double)(total - diff)/total);
	fclose(fpTestfile);
	fclose(fpPredictfile);
	fclose(fpResultfile);
	return 0;
}

