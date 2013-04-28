#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "minEditDistance.h"
#include "virtualcontent.h"
#include "dirTraversal.h"
#include "tokens.h"

int moved = 0;
int total = 0;

int isUnAnnotated()
{
	int i;
	//const char *p = getPcontent();
	int len = getPclen();
	for(i=0;i<len;i++)
	{
		if(*(getTags()+i) == 1) return 0;
	}
	
	return 1;
}

int getPath(char *path,const char *nameToParse,int len)
{
	int i;
	for(i=len-1;i>=0;i--)
	{
		if(nameToParse[i]=='\\' || nameToParse[i] == '/') break;
	}
	strncpy(path,nameToParse,i);
	path[i] = '\0';
	return 1;
}

int getFileName(char *name,const char *nameToParse,int len)
{
	int i;
	for(i=len-1;i>=0;i--)
	{
		if(nameToParse[i]=='\\' || nameToParse[i] == '/') break;
	}
	strncpy(name,nameToParse+i+(nameToParse[0]=='\\'||nameToParse[0]=='/'||i!=0?1:0),len-i);
	name[len-i] = '\0';
	return 1;
}


int doClassify(const char* fileName,int isDir)
{
        if(isDir)
        {
                printf("ignore dir:%s\n",fileName);
                return 1;
        }
	total ++ ;
	
        //printf("handling : %s . . . \n",fileName);
        
 	initContent();
	if(!parseFile(fileName))
	{
		fprintf(stderr,"[[error parsing file : #%s#]]",fileName);
		return 0;
	}
	char name[1024];
	char newPath[1024];
	if(isUnAnnotated())
	{
		getFileName(name,fileName,strlen(fileName));
		sprintf(newPath,"unused/%s",name);
		printf("move file [%s] to [%s]\n",fileName,newPath);
		//getchar();
		rename(fileName,newPath);
		moved ++;
	}
	
        cleanContent();
        return 1;
}


int main(int argc,char *argv[])
{
	//int insertFilterData(int accept,const char *cmpStr,int cmpLen);
	dirTraversal("data/",1,doClassify);
	//printf("done(%d/%d)\n",bingo,fileNum);
	printf("moved : %d total : %d\n",moved,total);
	return 0;
}

