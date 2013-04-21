#include "eftfun.h"
#include "virtualcontent.h"
#include "persistence.h"
#include "tokens.h"
#include "minEditDistance.h"
#include "myMath.h"
#include "hftnpse.h"
#include "hftctl.h"
#include <stdio.h>
#include <string.h>

#define thresholdForDifferneces  10
#define thresholdForGetOffsetSuggestion(x)  x*0.3
#define T4GOS(x) thresholdForGetOffsetSuggestion(x)
#ifndef INLMT
#define INLMT(x) (editDistanceT(x,strlen(x),content+i,strlen(x)>strlen(content+i)?strlen(content+i):strlen(x),T4GOS(strlen(x))) != -1)
#endif
//#define INLMT(x) (editDistanceS(x,strlen(x),content+i,strlen(x)) <= T4GOS(strlen(x)))
featureData mfd;
featureDataContainer mfdc;


OffsetCallback endFunctionList[CALLBACK_LEN]={hasPPafterTheOffset,hasPPafterTheOffset2,
			hasYearafterTheOffset,hasNameafterTheOffset0,
			hasNameafterTheOffset1,//hasNameafterTheOffset2,
			hasSeqOfTheOffset,hasSeqOfTheOffset2,
			hasSpecialKeyWords,hasLocationafterTheOffset};

struct endKWD
{
	unsigned int len;
	char key[200];
};

typedef struct theEKContainer
{
	int top;
	struct endKWD data[200];
}ekContainer;

ekContainer myEc;

int cleanEndKWDContainer()
{
	myEc.top = 0;
	return 1;
}

//"TABLE","He is","Figure","In this appendix","NOTICE OF","He has","Are there"
int insertEndKWD(const char *key)
{
	sprintf(myEc.data[myEc.top].key,"%s",key);
	myEc.data[myEc.top].len = strlen(key);
	myEc.top++;
	return 1;
}



int getLastYearOffset(unsigned int startOffset)
{
	//hasYearafterTheOffset(int offset,int limit)
	int len= getPclen();
	int offset = 0;
	while((startOffset = hasYearafterTheOffset(startOffset,len)) != 0)
	{
		offset = startOffset;
	}
	return offset;
}

int getLastPageOffset(unsigned int startOffset)
{
	//hasYearafterTheOffset(int offset,int limit)
	int len= getPclen();
	int offset = 0;
	char *content = getPcontent();
	while((startOffset = hasPPafterTheOffset(startOffset,len)) != 0)
	{
		offset = startOffset;
		
		for(int i=offset-10;i<offset+10  && i < getPclen();i++)
		{
			if(i == offset) putchar('%');
			putchar(content[i]);
		}
		putchar('\n');
	}
	
	printf("PP:[");
	for(int i=offset-10;i<offset+10 && i < getPclen();i++)
	{
		if(i == offset) putchar('%');
		putchar(content[i]);
	}
	printf("]$$\n");
	return offset;
}

int getLastPage2Offset(unsigned int startOffset)
{
	//hasYearafterTheOffset(int offset,int limit)
	int len= getPclen();
	int offset = 0;
	while((startOffset = hasPPafterTheOffset2(startOffset,len)) != 0)
	{
		offset = startOffset;
	}
	return offset;
}



