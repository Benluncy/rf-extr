#include "hftrg.h"
#include "hftctl.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include "minEditDistance.h"
#include "virtualcontent.h"
#include "hftnpse.h"
#include "persistence.h"
#include "tokens.h"

#include "myMath.h"
#include "svm_common.h"
#include "common.h"


//for debug
#include "debuginfo.h"

_FilterData pFilterData;

FILE *fpTrain; // train.txt
FILE *fpTest; // test.txt

featureDataContainer mfdc; // myFeatureDataContainer
featureData mfd; // myFeatureData




int reGetOffsetStartFeaturesW(const char* fileName,WORD ws[ITNUM][FTWIDE],long offlist[ITNUM])
{
        // parse tag or etc ,move data to RAM
 	initContent();
	if(!parseFile(fileName))
	{
		fprintf(stderr,"[[error parsing file : #%s#]]",fileName);
		getchar();
		return 0;
	}
	//for(int z=0;z<)
	for(int i=0;i<mfdc.top;i++)
	{
		for(int k=0;k<CALLBACK_LEN;k++)
		{
			for(int z=0;z<4;z++) mfdc.data[i].seq[k][z] = 1;
		}
		
		for(int j=0;j<mfdc.top;j++)
		{
			if(i==j) continue;
			for(int k=0;k<CALLBACK_LEN;k++)
			{
				//if(mfdc.data[i].density[k][0][1]/NOTZERO(mfdc.data[i].density[k][0][0])
				//	< mfdc.data[j].density[k][0][1]/NOTZERO(mfdc.data[j].density[k][0][0]))
				if(mfdc.data[i].density[k][0][1]*NOTZERO(mfdc.data[j].density[k][0][0])
					<mfdc.data[j].density[k][0][1]*NOTZERO(mfdc.data[i].density[k][0][0]))
					mfdc.data[i].seq[k][0] ++;
				
				//if(mfdc.data[i].density[k][1][1]/NOTZERO(mfdc.data[i].density[k][1][0])
				//	< mfdc.data[j].density[k][1][1]/NOTZERO(mfdc.data[j].density[k][1][0]))
				if(mfdc.data[i].density[k][1][1]*NOTZERO(mfdc.data[j].density[k][1][0])
					<mfdc.data[j].density[k][1][1]*NOTZERO(mfdc.data[i].density[k][1][0]))
					mfdc.data[i].seq[k][1] ++;
				
				//if(mfdc.data[i].density[k][2][1]/NOTZERO(mfdc.data[i].density[k][2][0])
				//	< mfdc.data[j].density[k][2][1]/NOTZERO(mfdc.data[j].density[k][2][0]))
				if(mfdc.data[i].density[k][2][1]*NOTZERO(mfdc.data[j].density[k][2][0])
					<mfdc.data[j].density[k][2][1]*NOTZERO(mfdc.data[i].density[k][2][0]))
					mfdc.data[i].seq[k][2] ++;	
				
				//if(mfdc.data[i].vari[1]/NOTZERO(mfdc.data[i].vari[0])
				//	< mfdc.data[j].vari[1]/NOTZERO(mfdc.data[j].vari[0]))
				if(mfdc.data[i].vari[1]*NOTZERO(mfdc.data[j].vari[0])
					<mfdc.data[j].vari[1]*NOTZERO(mfdc.data[i].vari[0]))
					mfdc.data[i].seq[k][3] ++;
			}
		}
	}
	
	
	
	//printf(".");fflush(NULL);//TODO TIPS
	// write sample
	//fprintf(fp,"#paper %d[%s] ",id,fileName);
	//for(int i=0;i<mfdc.top;i++) fprintf(fp,"[%d:%d]",i,mfdc.data[i].offset);
	//fprintf(fp,"\n");
	int start = 0;
	
	for(int i=0;i<mfdc.top;i++)
	{
		offlist[i] = mfdc.data[i].offset;	
		if(REMOVE_ONLY_FIVE)
		{
			if(mfdc.data[i].t[0]==0&&mfdc.data[i].t[1]==0&&mfdc.data[i].t[2]==0&&mfdc.data[i].t[3]==0)
				continue;
		}
		//fprintf(fp,"%c1 ",mfdc.data[i].positive?'+':'-');
		for(int j=0;j<FEATURE_SIZE;j++)
		{
			rankWriteW(ws[i],start,mfdc.data[i].t[j],5);
			start+=5;
		}
		//	rankWrite(fp,j*5+1,mfdc.data[i].t[j],5);
		//	fprintf(fp,"%d:%d ",j+1,mfdc.data[i].t[j]);
			
		start = gen26ToEndW(ws[i],mfdc.data[i],start);
		
		ws[i][start].wnum = 0; 
		
	}
	
	storeREFO();
	
	//printf("  [done]\n");
        cleanContent();
        selfAddFileNum();
        return mfdc.top;
}



