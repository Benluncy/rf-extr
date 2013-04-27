#include "eftctl.h"
#include "eftfun.h"
#include "hftctl.h"
#include "debuginfo.h"
#include "persistence.h"
#include "virtualcontent.h"
#include "tokens.h"
#include "minEditDistance.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

FILE *flog;

void openLogFile()
{
	flog = fopen("afterend.log","w");
}


void closeLogFile()
{
	fclose(flog);
}

#define ONLY(x) if(strncmp(x,fileName,strlen(x))!=0) return 1;

int genEndSampleCtl(const char* fileName,int isDir)
{
	//
       	//ONLY("data/orbz_sec/Another approach to service courses.txt");
	FILE *fp;
	static int id = 0;
	int trainOrTest;
	unsigned int targetOffset;
	unsigned int startOffset;
	endFeatureDataContainer *_mfdc =getEndFeatureDataContainer();
	endFeatureDataContainer tmpCter ;
	memset(_mfdc,0,sizeof(endFeatureDataContainer));
	memset(&tmpCter,0,sizeof(endFeatureDataContainer));
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
	
	targetOffset = getReferenceEndOffset();
 	startOffset = getReferenceHeadOffset();
	//step 1: offsets generate
	//if(1) // generate or get offset from 
	//if(!getFeature(fileName,_mfdc)) // get One To Five from DB
	//TODO read and write offsets from | into db 
	{
		cleanFeature(fileName);	
		// get head of references
		//int basicFilter(featureDataContainer *container,unsigned int startOffset)
		basicFilter(_mfdc,startOffset);
		combineOffsets(_mfdc);
		//!INSERT INTO DB
		/*
		for(int i = 0; i < _mfdc->top;i++)
		{
			
			if(!insertFeature(fileName,_mfdc->data[i]))
			{
				fprintf(stderr,"[DB] insertFeature()(1): error --%d",__LINE__);
				//getchar();
			}
		}*/
		tmpCter = *_mfdc;
		makeSequenceForCombinedOffsets(&tmpCter);
		// filters
		
		// get endness of the file
		
		// last year
		
		// last pp
		
		// what
		
		// combine ... 
	}
	
	//step 2: calculator
	
	prepareDensityData();
	
	// log
	fprintf(flog,"##[[DATA:%s|\n%20s]]\n",fileName,getPcontent()+targetOffset);
	
	
	//step 3: write into file
	fprintf(fp,"# %s \n",fileName);
	int pptag = 0;
	for(int i = 0; i < _mfdc->top;i++)
	{
		printf(".");
		//positive
		fprintf(fp,"%c1 ",!hasDifferneces(_mfdc->data[i].offset,targetOffset)?'+':'-');
		if(!hasDifferneces(_mfdc->data[i].offset,targetOffset)) pptag = 1;
		//tx
		// 1-90 18*5 (in fact 17*5)
		for(int j=1;j<ENDLEN;j++)
			rankWrite(fp,1+j*5,_mfdc->data[i].t[j],5);
		//	fprintf(fp,"%d:%d ",);
		
		
		// 91-180 18*5 
		for(int j=0;j<ENDLEN;j++)
			rankWrite(fp,91+j*5,tmpCter.data[i].t[j],5);
		
		// 181 - end 
		//gen26ToEnd(fp,_mfdc->data[i]);
		genNextDataForEndfeature(fp,_mfdc->data[i],181);
		fprintf(fp,"\n");
	}
	
	//DEBUG
	if(!pptag)
	{
		printf("\n DEBUG : contents\n");
		for(int i=0;i< _mfdc->top;i++)
		{
			printf("[E:%d]",_mfdc->data[i].offset);
			printfContext(_mfdc->data[i].offset);
		}
		printf("[T:%d]",targetOffset);
		printfContext(targetOffset);
		
	}
	//step 4: finish handle
	printf(" [done]\n");
	id++;
	
	fflush(NULL);
	cleanContent();
	selfAddFileNum();
	return 1;
}