int basicFilter(featureDataContainer *container,unsigned int startOffset)
{
	char *content = getPcontent();
	int cLen = getPclen();	
	int hasContent = 0;
	int lastYearOffset = getLastYearOffset(startOffset);
	int isMarkedYear = 0;
	int lastPageOffset = getLastPageOffset(startOffset);
	int isMarkedPage = 0;
	int lastPageOffset2 = getLastPage2Offset(startOffset);
	int isMarkedPage2 = 0;
	//printf("last year offset is : %d",lastYearOffset);
	//printf("%c -%d",content[0],cLen);
	//POFI("APPENDIX");
	//POFI("TABLE");
	//POFI("ACKNOWLEDGEMENT");
	//POFI("AUTHOR BIBLIOGRAPHIES");
	//POFI("AUTHOR BIBLIOGRAPHY");
	container->top = 0;
	
	//char kwdList[][30]={"TABLE","He is","Figure","In this appendix","NOTICE OF","He has","Are there"};
	for(int i=startOffset;i<cLen;i++)
	{
		if(i!=0) if(fitPattern('d',content[i-1])) continue;
		hasContent = 0;
		//1. leave to datas
		
		//2. 
		//APPENDIX || ACKNOWLEDGEMENT
		//AUTHOR BIBLIOGRAPHIES || AUTHOR BIBLIOGRAPHY
		//if(editDistanceS("APPENDIX",strlen("APPENDIX"),content+i,strlen("APPENDIX")) <= T4GOS ||
		//	editDistanceS("ACKNOWLEDGEMENT",strlen("ACKNOWLEDGEMENT"),content+i,strlen("ACKNOWLEDGEMENT")) <= T4GOS ||
		//	editDistanceS("AUTHOR BIBLIOGRAPHIES",strlen("AUTHOR BIBLIOGRAPHIES"),content+i,strlen("AUTHOR BIBLIOGRAPHIES")) <= T4GOS ||
		//	editDistanceS("AUTHOR BIBLIOGRAPHY",strlen("AUTHOR BIBLIOGRAPHY"),content+i,strlen("AUTHOR BIBLIOGRAPHY")) <= T4GOS)
		if(INLMT("APPENDIX")||INLMT("ACKNOWLEDGEMENT")
			||INLMT("AUTHOR BIBLIOGRAPHIES") ||INLMT("AUTHOR BIBLIOGRAPHY") //AUTHOR BIOGRAPHIES 
			|| INLMT("AUTHOR BIOGRAPHIES") || INLMT("AUTHOR BIOGRAPHY")) //
		{
			container->data[container->top].t[1] = 1;
			hasContent = 1;
		}
		//3. "TABLE" "He is" "Figure " ,"In this appendix" , "NOTICE OF","He has","Are there " etc.
		for(int x = 0;x < myEc.top;x++)
		{
			/*
			if(editDistanceS(myEc.data[x].key,myEc.data[x].len,
				content+i,myEc.data[x].len) <= myEc.data[x].len*0.3)
			{
				printf("[+]");
				container->data[container->top].t[2] = 1;
				hasContent = 1;
			}*/
			if(INLMT(myEc.data[x].key))
			{
				printf("[+]");
				container->data[container->top].t[2] = 1;
				hasContent = 1;
			}
		}
		
		
	
		
		//4. end of year 
		if(!hasDifferneces(lastYearOffset,i) && !isMarkedYear)
		{
			container->data[container->top].t[3] = 1;
			isMarkedYear = 1;
			hasContent = 1;
		}
		
		//4. end of  pp
		if(!hasDifferneces(lastPageOffset,i) && !isMarkedPage)
		{
			container->data[container->top].t[3] = 1;
			isMarkedYear = 1;
			hasContent = 1;
		}
		
		//4. end of  pp2
		if(!hasDifferneces(lastPageOffset2,i) && !isMarkedPage2)
		{
			container->data[container->top].t[3] = 1;
			isMarkedYear = 1;
			hasContent = 1;
		}
		
		//5 end of article
		if(!hasDifferneces(cLen,i))
		{
			container->data[container->top].t[4] = 1;
			container->data[container->top].offset = i;
			container->top++;

			/*					
			printf("\n\n#:{}\n");
			for(int i=0;i<container->top;i++)
			{
				printf(">>");
				for(int j=0;j<LENOFT;j++)
				{
					printf("%d ",container->data[i].t[j]);
				}
				printf("%d \n",container->data[i].offset);
			}*/
			return 1;
		}
		if(hasContent)
		{
			container->data[container->top].offset = i;
			container->top++;
		}
	}
	return 0;
}


