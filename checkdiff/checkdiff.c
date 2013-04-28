#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define LINELEN 2048

char *doEscape(char *str)
{
	int len = strlen(str);
	int j=0;
	static char mstr[1024];
	for(int i=0;i<len;i++)
	{
		if(str[i]=='\"')
		{
			mstr[j] = '\\';
			j++;
		}
		mstr[j] = str[i];
		j++;
	}
	mstr[j] = '\0';
	j--;
	while(mstr[j]==' ' || mstr[j]=='\\' || mstr[j]=='\n' || mstr[j]=='\r')
	{
		mstr[j] = '\0';
		j--;
	}
	return mstr;
	
}

int main(int argc,char *argv[])
{
	FILE *fpTestfile = fopen("test.txt","r");
	FILE *fpPredictfile = fopen("svm_predictions","r");
	FILE *fpResultfile = fopen("diff.log","w");
	const char *btu="/home/yu/Workspace/cc/features/GetContext/main ";
	//const char *path="/home/yu/Workspace/cc/features/GetContext/";
	
	char command[1024];
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
		//if(1)
		{
			diff++;
			fprintf(fpResultfile,"%s",paperName+2);
			fprintf(fpResultfile,"%d:%s%s\n",xx,strPredictFileContent,strTestFileContent);
			
			int slen = strlen(strTestFileContent);
			int dOffset = 0;
			for(int z=0;z<slen;z++)
			{
				if(strTestFileContent[z]=='#')
				{
					dOffset = atoi(strTestFileContent+z+1);
					break;
				}
				
			}
			sprintf(command,"%s \"%s\" %d",btu,doEscape(paperName+2),dOffset);
			if(dOffset!=0)
			{
				//printf("run: {%s}\n",command);
				//printf("content: {%s}\n",strTestFileContent);
				//break;
				printf("%s",paperName+2);
				printf("%d:%s%s\n",xx,strPredictFileContent,strTestFileContent);
				system(command);
				break;
			}
				
		}
	}
	printf("diff / total : %d / %d (%f)\n",diff,total,(double)(total - diff)/total);
	fclose(fpTestfile);
	fclose(fpPredictfile);
	fclose(fpResultfile);
	return 0;
}

