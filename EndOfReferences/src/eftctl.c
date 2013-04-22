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

int genEndSampleCtl(const char* fileName,int isDir)
{
	FILE *fp;
	static int id = 0;
	int trainOrTest;
	unsigned int targetOffset;
	unsigned int startOffset;
	featureDataContainer *_mfdc =getEndFeatureDataContainer();
	featureDataContainer tmpCter ;
	memset(_mfdc,0,sizeof(featureDataContainer));
	memset(&tmpCter,0,sizeof(featureDataContainer));
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
		for(int i = 0; i < _mfdc->top;i++)
		{
			if(!insertFeature(fileName,_mfdc->data[i]))
			{
				fprintf(stderr,"[DB] insertFeature()(1): error --%d",__LINE__);
				//getchar();
			}
		}
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
		// 1-20
		for(int j=1;j<LENOFT;j++)
			rankWrite(fp,1+j*5,_mfdc->data[i].t[j],5);
		//	fprintf(fp,"%d:%d ",);
		
		
		// 21 - 45
		for(int j=0;j<LENOFT;j++)
			rankWrite(fp,26+j*5,tmpCter.data[i].t[j],5);
		
		// 46 - end 
		//gen26ToEnd(fp,_mfdc->data[i]);
		genNextDataForEndfeature(fp,_mfdc->data[i],46);
		fprintf(fp,"\n");
	}
	if(!pptag || pptag)
	{
		printf("\n>>EEEE\n");
		char *p = getPcontent();
		for(int i=0;i< _mfdc->top;i++)
		{
			printf("[E:%d]",_mfdc->data[i].offset);
			for(int j = _mfdc->data[i].offset-30; j <  _mfdc->data[i].offset + 30 ; j ++)
			{
				if(j == _mfdc->data[i].offset) putchar('%');
				putchar(*(p+j));
			}
				
			putchar('\n');
		}
		printf("[T:%d]",targetOffset);
		for(int j = targetOffset-30; j < targetOffset + 30 ; j ++)
		{
			if(j ==targetOffset) putchar('%');
			putchar(*(p+j));
		}
			
		putchar('\n');
		printf("\n");
	}
	//step 4: finish handle
	printf(" [done]\n");
	id++;
	
	fflush(NULL);
	cleanContent();
	selfAddFileNum();
	return 1;
}