int combineOffsets(featureDataContainer *container)//combine nearly offsets and make sure 
{
	int j = 0;
	int lastOffset = container->data[0].offset;
	//int reo = getReferenceEndOffset();
	//container->data[0].positive = !hasDifferneces(lastOffset,reo);
	for(int i=0;i<container->top;i++)
	{
		container->data[j].offset = container->data[i].offset;
		//hasDifferneces(int dest,int src)
		if(!hasDifferneces(lastOffset,container->data[i].offset))
		{
			//container->data[j].offset = container->data[i].offset;
			//container->data[j].positive = !hasDifferneces(container->data[i].offset,reo)|| container->data[j].positive;
			for(int k=0;k<LENOFT;k++)
			{
				container->data[j].t[k] = container->data[i].t[k] || container->data[j].t[k];
			}
		}else
		{
			j++;
			//container->data[j].offset = container->data[i].offset;
			//container->data[j].positive = !hasDifferneces(container->data[i].offset,reo);
			for(int k=0;k<LENOFT;k++)
			{
				container->data[j].t[k] = container->data[i].t[k];
			}
		}
		lastOffset = container->data[i].offset;
	}
	container->top = j+1;
	/*
	for(int i=0;i<container->top;i++)
	{
		for(int j=0;j<LENOFT;j++)
		{
			printf("%d ",container->data[i].t[j]);
		}
		printf("\n");
	}*/
	return 1;
}

int makeSequenceForCombinedOffsets(featureDataContainer *container)
{
	unsigned int max[LENOFT];
	//unsigned int before[LENOFT];
	unsigned int hasData[LENOFT];
	int top = container->top;
	for(int i=0;i<LENOFT;i++)
	{
		max[i] = 1;
		hasData[i] = 0;
	}
	for(int i=0;i<top;i++)
		for(int j=1;j<LENOFT;j++) 
			if(container->data[i].t[j] != 0 ) hasData[j] = 1;
	
	for(int i=0;i<top;i++)
	{
		for(int j=1;j<LENOFT;j++)
		{
			if(hasData[j])
			{
				if(container->data[i].t[j] == 0)
				{
					container->data[i].t[j] = max[j];
				}else
				{
					max[j]++;
					container->data[i].t[j] = max[j];
				}
			
			}
		}
		container->data[i].t[0] = i+1;
	}
	/*
	printf("X:\n");
	for(int i=0;i<top;i++)
	{
		for(int j=0;j<LENOFT;j++)
		{
			printf("%3d ",container->data[i].t[j]);
		}
		printf("\n");
	}*/
	return 1;
}


