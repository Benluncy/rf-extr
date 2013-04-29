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
#include <stdlib.h>
//TODO DEBUG
#include "debuginfo.h"

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

//#define INDEBUG 0


/*
#define SHOWLMT(x) printf("%s(%d) %d <=> %d <%d>\n",\
			x,strlen(x),\
			editDistanceS(x,strlen(x),content+realoffset,\
				strlen(x)>strlen(content+realoffset)?strlen(content+realoffset):strlen(x)),\
			T4GOS(strlen(x)),\
			INLMT(x)\
			);
*/
//#define INLMT(x) (editDistanceS(x,strlen(x),content+i,strlen(x)) <= T4GOS(strlen(x)))

//#define IN(x) {printf("in:%s\n",x);fflush(NULL);}
//#define OT(x) {printf("out:%s\n",x);fflush(NULL);}
//#define NX(x) {printf("next:%s\n",x);fflush(NULL);}

endFeatureData mfd;
endFeatureDataContainer mfdc;

#define ECBL 5

OffsetCallback endFunctionList[ENDCALLBACKLEN]={hasPPafterTheOffset,
						hasPPafterTheOffset2,
						hasYearafterTheOffset,
						hasNameafterTheOffset0,
						hasNameafterTheOffset1,//hasNameafterTheOffset2,
						hasSeqOfTheOffset,
						hasSeqOfTheOffset2,
						hasSpecialKeyWords,
						hasLocationafterTheOffset,
						hasWords};
/*						
OffsetCallback endFunctionList[ECBL]={hasPPafterTheOffset,
						hasPPafterTheOffset2,
						hasYearafterTheOffset,
						hasSpecialKeyWords,
						hasWords};
*/

int edOffsetList[9];
int absOffset[2]; // ack fig



inline int cleanEndKWDContainer()
{
	myEc.top = 0;
	return 1;
}

inline int insertEndKWD(const char *key)
{
	if(myEc.top<0) myEc.top = 0;
	if(myEc.top>=200) return 0;
	sprintf(myEc.data[myEc.top].key,"%s",key);
	myEc.data[myEc.top].len = strlen(key);
	myEc.top++;
	return 1;
}

int getLastYearOffset(unsigned int startOffset,int limit)
{
	int offset = 0;
	while((startOffset = hasYearafterTheOffset(startOffset,limit)) != 0)
	{
		offset = startOffset;
	}
	return offset;
}

//TODO DEBUGING
int getLastPageOffset(unsigned int startOffset,int limit)
{
	int offset = 0;
	while((startOffset = hasPPafterTheOffset(startOffset,limit)) != 0)
	{
		offset = startOffset;
	}
	return offset;
}

int getLastPage2Offset(unsigned int startOffset,int limit)
{
	int offset = 0;
	while((startOffset = hasPPafterTheOffset2(startOffset,limit)) != 0)
	{
		offset = startOffset;
	}
	return offset;
}

//int maxLen = 0;

