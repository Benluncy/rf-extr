#include "eftctl.h"
#include "hftctl.h"
#include "debuginfo.h"
#include "persistence.h"
#include "virtualcontent.h"
#include <stdio.h>
#include <stdlib.h>

int genEndSampleCtl(const char* fileName,int isDir)
{
	FILE *fp;
	static int id = 0;
	int trainOrTest;
	
	// ignore dir
        if(isDir)
        {
                printf("ignore dir:%s\n",fileName);
                return 1;
        }
        
        // train or test
	if(rand()%2) //train is 50% and test is 50%
	{
		fp = getTrainFile();
		trainOrTest = 1;
	}else
	{
		fp = getTestFile();
		trainOrTest = 0;
	}
	
	//printf("[%d] %s:%s",id,(trainOrTest?"train":"test"),fileName);
	printf("[%d] %s:%s",id,(trainOrTest?"train":"test"),queryEscape(fileName));
	
	
	fflush(NULL);
        
        // parse tag or etc ,move data to RAM
 	initContent();
	if(!parseFile(fileName))
	{
		fprintf(stderr,"[[error parsing file : #%s#]]",fileName);
		//getchar();
		return 0;
	}
	
	//step 1: offsets generate
	if(1) // generate or get offset from 
	//TODO read and write offsets from | into db 
	{
		// get head of references
		
		// filters
		
		// get endness of the file
		
		// last year
		
		// last 
		

	}
	
	//step 2: calculator
	
	
	
	//step 3: write into file
	fprintf(fp,"\n");
	
	
	
	//step 4: finish handle
	printf(" [done]\n");
	id++;
	selfAddFileNum();
	return 1;
}