int genNextDataForEndfeature(FILE *fp,featureData fd,int start)
{
	int offset = fd.offset;
	int lmt = 200;
	
	char *content = getPcontent();
	int offend = getPclen();	
	int finac = 1;// :1 is not ascii code  :0 is ascii code
	// follow is not ascii code for instance : References [ right References Manual false
	for(int i=offset;i<offend;i++ )
	{
		if(content[i] != ' ')
		{
			finac = fitPattern('a',content[i]) && fitPattern('a',content[i+1]) ? 0 : 1;
			if(!finac && fitPattern('n',content[i]))
			{
				finac = 1;
			}
			break;
		}
	}
	fprintf(fp,"%d:%d ",start++,finac);
	
	//fp = stdout;
	//for(;lmt<=1000;lmt+=150)
	//{
	lmt = -200;
	
	
	fprintf(fp,"%d:%d ",start++,(hasPPafterTheOffset(offset,lmt)?1:0));
	fprintf(fp,"%d:%d ",start++,(hasPPafterTheOffset2(offset,lmt)?1:0));
	fprintf(fp,"%d:%d ",start++,(hasYearafterTheOffset(offset,lmt)?1:0));
	fprintf(fp,"%d:%d ",start++,(hasNameafterTheOffset0(offset,lmt)?1:0));
	fprintf(fp,"%d:%d ",start++,(hasNameafterTheOffset1(offset,lmt)?1:0));
	fprintf(fp,"%d:%d ",start++,(hasNameafterTheOffset2(offset,lmt)?1:0));
	
	fprintf(fp,"%d:%d ",start++,(hasPPafterTheOffset(offset,lmt) >= hasPPafterTheOffset(offset,-lmt)));
	fprintf(fp,"%d:%d ",start++,(hasPPafterTheOffset2(offset,lmt) >= hasPPafterTheOffset2(offset,-lmt)));
	fprintf(fp,"%d:%d ",start++,(hasYearafterTheOffset(offset,lmt) >= hasYearafterTheOffset(offset,-lmt)));
	fprintf(fp,"%d:%d ",start++,(hasNameafterTheOffset0(offset,lmt) >= hasNameafterTheOffset0(offset,-lmt)));
	fprintf(fp,"%d:%d ",start++,(hasNameafterTheOffset1(offset,lmt) >= hasNameafterTheOffset1(offset,-lmt)));
	fprintf(fp,"%d:%d ",start++,(hasNameafterTheOffset2(offset,lmt) >= hasNameafterTheOffset2(offset,-lmt)));

	fprintf(fp,"%d:%d ",start++,asciiCodeDensity(offset,lmt) >= asciiCodeDensity(offset,-lmt));
	fprintf(fp,"%d:%d ",start++,dataDensity(offset,lmt) >= dataDensity(offset,-lmt));

	lmt = 200;
	fprintf(fp,"%d:%d ",start++,(hasPPafterTheOffset(offset,lmt)?1:0));
	fprintf(fp,"%d:%d ",start++,(hasPPafterTheOffset2(offset,lmt)?1:0));
	fprintf(fp,"%d:%d ",start++,(hasYearafterTheOffset(offset,lmt)?1:0));
	fprintf(fp,"%d:%d ",start++,(hasNameafterTheOffset0(offset,lmt)?1:0));
	fprintf(fp,"%d:%d ",start++,(hasNameafterTheOffset1(offset,lmt)?1:0));
	fprintf(fp,"%d:%d ",start++,(hasNameafterTheOffset2(offset,lmt)?1:0));

	lmt = -1000;
	
	fprintf(fp,"%d:%d ",start++,(hasSeqOfTheOffset(offset,lmt)?1:0));
	fprintf(fp,"%d:%d ",start++,(hasSeqOfTheOffset2(offset,lmt)?1:0));	
	fprintf(fp,"%d:%d ",start++,(hasSeqOfTheOffset(offset,lmt) >= hasSeqOfTheOffset(offset,-lmt)));
	fprintf(fp,"%d:%d ",start++,(hasSeqOfTheOffset2(offset,lmt) >= hasSeqOfTheOffset2(offset,-lmt)));
	
	fprintf(fp,"%d:%d ",start++,asciiCodeDensity(offset,lmt) >= asciiCodeDensity(offset,-lmt));
	fprintf(fp,"%d:%d ",start++,dataDensity(offset,lmt) >= dataDensity(offset,-lmt));
	
	//}
	
	//32
	//fprintf(fp,"%d:%f ",start++,(double)offset/getPclen());
	//fprintf(fp,"%d:%f ",start++,(double)(getPclen()-offset)/offset);
	//rateWrite(fp,start,(double)offset/getPclen());
	//start+=5;
	
	for(int i=0;i<CALLBACK_LEN;i++)
	{
		//rateWrite(fp,start,(fd.offset == getPclen()) ? -1 :((double)fd.fid[i][0]/fd.offset)/
		//			((fd.fid[i][1]-fd.fid[i][0])/(getPclen()-fd.offset)));
		//start+=5; //TODO ADD 
		
		fprintf(fp,"%d:%d ",start++,fd.density[i][0][1] <= fd.density[i][0][0]);
		fprintf(fp,"%d:%d ",start++,fd.density[i][1][1] <= fd.density[i][1][0]);
		fprintf(fp,"%d:%d ",start++,fd.density[i][2][1] <= fd.density[i][2][0]);
		fprintf(fp,"%d:%d ",start++,fd.vari[1] <= fd.vari[0]);
		
		//fprintf(fp,"%d:%f ",start++,quot(fd.density[i][0][1],fd.density[i][0][0]));
		//fprintf(fp,"%d:%f ",start++,quot(fd.density[i][1][1],fd.density[i][1][0]));
		//fprintf(fp,"%d:%f ",start++,quot(fd.density[i][2][1],fd.density[i][2][0]));
		//fprintf(fp,"%d:%f ",start++,quot(fd.vari[1],fd.vari[0]));
		
		int wide = 6;
		rankWrite(fp,start,fd.seq[i][0],wide);
		start+=wide;

		rankWrite(fp,start,fd.seq[i][1],wide);
		start+=wide;
		
		rankWrite(fp,start,fd.seq[i][2],wide);
		start+=wide;
		
		rankWrite(fp,start,fd.seq[i][3],wide);
		start+=wide;

	}

	return start;
}