int basicFilter(endFeatureDataContainer *container,unsigned int startOffset)
{
	char *content = getPcontent();
	int cLen = getPclen();	
	int hasContent = 0;
	container->top = 0;
	
	//int avgOffset = 0;
	//int offnum = 0;
	//introduce of settings
	// 0 empty for valued -- index of all offsets
	// 1 acknowledgements etc.
	// 2 table , he is figure ... (a list)
	// 3 end of year // before end of content
	int lastYearOffset = getLastYearOffset(startOffset,cLen);
	int isMarkedEOY = 0;
	
	// 4 end of page // before end of content
	int lastPageOffset = getLastPageOffset(startOffset,cLen);
	int isMarkedEOP = 0;
	
	// 5 end of page2 // before end of content
	int lastPageOffset2 = getLastPage2Offset(startOffset,cLen);
	int isMarkedEOP2 = 0;
	// 6 end of article
	// nothing to init
	
	// 7 end year before ack
	int markedAck=0;
	int markedTable=0;
	
	int endYearBeforeAck = 0;
	int endYearBeforeTable = 0;
	
	int endPageBeforeAck = 0;
	int endPageBeforeTable = 0;
	
	int endPage2BeforeAck = 0;
	int endPage2BeforeTable = 0;
	
	for(int i=0;i<9;i++) edOffsetList[i] = 0;
	
	for(int i=startOffset;i<cLen;i++)
	{
		if(i!=0) if(fitPattern('d',content[i-1])) continue;
		if(INLMT("APPENDIX")
			|| INLMT("ACKNOWLEDGEMENT")
			|| INLMT("AUTHOR BIBLIOGRAPHIES")
			|| INLMT("AUTHOR BIBLIOGRAPHY")
			|| INLMT("AUTHOR BIOGRAPHIES")
			|| INLMT("AUTHOR BIOGRAPHY")
			|| INLMT("ADDRESS INFORMATION")
			|| INLMT("Bibliographical"))
		{
			if(!markedAck)
			{
				endYearBeforeAck = i;
				endPageBeforeAck = i;
				endPage2BeforeAck = i;
				absOffset[0] = i;
				markedAck = 1;
			}
			
		}
		for(int x = 0;x < myEc.top;x++)
		{
			if(INLMT(myEc.data[x].key))
			{
				if(!markedTable)
				{
					endYearBeforeTable = i;
					endPageBeforeTable = i;
					endPage2BeforeTable = i;
					markedTable = 1;
					absOffset[1] = i;
				}
				
			}
		}
		if(markedAck && markedTable) break;
	}
	
	
	
	endYearBeforeAck = endYearBeforeAck == 0 ? 0 : getLastPageOffset(startOffset,endYearBeforeAck);
	
	//int isMarkedEYBA = 0;
	
	// 8 end year before table
	endYearBeforeTable = endYearBeforeTable == 0 ? 0 : getLastPageOffset(startOffset,endYearBeforeTable);
	
	//int isMarkedEYBT = 0;
	
	// 9 end year before ack or table
	int endYearBeforeAckOrTable = MINANDNZ(endYearBeforeAck,endYearBeforeTable);
	
	//int isMarkedEYBAOT = 0;
	
	
	// 10 end page before ack
	endPageBeforeAck = endPageBeforeAck == 0 ? 0 : getLastPageOffset(startOffset,endPageBeforeAck);
	
	//int isMarkedEPBA = 0;
	
	
	// 11 end page before table
	endPageBeforeTable = endPageBeforeTable == 0 ? 0 : getLastPageOffset(startOffset,endPageBeforeTable);
	
	//int isMarkedEPBT = 0;
	
	// 12 end page before ack or table
	int endPageBeforeAckOrTable = MINANDNZ(endPageBeforeAck,endPageBeforeTable);
	
	//int isMarkedEPBAOT = 0;
	
	// 13 end page2 before ack
	endPage2BeforeAck = endPage2BeforeAck == 0 ? 0 : getLastPage2Offset(startOffset,endPage2BeforeAck);
	
	//int isMarkedEP2BA = 0;
	
	// 14 end page2 before table
	endPage2BeforeTable = endPage2BeforeTable == 0 ? 0 : getLastPage2Offset(startOffset,endPage2BeforeTable);
	
	//int isMarkedEP2BT = 0;
	
	// 15 end page2 before ack or table
	int endPage2BeforeAckOrTable = MINANDNZ(endPage2BeforeAck,endPage2BeforeTable);
	//int isMarkedEP2BAOT = 0;
	
	// 16 index of year
	int nextYearOffset = hasYearafterTheOffset(startOffset,cLen);
	int lastYearOffsetForIndex = startOffset;
	int indexOfYear = 0;
	// 17 index of page
	int nextPageOffset = hasPPafterTheOffset(startOffset,cLen);
	int lastPageOffsetForIndex = startOffset;
	int indexOfPage = 0;
	
	// 18 index of page2
	int nextPage2Offset = hasPPafterTheOffset2(startOffset,cLen);
	int lastPage2OffsetForIndex = startOffset;
	int indexOfPage2 = 0;
	
	for(int i=startOffset;i<cLen;i++)
	{//endYearBeforeAck
		// init
		hasContent = 0;
		
		// 16 index of year
		if(nextYearOffset != -1)
		{
			if(i==nextYearOffset )
			{
				indexOfYear ++;
				container->data[container->top].t[16] = indexOfYear;
				lastYearOffsetForIndex = nextYearOffset;
				nextYearOffset = hasYearafterTheOffset(nextYearOffset,cLen);
				if(nextYearOffset == 0) nextYearOffset = -1;
				//printf("[Y]");
				hasContent = 1;
			}else if(!haveDiffernecesE(i,nextYearOffset))
			{
				container->data[container->top].t[16] = indexOfYear+1;
			}else if(!haveDiffernecesE(i,lastYearOffsetForIndex))
			{
				container->data[container->top].t[16] = indexOfYear;
			}
		
		}
		
		// 17 index of page
		if(nextPageOffset != -1)
		{
			if(i==nextPageOffset)
			{
				indexOfPage ++ ;
				container->data[container->top].t[17] = indexOfPage;
				lastPageOffsetForIndex = nextPageOffset;
				nextPageOffset = hasPPafterTheOffset(nextPageOffset,cLen);
				//printf("#################################RECEIVE:%d\n",nextPageOffset);
				if(nextPageOffset == 0) nextPageOffset = -1;
				//printf("[P]");
				//printfContextS(i,"P1");
				hasContent = 1;
			}else if(!haveDiffernecesE(i,nextPageOffset))
			{
				container->data[container->top].t[17] = indexOfPage+1;
			}else if(!haveDiffernecesE(i,lastPageOffsetForIndex))
			{
				container->data[container->top].t[17] = indexOfPage;
			}
		
		}
		
		// 18 index of page2
		if(nextPage2Offset != -1)
		{
			if(i==nextPage2Offset)
			{
				indexOfPage2 ++ ;
				container->data[container->top].t[17] = indexOfPage2;
				lastPage2OffsetForIndex = nextPage2Offset;
				nextPage2Offset = hasPPafterTheOffset(nextPage2Offset,cLen);
				if(nextPage2Offset == 0) nextPage2Offset = -1;
				//printf("[PP]");
				//printfContextS(i,"P2");
				hasContent = 1;
			}else if(!haveDiffernecesE(i,nextPage2Offset))
			{
				container->data[container->top].t[17] = indexOfPage2+1;
			}else if(!haveDiffernecesE(i,lastPage2OffsetForIndex))
			{
				container->data[container->top].t[17] = indexOfPage2;
			}
		}
		
		
		
		if(i!=0)
		{
			// 7 ~ 15
			// 7 end year before ack
			if(endYearBeforeAck == i)
			{
				container->data[container->top].t[7] = 2;
				edOffsetList[0] = container->top;
				hasContent = 1;
				//isMarkedEYBA = 1;
			}
			// 8 end year before table
			if(endYearBeforeTable == i)
			{
				container->data[container->top].t[8] = 2;
				edOffsetList[1] = container->top;
				hasContent = 1;
				//isMarkedEYBT = 1;
			}
			// 9 end year before ack or table
			//if(endYearBeforeAckOrTable == i)
			if((endYearBeforeAck == i) || (endYearBeforeTable == i))
			{
				container->data[container->top].t[9] = 2;
				hasContent = 1;
				edOffsetList[2] = container->top;
				//isMarkedEYBAOT = 1;
			}
			// 10 end page before ack
			if(endPageBeforeAck == i)
			{
				container->data[container->top].t[10] = 2;
				edOffsetList[3] = container->top;
				hasContent = 1;
				//isMarkedEPBA = 1;
			}
			// 11 end page before table
			if(endPageBeforeTable == i)
			{
				container->data[container->top].t[11] = 2;
				edOffsetList[4] = container->top;
				hasContent = 1;
				//isMarkedEPBT = 1;
			}
			// 12 end page before ack or table
			//if(endPageBeforeAckOrTable == i)
			if((endPageBeforeAck == i) || (endPageBeforeTable == i))
			{
				container->data[container->top].t[12] = 2;
				hasContent = 1;
				edOffsetList[5] = container->top;
				//isMarkedEPBAOT = 1;
			}
			// 13 end page2 before ack
			if(endPage2BeforeAck == i)
			{
				container->data[container->top].t[13] = 2;
				edOffsetList[6] = container->top;
				hasContent = 1;
				//isMarkedEP2BA = 1;
			}
			// 14 end page2 before table
			if(endPage2BeforeTable == i)
			{
				container->data[container->top].t[14] = 2;
				edOffsetList[7] = container->top;
				hasContent = 1;
				//isMarkedEP2BT = 1;
			}
			// 15 end page2 before ack or table
			//if(endPage2BeforeAckOrTable == i)
			if((endPage2BeforeAck == i) || (endPage2BeforeTable == i))
			{
				container->data[container->top].t[15] = 2;
				hasContent = 1;
				edOffsetList[8] = container->top;
				//isMarkedEP2BAOT = 1;
			}
			//
			// ingore useless i == 0 or past is not data(data:[0-9][a-z][A-Z])
			if(fitPattern('d',content[i-1]) && !hasContent) continue;
		}
		
		
		
		
		//////////////////////////////////////////////////////////////////////////
		// 0 empty for valued -- index of all offsets
		// leave empty
		
		// 1 acknowledgements etc.
		//
		if(INLMT("APPENDIX")
			|| INLMT("ACKNOWLEDGEMENT")
			|| INLMT("AUTHOR BIBLIOGRAPHIES") 
			|| INLMT("AUTHOR BIBLIOGRAPHY")
			|| INLMT("AUTHOR BIOGRAPHIES")
			|| INLMT("AUTHOR BIOGRAPHY")
			|| INLMT("ADDRESS INFORMATION")
			|| INLMT("Bibliographical"))
			//|| INLMT("Bibliography");
		{
			container->data[container->top].t[1] = 1;
			hasContent = 1;
		}
		
		// 2 table , he is figure ... (a list)
		for(int x = 0;x < myEc.top;x++)
		{
			if(INLMT(myEc.data[x].key))
			{
				//printf("[+]");
				container->data[container->top].t[2] = 1;
				hasContent = 1;
			}
		}
		
		// 3 end of year // before end of article
		if(!haveDifferneces(lastYearOffset,i))
		{
			if(!isMarkedEOY)
			{
				isMarkedEOY = 1;
				hasContent = 1;
			}
			container->data[container->top].t[3] = 2;
		}else if(i > lastYearOffset)
		{
			container->data[container->top].t[3] = 1;
		}
		
		// 4 end of page // before end of article
		if(!haveDifferneces(lastPageOffset,i))
		{
			if(!isMarkedEOP)
			{
				isMarkedEOP = 1;
				hasContent = 1;
			}
			container->data[container->top].t[4] = 2;
		}else if(i > lastPageOffset)
		{
			container->data[container->top].t[4] = 1;
		}
		
		// 5 end of page2 // before end of article
		if(!haveDifferneces(lastPageOffset2,i))
		{
			if(!isMarkedEOP2)
			{
				isMarkedEOP2 = 1;
				hasContent = 1;
			}
			container->data[container->top].t[5] = 2;
		}else if(i > lastPageOffset2)
		{
			container->data[container->top].t[5] = 1;
		}
		
		// 6 end of article (move down)

		// 7 end year before ack
		
		if(i>endYearBeforeAck && endYearBeforeAck && !container->data[container->top].t[7] )
		{
			if(!haveDifferneces(endYearBeforeAck,i))
				container->data[container->top].t[7] = 1;
		}
		
		// 8 end year before table
		if(i>endYearBeforeTable && endYearBeforeTable && !container->data[container->top].t[8] )
		{
			if(!haveDifferneces(endYearBeforeTable,i))
				container->data[container->top].t[8] = 1;
		}
		// 9 end year before ack or table
		if(i>endYearBeforeAckOrTable && endYearBeforeAckOrTable && !container->data[container->top].t[9] )
		{
			if((!haveDifferneces(endYearBeforeAck,i)) || (!haveDifferneces(endYearBeforeTable,i)))
				container->data[container->top].t[9] = 1;
		}

		// 10 end page before ack
		if(i>endPageBeforeAck && endPageBeforeAck && !container->data[container->top].t[10] )
		{
			if(!haveDifferneces(endPageBeforeAck,i))
				container->data[container->top].t[10] = 1;
		}
		
		// 11 end page before table
		if(i>endPageBeforeTable && endPageBeforeTable && !container->data[container->top].t[11] )
		{
			if(!haveDifferneces(endPageBeforeTable,i))
				container->data[container->top].t[11] = 1;
		}
		
		// 12 end page before ack or table
		if(i>endPageBeforeAckOrTable && endPageBeforeAckOrTable && !container->data[container->top].t[12] )
		{
			if((!haveDifferneces(endPageBeforeAck,i)) || (!haveDifferneces(endPageBeforeTable,i)))
				container->data[container->top].t[12] = 1;
		}
		// 13 end page2 before ack
		if(i>endPage2BeforeAck && endPage2BeforeAck && !container->data[container->top].t[13] )
		{
			if(!haveDifferneces(endPage2BeforeAck,i))
				container->data[container->top].t[13] = 1;
		}
		// 14 end page2 before table
		
		if(i>endPage2BeforeTable && endPage2BeforeTable && !container->data[container->top].t[14] )
		{
			if(!haveDifferneces(endPage2BeforeTable,i))
				container->data[container->top].t[14] = 1;
		}
		// 15 end page2 before ack or table
		if(i>endPage2BeforeAckOrTable && endPage2BeforeAckOrTable && !container->data[container->top].t[15] )
		{
			if((!haveDifferneces(endPage2BeforeAck,i)) ||  (!haveDifferneces(endPage2BeforeTable,i)))
				container->data[container->top].t[15] = 1;
		}

		// move up
		
		//////////////////////////////////////////////////////////////////////////

		
		// 6 end of article
		/*
		if(!haveDiffernecesH(cLen,i))
		{
			if(ENDCTNMAX <= container->top) // 
			{
				fprintf(stderr,"[ERROR] pool is full! %s %d",__FILE__,__LINE__);
				getchar();
			}
			container->data[container->top].t[6] = 1;
			container->data[container->top].offset = i;
			container->top++;
			if(ENDCTNMAX < container->top)
			{
				fprintf(stderr,"[WARNING] pool is full! %s %d",__FILE__,__LINE__);
			}
			return 1;
		}*/
		
		if(hasContent || !haveDiffernecesH(cLen,i))// || !haveDiffernecesH(cLen,i))
		{
			/*
			if(container->top==0)
			{
				avgOffset += container->data[container->top].offset;
				offnum ++;
				container->data[container->top].offset = i;
				container->top++;
				if(container->top >= ENDCTNMAX)
				{
					fprintf(stderr,"[ERROR] pool is full! %s %d",__FILE__,__LINE__);
					getchar();
				}
			}
			else
			{
			
				if(!haveDiffernecesH(container->data[container->top-1].offset,i))
				{
					avgOffset += i;
					offnum ++;
					for(int k=1;k<ENDLEN;k++)
					{
						container->data[container->top-1].t[k] = container->data[container->top-1].t[k]+container->data[container->top].t[k];
						container->data[container->top].t[k] = 0;
						//MINANDNZ(container->data[container->top].t[k],container->data[container->top+1].t[k]);
					}
				}else
				{
					
					container->data[container->top-1].offset = avgOffset / offnum;
					offnum = 1;
					container->data[container->top].offset = i;
					container->top++;
					if(container->top >= ENDCTNMAX)
					{
						fprintf(stderr,"[ERROR] pool is full! %s %d",__FILE__,__LINE__);
						getchar();
					}
			//	}
			}
			*/
			
			container->data[container->top].offset = i;
			container->top++;
			if(container->top >= ENDCTNMAX)
			{
				fprintf(stderr,"[ERROR] pool is full! %s %d",__FILE__,__LINE__);
				getchar();
			}

			// 6 end of article
			if(!haveDiffernecesH(cLen,i))
			{
				//container->data[container->top].offset = i;
				container->data[container->top-1].t[6] = 1;
				//container->top ++ ;
				return 1;
			}
			
			//if(container->top >= 200) printf("found!");
			//maxLen = container->top > maxLen ? container->top : maxLen;
		}
		
	}
	return 0;
}

