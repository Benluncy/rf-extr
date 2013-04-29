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
	/**
	 * todo list:
	 * data/oraby/A framework for assisted exploration with collaboration.txt
	 *
	 */
	
       	//ONLY("data/orbz_sec/Antialiased ray tracing by adaptive progressive refinement.txt");
	FILE *fp;
	static int id = 0;
	int trainOrTest;
	unsigned int targetOffset;
	unsigned int startOffset;
	endFeatureDataContainer *_mfdc =getEndFeatureDataContainer();
	//endFeatureDataContainer tmpCter ;
	memset(_mfdc,0,sizeof(endFeatureDataContainer));
	//memset(&tmpCter,0,sizeof(endFeatureDataContainer));
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


	// log
	fprintf(flog,"##[[DATA:%s|\n%20s]]\n",fileName,getPcontent()+targetOffset);
		
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
	fprintf(fp,"# %s \n",fileName);
	int pptag = 0;
	int start = 1;
	//printfContextS(targetOffset,"targetOffset");
	
	int lastOffset = _mfdc->data[0].offset;
	for(int i = 0; i < _mfdc->top;i++)
	{
		//if(!haveDiffernecesE(lastOffset,_mfdc->data[i].offset)) continue;
		//lastOffset = _mfdc->data[i].offset;
		
		
		printf(".(%d)",_mfdc->data[i].offset);
		//positive
		
		fprintf(fp,"%c1 ",!haveDiffernecesD(_mfdc->data[i].offset,targetOffset)?'+':'-');
		if(!haveDifferneces(_mfdc->data[i].offset,targetOffset)) pptag ++;
		/*
		if(ABSDIFF(_mfdc->data[i].offset,targetOffset)<100)
		{
			printf("[PP:%c]\n",!haveDiffernecesH(_mfdc->data[i].offset,targetOffset)?'+':'-');
			printfContextS(_mfdc->data[i].offset,"nowoffset");
		}
		*/
		//1-10
		//rankWrite(fp,start,_mfdc->data[i].t[0]+1,10);
		//start += 10;
		//11-55
		start = 1;
		for(int j=0;j<ENDLEN;j++)
		{
		
			switch(j)
			{
			//case 0:
			//	rateWrite(fp,start,_mfdc->data[j].t[0]/_mfdc->top);
			//	start+=5;
			//	break;
			/*
			case 1:
			case 2:
				// 1 acknowledgements etc.
				// 2 table , he is figure ... (a list)
				fprintf(fp,"%d:%d ",start++,_mfdc->data[_mfdc->top].t[j]);
				break;
			*/
			case 3:
			case 4:
			case 5:
				// 3 end of year // before end of content
				// 4 end of page // before end of content
				// 5 end of page2 // before end of content
				// 0 , 1 , 2
				rankWrite(fp,start,_mfdc->data[i].t[j],3);
				start += 3;
				break;
			case 6: // end of article
				// 0 , 1
				fprintf(fp,"%d:%d ",start++,_mfdc->data[_mfdc->top].t[j]);
				break;
			case 9:
			case 12:
			case 15:
				// 9 end year before ack or table
				// 12 end page before ack or table
				rankWrite(fp,start,_mfdc->data[i].t[j],3);
				start += 3;
				break;
			case 21:
			case 24:
			case 27:
				// the min the better
				// ABSDIFF of INDEX
				powerWrite(fp,start,_mfdc->data[i].t[j],8);
				start+=8;
				break;
			}
			
		}
			
		//	fprintf(fp,"%d:%d ",);
		
		/*
		// 91-180 18*5 
		for(int j=0;j<ENDLEN;j++)
			rankWrite(fp,56+j*5,tmpCter.data[i].t[j],5);
		*/
		// 181 - end 
		//gen26ToEnd(fp,_mfdc->data[i]);
		start = genNextDataForEndfeature(fp,_mfdc->data[i],start);
		fprintf(fp," #%d",_mfdc->data[i].offset);
		fprintf(fp,"\n");
	}
	
	
	//DEBUG
	if(!pptag)
	{
		for(int i = 0; i < _mfdc->top;i++)
		{
			if(!haveDiffernecesD(_mfdc->data[i].offset,targetOffset))
			{
				start = 1;
				pptag = -1;
				fprintf(fp,"+1 ");
				for(int j=0;j<ENDLEN;j++)
				{
					switch(j)
					{
					case 0:
						rateWrite(fp,start,_mfdc->data[j].t[0]/_mfdc->top);
						start+=5;
						break;
					case 1:
					case 2:
						// 1 acknowledgements etc.
						// 2 table , he is figure ... (a list)
						fprintf(fp,"%d:%d ",start++,_mfdc->data[_mfdc->top].t[j]);
						break;
					case 3:
					case 4:
					case 5:
						// 3 end of year // before end of content
						// 4 end of page // before end of content
						// 5 end of page2 // before end of content
						// 0 , 1 , 2
						rankWrite(fp,start,_mfdc->data[i].t[j],3);
						start += 3;
						break;
					case 6: // end of article
						// 0 , 1
						fprintf(fp,"%d:%d ",start++,_mfdc->data[_mfdc->top].t[j]);
						break;
					case 9:
					case 12:
					case 15:
						// 9 end year before ack or table
						// 12 end page before ack or table
						rankWrite(fp,start,_mfdc->data[i].t[j],3);
						start += 3;
						break;
					case 21:
					case 24:
					case 27:
						// the min the better
						// ABSDIFF of INDEX
						powerWrite(fp,start,_mfdc->data[i].t[j],8);
						start+=8;
						break;
					}
			
				}
				start = genNextDataForEndfeature(fp,_mfdc->data[i],start);
				fprintf(fp," #%d",_mfdc->data[i].offset);
				fprintf(fp,"\n");
				break;
			}
		}
		/*
		printf("\n DEBUG : contents\n");
		for(int i=0;i< _mfdc->top;i++)
		{
			printf("[E:%d]",_mfdc->data[i].offset);
			printfContext(_mfdc->data[i].offset);
		}
		printf("[T:%d]",targetOffset);
		printfContext(targetOffset);
		*/
	///	
	}
	//step 4: finish handle
	printf("(%d)",pptag);
	printf(" [done]\n");
	id++;
	
	fflush(NULL);
	cleanContent();
	selfAddFileNum();
	return 1;
}