int prepareDensityData(void)
{
	int beforeData;
	int totalData;
	for(int i=0;i<mfdc.top;i++)
	{
		
		for(int j=0;j<CALLBACK_LEN;j++)
		{
			offsetStat(mfdc.data[i].offset,
					&totalData,&beforeData,endFunctionList[j]);
			mfdc.data[i].fid[j][0] = beforeData;
			mfdc.data[i].fid[j][1] = totalData;
		}
	}
	
	//density and etc
	
	printf(".");fflush(NULL);//TODO TIPS
	
	// adjacencyOffset setting ... 
	for(int i=0;i<mfdc.top;i++)
	{
		mfdc.data[i].adjacencyOffset[0] = 0;
		mfdc.data[i].adjacencyOffset[1] = getPclen();
		for(int j=0;j<mfdc.top;j++)
		{
			if(i == j) continue;
			if(mfdc.data[i].offset < mfdc.data[i].offset && mfdc.data[i].adjacencyOffset[0] < mfdc.data[i].offset)
				mfdc.data[i].adjacencyOffset[0] = mfdc.data[i].offset ;
			else if(mfdc.data[i].offset > mfdc.data[i].offset && mfdc.data[i].adjacencyOffset[1] > mfdc.data[i].offset)
				mfdc.data[i].adjacencyOffset[1] = mfdc.data[i].offset ;
		}
	}
	
	
	printf(".");fflush(NULL);//TODO TIPS
	
	int diff;
	int lmt;
	//density && variance setting ... 
	CloseKWD ckwd;
	for(int i=0;i<mfdc.top;i++)
	{
		for(int j=0;j<CALLBACK_LEN;j++)
		{
			mfdc.data[i].density[j][0][0] = (double)mfdc.data[i].fid[j][0]/NOTZERO(mfdc.data[i].offset);
			mfdc.data[i].density[j][0][1] = (double)(mfdc.data[i].fid[j][1]-mfdc.data[i].fid[j][0])/NOTZERO(getPclen()-mfdc.data[i].fid[j][0]);
			
			offsetBetweenStat(mfdc.data[i].offset,mfdc.data[i].adjacencyOffset[0],&diff,endFunctionList[j]);
			//mfdc.data[i].density[j][1][0] = (double)(mfdc.data[i].offset-mfdc.data[i].adjacencyOffset[0])/NOTZERO(diff);
			mfdc.data[i].density[j][1][0] = (double)diff/NOTZERO(VALUESDIFF(mfdc.data[i].offset,mfdc.data[i].adjacencyOffset[0]));
			
			offsetBetweenStat(mfdc.data[i].offset,mfdc.data[i].adjacencyOffset[1],&diff,endFunctionList[j]);
			mfdc.data[i].density[j][1][1] = (double)diff/NOTZERO(VALUESDIFF(mfdc.data[i].adjacencyOffset[1],mfdc.data[i].offset));
			
			
			lmt = 300;
			// prev && next 300* words
			offsetBetweenStat(mfdc.data[i].offset,LMB(mfdc.data[i].offset-lmt),&diff,endFunctionList[j]);
			mfdc.data[i].density[j][2][0] = (double)diff/NOTZERO(VALUESDIFF(mfdc.data[i].offset,LMB(mfdc.data[i].offset-lmt)));
			
			offsetBetweenStat(mfdc.data[i].offset,LMB(mfdc.data[i].offset+lmt),&diff,endFunctionList[j]);
			mfdc.data[i].density[j][1][1] = (double)diff/NOTZERO(VALUESDIFF(LMB(mfdc.data[i].offset+lmt),mfdc.data[i].offset));
			
			// variance 
			//int getCloseKWD(int offset,CloseKWD *closeKWD,int (*callback)(int offset,int limit));
			getCloseKWD(mfdc.data[i].offset,&ckwd,endFunctionList[j]);
			mfdc.data[i].vari[0] = getVariance(ckwd.prev,ckwd.pLen);
			mfdc.data[i].vari[1] = getVariance(ckwd.next,ckwd.nLen);
		}
	}
	
	
	// sequence setting ... 
	printf(".");fflush(NULL);//TODO TIPS
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
				if(mfdc.data[i].density[k][0][1]/NOTZERO(mfdc.data[i].density[k][0][0])
					< mfdc.data[j].density[k][0][1]/NOTZERO(mfdc.data[j].density[k][0][0]))
					mfdc.data[i].seq[k][0] ++;
				
				if(mfdc.data[i].density[k][1][1]/NOTZERO(mfdc.data[i].density[k][1][0])
					< mfdc.data[j].density[k][1][1]/NOTZERO(mfdc.data[j].density[k][1][0]))
					mfdc.data[i].seq[k][1] ++;
				
				if(mfdc.data[i].density[k][2][1]/NOTZERO(mfdc.data[i].density[k][2][0])
					< mfdc.data[j].density[k][2][1]/NOTZERO(mfdc.data[j].density[k][2][0]))
					mfdc.data[i].seq[k][2] ++;	
				
				if(mfdc.data[i].vari[1]/NOTZERO(mfdc.data[i].vari[0])
					< mfdc.data[j].vari[1]/NOTZERO(mfdc.data[j].vari[0]))
					mfdc.data[i].seq[k][3] ++;
			}
		}
	}
	
	return 1;
}