//int getMaxLen(){return maxLen;}


int combineOffsets(endFeatureDataContainer *container)//combine nearly offsets and make sure 
{
	//return 1;
	int j = 0;
	int lastOffset = container->data[0].offset;
	int th=0;
	int realOffset[9];// 7 8 10 11  13 14
	int markedReal[9];
	for(j=0;j<9;j++)
	{
		markedReal[j] = edOffsetList[j]==0?1:0;
		realOffset[j] = 0;
	}
	//edOffsetList[0] = container->top;
	/*
	realOffset[0] = container->data[edOffsetList[0]].t[16];
	realOffset[1] = container->data[edOffsetList[1]].t[16];
	
	realOffset[2] = container->data[edOffsetList[2]].t[17];
	realOffset[3] = container->data[edOffsetList[3]].t[17];
	
	realOffset[4] = container->data[edOffsetList[4]].t[18];
	realOffset[5] = container->data[edOffsetList[5]].t[18];
	*/
	for(th=0;(!allMarked(markedReal,6)) && ((edOffsetList[j]+th)< container->top || (edOffsetList[j]-th) >= 0);th++)
	{
		for(j=0;j<9;j++)
		{
			if((edOffsetList[j]+th) < container->top)
			{
				if(container->data[edOffsetList[j]+th].t[16+(j/3)]!=0 && !markedReal[j])
				{
					realOffset[j] = container->data[edOffsetList[j]+th].t[16+(j/3)];
					markedReal[j] = 1;
				}
			}
			if((edOffsetList[j]-th) >= 0)
			{
				if(container->data[edOffsetList[j]-th].t[16+(j/3)]!=0 && !markedReal[j])
				{
					realOffset[j] = container->data[edOffsetList[j]-th].t[16+(j/3)];
					markedReal[j] = 1;
				}
			}
		}
	}
	
	for(j=0;j<9;j++) markedReal[j] = realOffset[j] || !edOffsetList[j];
	for(th=container->top;(!allMarked(markedReal,6)) && th>=0;th--)
	{
		for(j=0;j<9;j++)
		{
			if(container->data[th].t[16+(j/2)]!=0 && !markedReal[j])
			{
				realOffset[j] = container->data[th].t[16+(j/3)];
				markedReal[j] = 1;
			}
		}
	}
	
	//introduce of settings
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
	//for(int i=1;i<container->top-1;i++)
	for(int i=1;i<container->top;i++)
	{
		//container->data[j].offset = container->data[i].offset;
		//haveDifferneces(int dest,int src)
		//if(!haveDiffernecesH(lastOffset,container->data[i].offset))
		for(j=0;j<9;j++)
		{
			if(container->data[i].t[16+j] != 0)
			{
				if(j%3 == 2)
				{
					container->data[i].t[19+j] = MINANDNZ(container->data[i].t[18+j],container->data[i].t[17+j]);
				}else
					container->data[i].t[19+j] = ABSDIFF(container->data[i].t[16+j],realOffset[j]);
				//printf("[%d-%d-%d]",i,19+j,container->data[i].t[19+j]);
			} 
		}
		
		
	}
	
	
	// do combine
	
	j=0;
	int offNum=1;
	int sumOffset = container->data[0].offset;;
	container->data[j].t[0] = 1;
	for(int i=1;i<container->top;i++)
	{
		if(!haveDifferneces(lastOffset,container->data[i].offset))
		{
			sumOffset += container->data[i].offset;
			offNum ++;
			for(int z=1;z<16;z++)
			{
				container->data[j].t[z] = MAX2(container->data[i].t[z],container->data[j].t[z]);
			}
			for(int z=16;z<ENDLEN;z++)
			{
				container->data[j].t[z] = MIN2(container->data[i].t[z],container->data[j].t[z]);
			}
		}else
		{
			container->data[j].offset = sumOffset / offNum;
			j++;
			container->data[j].offset = container->data[i].offset;
			lastOffset = container->data[i].offset;
			container->data[j].t[0] = j+1;
			offNum = 1;
			sumOffset =  container->data[i].offset;
			for(int k=1;k<ENDLEN;k++)
			{
				container->data[j].t[k] = container->data[i].t[k];
			}
			
		}
	}
	container->top = j+1;
	/*
	if(!haveDifferneces(lastOffset,getPclen()))
	{
		for(int k=0;k<ENDLEN;k++)
		{
			container->data[j].t[k] = container->data[container->top-1].t[k] || container->data[j].t[k];
		}
	}else
	{
		j++;
		container->data[j].offset = container->data[container->top-1].offset;
		for(int k=0;k<ENDLEN;k++)
		{
			container->data[j].t[k] = container->data[container->top-1].t[k];
		}
	}
	*/
	/*
	j++;
	container->data[j].offset = container->data[container->top-1].offset;
	for(int k=0;k<ENDLEN;k++)
	{
		container->data[j].t[k] = container->data[container->top-1].t[k];
	}*/
	
	
	//container->top = j+1;
	return 1;
}

