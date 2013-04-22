#include "hftctl.h"
#include "eftctl.h"
#include "eftfun.h"
#include "hftnpse.h"
#include "dirTraversal.h"
#include "persistence.h"
#include "main.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>

#include "virtualcontent.h"

int genSamples4Endness()
{
	dbInit();
	//initFilterData();
	/*
	//filters (before)
	POFI("APPENDIX");
	POFI("TABLE");
	POFI("ACKNOWLEDGEMENT");
	POFI("AUTHOR BIBLIOGRAPHIES");
	POFI("AUTHOR BIBLIOGRAPHY");
	
	insertFilterData(1,"REFERENCES AND BIBLIOGRAPHY",strlen("REFERENCES AND BIBLIOGRAPHY"));
	insertFilterData(1,"REFERENCES",strlen("REFERENCES"));
	insertFilterData(0,"CONFERENCES",strlen("CONFERENCES"));
	insertFilterData(1,"BIBLIOGRAPHY",strlen("BIBLIOGRAPHY"));
	insertFilterData(1,"BIBLIOGRAPHIES",strlen("BIBLIOGRAPHIES"));
	insertFilterData(0,"AUTHOR BIOGRAPHY",strlen("AUTHOR BIOGRAPHY"));
	insertFilterData(0,"AUTHOR BIOGRAPHIES",strlen("AUTHOR BIOGRAPHIES"));
	*/
	cleanEndKWDContainer();
	
	/**
	 * Before KWD is collected 
	 */
	insertEndKWD("TABLE ");
	insertEndKWD("He is");
	insertEndKWD("Figure");
	insertEndKWD("In this appendix");
	insertEndKWD("NOTICE OF");
	insertEndKWD("He has");
	insertEndKWD("Are there");
	
	openLogFile();
	
	setTrainFile(fopen("train.txt","w")); // train.txt
	setTestFile(fopen("test.txt","w")); // test.txt   

  
	srand((unsigned int)time(NULL));
	if(getTrainFile() == NULL || getTestFile() == NULL)
	{
		fprintf(stderr,"error opening sample file\n");
		return -1;
	}
	
	//dir traversal,and  author to ctl
	dirTraversal("data/",1,genEndSampleCtl);
	
	fclose(getTrainFile());
	fclose(getTestFile());	
	
	
	closeLogFile();
	
	return getFileNum();
}


int main(int argc,char *argv[])
{
	int fileNum = 0;
	//get: end of references
	fileNum = genSamples4Endness();
	printf("done \n total : %d\n",fileNum);
	return 0;
}

