#include "hftctl.h"
#include "hftnpse.h"
#include "dirTraversal.h"
#include "persistence.h"
#include "main.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>



int main(int argc,char *argv[])
{
	dbInit();
	
	//int insertFilterData(int accept,const char *cmpStr,int cmpLen);
	//fp = fopen("sample.txt","w");
	//char ch;
	//int i;
	setTrainFile(fopen("train.txt","w")); // train.txt
	setTestFile(fopen("test.txt","w")); // test.txt     
	srand((unsigned int)time(NULL));

	initFilterData();
	insertFilterData(1,"REFERENCES AND BIBLIOGRAPHY",strlen("REFERENCES AND BIBLIOGRAPHY"));
	insertFilterData(1,"REFERENCES",strlen("REFERENCES"));
	insertFilterData(0,"CONFERENCES",strlen("CONFERENCES"));
	insertFilterData(1,"BIBLIOGRAPHY",strlen("BIBLIOGRAPHY"));
	insertFilterData(1,"BIBLIOGRAPHIES",strlen("BIBLIOGRAPHIES"));
	insertFilterData(0,"AUTHOR BIOGRAPHY",strlen("AUTHOR BIOGRAPHY"));
	insertFilterData(0,"AUTHOR BIOGRAPHIES",strlen("AUTHOR BIOGRAPHIES"));
	if(getTrainFile() == NULL || getTestFile() == NULL)
	{
		fprintf(stderr,"error opening sample file\n");
		return -1;
	}
	//for(i=0;i<100;i++)
	dirTraversal("data/",1,genStartSampleCtl);
	//printf("done(%d/%d)\n",bingo,fileNum);
	printf("done \n total : %d\n",getFileNum());
	cleanFilterData();
	fclose(getTrainFile());
	fclose(getTestFile());
	return 0;
}