int makeSequenceForCombinedOffsets(endFeatureDataContainer *container)
{
	return 1;
	unsigned int max[ENDLEN];
	//unsigned int before[ENDLEN];
	unsigned int hasData[ENDLEN];
	int top = container->top;
	for(int i=0;i<ENDLEN;i++)
	{
		max[i] = 1;
		hasData[i] = 0;
	}
	for(int i=0;i<top;i++)
		for(int j=1;j<ENDLEN;j++) 
			if(container->data[i].t[j] != 0 ) hasData[j] = 1;
	
	for(int i=0;i<top;i++)
	{
		for(int j=1;j<ENDLEN;j++)
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
		for(int j=0;j<ENDLEN;j++)
		{
			printf("%3d ",container->data[i].t[j]);
		}
		printf("\n");
	}*/
	return 1;
}


int genNextDataForEndfeature(FILE *fp,endFeatureData fd,int start)
{
	//return 1;
	//return 1;
	int offset = fd.offset;
	int lmt;
	
	//prepareDensityData();
	
	/*
	// f g1
	lmt = -50;
	for(int z=0;z<ECBL;z++)//ENDCALLBACKLEN
	{
		fprintf(fp,"%d:%d ",start++,((endFunctionList[z](offset,lmt) > 
			endFunctionList[z](offset,-lmt)))?1:0);
	}
	// */
	/*
	fprintf(fp,"%d:%d ",start++,((hasPPafterTheOffset(offset,lmt) >= hasPPafterTheOffset(offset,-lmt)))?1:-1);
	fprintf(fp,"%d:%d ",start++,((hasPPafterTheOffset2(offset,lmt) >= hasPPafterTheOffset2(offset,-lmt)))?1:-1);
	fprintf(fp,"%d:%d ",start++,((hasYearafterTheOffset(offset,lmt) >= hasYearafterTheOffset(offset,-lmt)))?1:-1);
	fprintf(fp,"%d:%d ",start++,((hasNameafterTheOffset0(offset,lmt) >= hasNameafterTheOffset0(offset,-lmt)))?1:-1);
	fprintf(fp,"%d:%d ",start++,((hasNameafterTheOffset1(offset,lmt) >= hasNameafterTheOffset1(offset,-lmt)))?1:-1);
	fprintf(fp,"%d:%d ",start++,((hasNameafterTheOffset2(offset,lmt) >= hasNameafterTheOffset2(offset,-lmt)))?1:-1);
	// */
	
	/*
	// f g2
	lmt = -300;
	fprintf(fp,"%d:%d ",start++,(asciiCodeDensity(offset,lmt) >= asciiCodeDensity(offset,-lmt))?1:-1);
	fprintf(fp,"%d:%d ",start++,(dataDensity(offset,lmt) >= dataDensity(offset,-lmt))?1:-1);
	fprintf(fp,"%d:%d ",start++,(wordsNumber(offset,lmt) >= wordsNumber(offset,-lmt))?1:-1);
	// */
	
	/*
	// f g3
	lmt = 30;
	fprintf(fp,"%d:%d ",start++,((hasPPafterTheOffset(offset,lmt) >= hasPPafterTheOffset(offset,-lmt)))?1:-1);
	fprintf(fp,"%d:%d ",start++,((hasPPafterTheOffset2(offset,lmt) >= hasPPafterTheOffset2(offset,-lmt)))?1:-1);
	fprintf(fp,"%d:%d ",start++,((hasYearafterTheOffset(offset,lmt) >= hasYearafterTheOffset(offset,-lmt)))?1:-1);
	fprintf(fp,"%d:%d ",start++,((hasNameafterTheOffset0(offset,lmt) >= hasNameafterTheOffset0(offset,-lmt)))?1:-1);
	fprintf(fp,"%d:%d ",start++,((hasNameafterTheOffset1(offset,lmt) >= hasNameafterTheOffset1(offset,-lmt)))?1:-1);
	fprintf(fp,"%d:%d ",start++,((hasNameafterTheOffset2(offset,lmt) >= hasNameafterTheOffset2(offset,-lmt)))?1:-1);
	// */	

	/*	
	// f g4
	//absOffset[0] = 
	lmt = 30;
	powerWrite(fp,start,ABSDIFF(offset,absOffset[0]),10);
	start+=10;
	powerWrite(fp,start,ABSDIFF(offset,absOffset[1]),10);
	start+=10;
	//*/
	
		
	// no use start
	/*
	int absdiff = ABSDIFF(offset,getReferenceEndOffset());
	//!haveDiffernecesD(_mfdc->data[i].offset,targetOffset)
	if(haveDiffernecesD(offset,getReferenceEndOffset()))
	{
		absdiff = 1;
	}else
	{
		absdiff = absdiff < 10 ? absdiff+10 : absdiff;
	}
	
	powerWrite(fp,start,absdiff,2);
	start+=2;*/
	// no use end 
	
	/*
	// f g5
	lmt = -200;
	fprintf(fp,"%d:%d ",start++,(hasSeqOfTheOffset(offset,lmt)?1:-1));
	fprintf(fp,"%d:%d ",start++,(hasSeqOfTheOffset2(offset,lmt)?1:-1));	
	fprintf(fp,"%d:%d ",start++,((hasSeqOfTheOffset(offset,lmt) >= hasSeqOfTheOffset(offset,-lmt)))?1:-1);
	fprintf(fp,"%d:%d ",start++,((hasSeqOfTheOffset2(offset,lmt) >= hasSeqOfTheOffset2(offset,-lmt)))?1:-1);
	//*/	
	//fprintf(fp,"%d:%d ",start++,asciiCodeDensity(offset,lmt) >= asciiCodeDensity(offset,-lmt));
	//fprintf(fp,"%d:%d ",start++,dataDensity(offset,lmt) >= dataDensity(offset,-lmt));
	
	//}
	
	//32
	//fprintf(fp,"%d:%f ",start++,(double)offset/getPclen());
	//fprintf(fp,"%d:%f ",start++,(double)(getPclen()-offset)/offset);
	//rateWrite(fp,start,(double)offset/getPclen());
	//start+=5;
	
	
	//for(int i=0;i<MECL;i++)
	//{
		//rateWrite(fp,start,(fd.offset == getPclen()) ? -1 :((double)fd.fid[i][0]/fd.offset)/
		//			((fd.fid[i][1]-fd.fid[i][0])/(getPclen()-fd.offset)));
		//start+=5;
		
		/*
		// f g6
		
		fprintf(fp,"%d:%d ",start++,(fd.density[i][0][1] < fd.density[i][0][0])?1:-1);
		fprintf(fp,"%d:%d ",start++,(fd.density[i][1][1] < fd.density[i][1][0])?1:-1);
		//fprintf(fp,"%d:%d ",start++,(fd.density[i][2][1] < fd.density[i][2][0])?1:-1);
		//fprintf(fp,"%d:%d ",start++,(fd.vari[1] < fd.vari[0])?1:-1);
		
		
		// */
		
		//fprintf(fp,"%d:%f ",start++,quot(fd.density[i][0][1],fd.density[i][0][0]));
		//fprintf(fp,"%d:%f ",start++,quot(fd.density[i][1][1],fd.density[i][1][0]));
		//fprintf(fp,"%d:%f ",start++,quot(fd.density[i][2][1],fd.density[i][2][0]));
		//fprintf(fp,"%d:%f ",start++,quot(fd.vari[1],fd.vari[0]));
		
		/*
		// f g7
		int wide = 6;
		
		rankWrite(fp,start,fd.seq[i][0],wide);
		start+=wide;

		rankWrite(fp,start,fd.seq[i][1],wide);
		start+=wide;
		
		rankWrite(fp,start,fd.seq[i][2],wide);
		start+=wide;
		
		rankWrite(fp,start,fd.seq[i][3],wide);
		start+=wide;
		// */
		
	//}
	
	return start;
}




