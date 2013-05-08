#include "eftctl.h"
#include "eftfun.h"
#include "hftctl.h"
#include "debuginfo.h"
#include "persistence.h"
#include "virtualcontent.h"
#include "tokens.h"
#include "minEditDistance.h"
#include "common.h"
#include "hftnpse.h"
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

int errs = 0;

int genEndSampleCtlW(const char* fileName,WORD *w,int limit)
{
	//
	/**
	 * todo list:
	 * data/oraby/A framework for assisted exploration with collaboration.txt
	 *
	 */
	//ONLY("data/orbz_sec/Animating facial expressions.txt");
       	//ONLY("data/orbz_sec/Antecedents and consequences of job satifaction among information center employees.txt");
	static int id = 0;
	//unsigned int targetOffset;
	unsigned int startOffset;
	endFeatureDataContainer *_mfdc =getEndFeatureDataContainer();
	//endFeatureDataContainer tmpCter ;
	memset(_mfdc,0,sizeof(endFeatureDataContainer));
	//memset(&tmpCter,0,sizeof(endFeatureDataContainer));


        // parse tag or etc ,move data to RAM
 	initContent();
	if(!parseFile(fileName))
	{
		fprintf(stderr,"[[error parsing file : #%s#]]",fileName);
		return 0;
	}
	
//	targetOffset = getReferenceEndOffset();
 	startOffset = getReferenceHeadOffset();
	//step 1: offsets generate
	//if(!getFeature(fileName,_mfdc)) // get One To Five from DB
	//TODO read and write offsets from | into db 
	{
		cleanFeature(fileName);	
		// get head of references
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
		
		//tmpCter = *_mfdc;
		//makeSequenceForCombinedOffsets(&tmpCter);
	}


	// log TODO
	//fprintf(flog,"##[[DATA:%s|\n%20s]]\n",fileName,getPcontent()+targetOffset);
		
	// 0 empty for valued -- index of all offsets
	// 1 acknowledgements etc.
	// 2 table , he is figure ... (a list)
	// 3 end of year // before end of content
	// 4 end of page // before end of content
	// 5 end of page2 // before end of content
	// 6 end of article
	// 7 end year before ack
	// 8 end year before table
	// 9 end year before ack or table
	// 10 end page before ack
	// 11 end page before table
	// 12 end page before ack or table
	// 13 end page2 before ack
	// 14 end page2 before table
	// 15 end page2 before ack or table
	// 16 index of year
	// 17 index of page
	// 18 index of page2
	
	//step 3: write into file
	//fprintf(fp,"# %s \n",fileName);
	//int pptag = 0;
	int start = 1;
	for(int i = 0; i < _mfdc->top ;i++)
	{
		printf(".");
		//adjust offset 
		int adja = hasYearafterTheOffset(_mfdc->data[i].offset-4,30);
		int adjb = hasPPafterTheOffset(_mfdc->data[i].offset-4,30);
		int adj = MAX2(adja,adjb);
		//"%c1 ",);
		//printf("[%c1 ]DIFF:%d-%d",!haveDiffernecesD(_mfdc->data[i].offset,targetOffset)?'+':'-'
		//		,_mfdc->data[i].offset,adj);
		//offset+=diff;
		if(adj != 0) _mfdc->data[i].offset = adj;
		//printfContextS(_mfdc->data[i].offset,"CONTEXTS");
		
		
		
		//positive
		//fprintf(fp,"%c1 ",!haveDiffernecesD(_mfdc->data[i].offset,targetOffset)?'+':'-');
		//if(!haveDifferneces(_mfdc->data[i].offset,targetOffset)) pptag ++;
		start = 0;
		for(int j=0;j<ENDLEN;j++)
		{
			//break;//when debuging .. .. ingore these info
			switch(j)
			{
			//*
			// f 0
			case 0:
				//rateWrite(fp,start,(double)_mfdc->data[i].t[0]/_mfdc->top);
				//start+=5;
				//rankWrite(fp,start,_mfdc->top-_mfdc->data[i].t[0]+1,5);
				//rankWriteNoMore(fp,start,_mfdc->top-_mfdc->data[i].t[0]+1,5);
				rankWriteNoMoreW(w,start,_mfdc->top-_mfdc->data[i].t[0]+1,5);
				start+=5;
				break;
			// */
			
			//*
			// f 1
			case 1:
			case 2:
				// 1 acknowledgements etc.
				// 2 table , he is figure ... (a list)
				//fprintf(fp,"%d:%d ",start++,_mfdc->data[i].t[j]);
				//rankWrite(fp,start,_mfdc->data[i].t[j],3);
				rankWriteW(w,start,_mfdc->data[i].t[j],3);
				start += 3;
				break;
			// */
			
			//* 
			// f 2
			case 3:
			case 4:
			case 5:
				//rankWrite(fp,start,_mfdc->data[i].t[j]+1,3);
				rankWriteW(w,start,_mfdc->data[i].t[j]+1,3);
				start += 3;
				break;
			// */
	
			//*
			// f 3
			case 6: // is end of article ?
				// 0 , 1 , 2
				//fprintf(fp,"%d:%d ",start++,);
				start = setFtValue(w,start,chkSpecialFlag()?0:_mfdc->data[i].t[j]*2);
				break;
		
			// f 4
			case 7:
			case 8:
			case 10:
			case 11:
			case 13:
			case 14:
				// 9 end year before ack or table
				// 12 end page before ack or table
				// end year before article
				// 0 , 1 , 2,
				//rankWrite(fp,start,_mfdc->data[i].t[j],5);
				rankWriteW(w,start,_mfdc->data[i].t[j],5);
				start += 5;
				break;	
		
			case 9:
			case 12:
			case 15:
				//rankWrite(fp,start,_mfdc->data[i].t[j-1]+_mfdc->data[i].t[j-2],5);
				rankWriteW(w,start,_mfdc->data[i].t[j-1]+_mfdc->data[i].t[j-2],5);
				start += 5;
				break;

			case 16:
			case 17:
			case 18:
				//powerWriteNoMore(fp,start,_mfdc->data[i].t[j],5);
				powerWriteNoMoreW(w,start,_mfdc->data[i].t[j],5);
				start+=5;
				break;

			case 19:
			case 20:
			case 22:
			case 23:
			case 25:
			case 26:

			case 21:
			case 24:
			case 27:
				powerWriteNoMoreW(w,start,_mfdc->data[i].t[j],8);
				start+=8;
				break;
			}
		
			
		}
			
		setNextElemOffset(i<_mfdc->top-1?_mfdc->data[i+1].offset:0);
		start = genNextDataForEndfeatureW(w,_mfdc->data[i],start);
		
		w[start].wnum=0;
		
		// endness of data
		//fprintf(fp," #%d",_mfdc->data[i].offset);
		//fprintf(fp,"\n");
	}

	//printf(" ok\n");
	id++;
	
	
		
	fflush(NULL);
	cleanContent();
	selfAddFileNum();
	return 1;
}