unsigned int getReferenceEndOffset()
{
	int i;
	int mypclen = getPclen();
	int in = 0;
	unsigned int myTags;
	unsigned int nowTag;
	if(mypclen == 0)
	{
		printf("error getting reference end of offset\n");
		getchar();
	}
	for(i=0;i<mypclen;i++)
	{
		myTags = *(getTags()+i);
		if(in && !myTags) return i;
		while((nowTag=tokenPop(&myTags)) > 0)
			if(nowTag == 1)
			{
				in = 1;
				break;
			}
				
	}
	return 0;
}

//TODO MAY USELESS ... 
//int getLastElement(int (*callback)(int offset,int limit));
	

int hasDifferneces(int dest,int src)
{
	if(src>=getPclen()) src = getPclen()-1;
	if(src<0) src = 0;
	if(dest<0) dest = 0;
	
	if(dest>=getPclen()) dest = getPclen()-1;
	int th = 0;
	char *content = getPcontent();

	if(dest < src)
	{
		int tmp = src;
		src = dest;
		dest = tmp;
	}
	if((dest - src) <= thresholdForDifferneces) return 0;
	for(int i=src;i<dest;i++)
	{
		// no ascii code
		if(fitPattern('a',content[i])&&fitPattern('a',content[i+1])) 
		{
			th++;
			if(th > 3) return 1;
			i+=2;
		}
	}
	return 0;
}

//TODO MY LORD ...
featureDataContainer *getEndFeatureDataContainer(void)
{
	return &mfdc;
}

featureData *getEndFeatureData(void)
{
	return &mfd;
}