int prepareDensityData(void)
{
	return 1;
	/*
	int beforeData;
	int totalData;
	
	for(int i=0;i<mfdc.top;i++)
	{
		
		for(int j=0;j<ENDCALLBACKLEN;j++)
		{
			offsetStat(mfdc.data[i].offset,
					&totalData,&beforeData,endFunctionList[j]);
			mfdc.data[i].fid[j][0] = beforeData;
			mfdc.data[i].fid[j][1] = totalData;
		}
	}
	*/
	//density and etc
	
	// adjacencyOffset setting ... 
	/*
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
	*/
	
	/*
	int diff;
	//int lmt;
	//density && variance setting ... 
	//CloseKWD ckwd;
	for(int i=0;i<mfdc.top;i++)
	{
		for(int j=0;j<MECL;j++)
		{
			//printf("X");
			mfdc.data[i].density[j][0][0] = (double)mfdc.data[i].fid[j][0]/NOTZERO(mfdc.data[i].offset);
			mfdc.data[i].density[j][0][1] = (double)(mfdc.data[i].fid[j][1]-mfdc.data[i].fid[j][0])/NOTZERO(getPclen()-mfdc.data[i].fid[j][0]);
			
			offsetBetweenStat(mfdc.data[i].offset,mfdc.data[i].adjacencyOffset[0],&diff,endFunctionList[j]);
			//mfdc.data[i].density[j][1][0] = (double)(mfdc.data[i].offset-mfdc.data[i].adjacencyOffset[0])/NOTZERO(diff);
			mfdc.data[i].density[j][1][0] = (double)diff/NOTZERO(VALUESDIFF(mfdc.data[i].offset,mfdc.data[i].adjacencyOffset[0]));
			
			offsetBetweenStat(mfdc.data[i].offset,mfdc.data[i].adjacencyOffset[1],&diff,endFunctionList[j]);
			mfdc.data[i].density[j][1][1] = (double)diff/NOTZERO(VALUESDIFF(mfdc.data[i].adjacencyOffset[1],mfdc.data[i].offset));
			
			
			//lmt = 300;
			// prev && next 300* words
			//offsetBetweenStat(mfdc.data[i].offset,LMB(mfdc.data[i].offset-lmt),&diff,endFunctionList[j]);
			//mfdc.data[i].density[j][2][0] = (double)diff/NOTZERO(VALUESDIFF(mfdc.data[i].offset,LMB(mfdc.data[i].offset-lmt)));
			
			//offsetBetweenStat(mfdc.data[i].offset,LMB(mfdc.data[i].offset+lmt),&diff,endFunctionList[j]);
			//mfdc.data[i].density[j][1][1] = (double)diff/NOTZERO(VALUESDIFF(LMB(mfdc.data[i].offset+lmt),mfdc.data[i].offset));
			
			// variance 
			//int getCloseKWD(int offset,CloseKWD *closeKWD,int (*callback)(int offset,int limit));
			//getCloseKWD(mfdc.data[i].offset,&ckwd,endFunctionList[j]);
			//mfdc.data[i].vari[0] = getVariance(ckwd.prev,ckwd.pLen);
			//mfdc.data[i].vari[1] = getVariance(ckwd.next,ckwd.nLen);
		}
		//printf("\n");
	}
	*/
	
	/*
	// sequence setting ... 
	printf(".");fflush(NULL);//TODO TIPS
	for(int i=0;i<mfdc.top;i++)
	{
		for(int k=0;k<ENDCALLBACKLEN;k++)
		{
			for(int z=0;z<4;z++) mfdc.data[i].seq[k][z] = 1;
		}
		
		for(int j=0;j<mfdc.top;j++)
		{
			if(i==j) continue;
			for(int k=0;k<ENDCALLBACKLEN;k++)
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
	// */

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

int haveDifferneces(int dest,int src) // extend
{
	if(src<0) src = 0;
	if(dest<0) dest = 0;
	if(src>=getPclen()) src = getPclen()-1;
	if(dest>=getPclen()) dest = getPclen()-1;

	if(INABSDIFF(dest,src)) return 0;

	int th = 0;
	char *content = getPcontent();

	if(dest < src)
	{
		int tmp = src;
		src = dest;
		dest = tmp;
	}
	for(int i=src;i<dest-1;i++)
	{
		/*
		// no ascii code
		//if(!(content[i]>='a'&&content[i]<='z')||(content[i]>='A'&&content[i]<='Z')) th++;
		if(content[i] != ' ' && content[i] != '\r' && content[i] != '\n' &&fitPattern()) th++;
		
		if(th>2) return 1;
		*/
		
		if(fitPattern('a',content[i])&&fitPattern('a',content[i+1])) 
		{
			//return 1;
			th++;
			if(th > 0) return 1;
			i+=2;
		}
	}
	return 0;
}

int haveDiffernecesD(int dest,int src) // extend
{
	if(src<0) src = 0;
	if(dest<0) dest = 0;
	if(src>=getPclen()) src = getPclen()-1;
	if(dest>=getPclen()) dest = getPclen()-1;

	if(EXINABSDIFF(dest,src)) return 0;

	int th = 0;
	char *content = getPcontent();

	if(dest < src)
	{
		int tmp = src;
		src = dest;
		dest = tmp;
	}
	for(int i=src;i<dest-1;i++)
	{
		/*
		// no ascii code
		//if(!(content[i]>='a'&&content[i]<='z')||(content[i]>='A'&&content[i]<='Z')) th++;
		if(content[i] != ' ' && content[i] != '\r' && content[i] != '\n' &&fitPattern()) th++;
		
		if(th>2) return 1;
		*/
		
		if(fitPattern('a',content[i])&&fitPattern('a',content[i+1])) 
		{
			//return 1;
			th++;
			if(th > 0) return 1;
			i+=2;
		}
	}
	return 0;
}

int haveDiffernecesH(int dest,int src)
{
	if(src>=getPclen()) src = getPclen()-1;
	if(src<0) src = 0;
	if(dest<0) dest = 0;
	
	if(dest>=getPclen()) dest = getPclen()-1;
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
		//if(!(content[i]>='a'&&content[i]<='z')||(content[i]>='A'&&content[i]<='Z')) th++;
		if(content[i] != ' ' && content[i] != '\r' && content[i] != '\n') return 1;
	}
	return 0;
}

int haveDiffernecesE(int dest,int src) // extend
{
	if(src<0) src = 0;
	if(dest<0) dest = 0;
	if(src>=getPclen()) src = getPclen()-1;
	if(dest>=getPclen()) dest = getPclen()-1;

	if(EXINABSDIFF(dest,src)) return 0;

	int th = 0;
	char *content = getPcontent();

	if(dest < src)
	{
		int tmp = src;
		src = dest;
		dest = tmp;
	}
	for(int i=src;i<dest-1;i++)
	{
		/*
		// no ascii code
		//if(!(content[i]>='a'&&content[i]<='z')||(content[i]>='A'&&content[i]<='Z')) th++;
		if(content[i] != ' ' && content[i] != '\r' && content[i] != '\n' &&fitPattern()) th++;
		
		if(th>2) return 1;
		*/
		
		if(fitPattern('d',content[i])&&fitPattern('d',content[i+1])) 
		{
			//return 1;
			th++;
			if(th > 3) return 1;
			i+=2;
		}
	}
	return 0;
}
/*
int haveDiffernecesH(int dest,int src)
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
		if(content[i] != ' ' && content[i] != '\r' && content[i] != '\n') return 1;
	}
	return 0;
}
*/


int allMarked(int a[],int len)
{
	int i;
	for(i=0;i<len;i++)
	{
		if(a[i]==0) return 0;
	}
	return 1;
}


int powerWrite(FILE *fp,int start,int value,int len)
{
	int i;
	value = value < 0 ? -value : value;
	for(i=0;value>0;i++)
	{
		value /= 2;
	}
	//printf("[pw:%d]",i);
	rankWrite(fp,start,i,len);
	return 1;
}

//TODO MY LORD ...
endFeatureDataContainer *getEndFeatureDataContainer(void)
{
	return &mfdc;
}

endFeatureData *getEndFeatureData(void)
{
	return &mfd;
}






