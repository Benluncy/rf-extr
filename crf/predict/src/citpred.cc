#include "citpred.h"
#include "virtualcontent.h"
#include "dirTraversal.h"
#include "hftctl.h" // int getReferenceHeadOffset(void);
#include "eftfun.h" // int getReferenceEndOffset(void);
#include "strHandle.h" //spilitContent
#include "tokens.h"
#include "minEditDistance.h"
#include "crftfun.h"
#include "hftnpse.h"
#include "dict.h"
#include "queue.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <ctype.h>

#ifndef WIN32
#include <strings.h>
#endif // WIN32

#include "debuginfo.h"

int initCitationNode(pCitationNode node)
{
	int i;
	if(node == NULL) return 0;
	for(i=0;i<10;i++) memset(node->author[i],0,50);
	for(i=0;i<10;i++) memset(node->editor[i],0,50);
	memset(node->title,0,50);
	memset(node->journal,0,50);
	memset(node->booktitle,0,50);
	memset(node->publisher,0,50);
	memset(node->institution,0,50);
	memset(node->volume,0,50);
	memset(node->location,0,50);
	memset(node->pages,0,50);
	memset(node->date,0,50);
	memset(node->url,0,50);
	memset(node->isbn,0,50);
	memset(node->tech,0,50);
	memset(node->other,0,50);
	node->next = NULL;
	return 1;	
}

int predftEnQueue(pCNSQ Q,int *currentOffset,char *mpredeli,int refAreaEnd)
{
	if(isFullQueue(Q)) return 0;
	char *content = getPcontent();
	char str[SINGLEWORDLEN];
	char nextpre;
	
	CrfNodeSnapshot crfNodeSnapshot;
	
	int isPublisher = 0;
	
	//spilitContent(char *dest,int dlen,const char *src,int len)
	if((crfNodeSnapshot.offset = spilitContent(str,SINGLEWORDLEN,
			content+(*currentOffset),
			refAreaEnd-(*currentOffset),
			&(crfNodeSnapshot.predeli),
			&(crfNodeSnapshot.nextdeli),
			&nextpre)) != 0)
	{
		int slen = strlen(str);
		sprintf(crfNodeSnapshot.str,"%s",str);
		crfNodeSnapshot.slen = slen;
		//int dval;
		char partStr[1024];
		int psI = 0;
		
		// abbr
		int abbrc = 0; // in Connect status
		int abbrl = 0; // abbr length
		int abbrs = 0 ; // abbr start type
		
		crfNodeSnapshot.quotflag = 0;
		crfNodeSnapshot.pareSflag = 0;
		crfNodeSnapshot.pareEflag = 0;
		crfNodeSnapshot.sqbSflag = 0;
		crfNodeSnapshot.sqbEflag = 0;
		crfNodeSnapshot.braSflag = 0;
		crfNodeSnapshot.braEflag = 0;
		crfNodeSnapshot.stopflag = 0;
		
		for(int i=(*currentOffset);i<=crfNodeSnapshot.offset+(*currentOffset);i++)
		{
			if(i>=refAreaEnd) break;
			if(DIGITLIKE(content[i]))
			{
				partStr[psI] = content[i];
				psI++;
				
			}else if(psI>0)
			{
				partStr[psI]='\0';
				isPublisher = isPublisher || isPublisherInDict(partStr); 
			}
			switch(content[i])
			{
				case '\"':
					crfNodeSnapshot.quotflag ++;
					break;
				case '(':
					crfNodeSnapshot.pareSflag = 1;
					break;
				case ')':
					crfNodeSnapshot.pareEflag = 1;
					break;
				case '[':
					crfNodeSnapshot.sqbSflag = 1;
					break;
				case ']':
					crfNodeSnapshot.sqbEflag = 1;
					break;
				case 'I':
				case 'l':
					if(isBlank(content[i+1]) && crfNodeSnapshot.sqbSflag)
					{
						crfNodeSnapshot.sqbEflag = 1;
					}
					break;
				
				case '{':
					crfNodeSnapshot.braSflag = 1;
					break;
				case '}':
					crfNodeSnapshot.braEflag = 1;
					break;	
					
				case ',':
					// filter Abbreviation
					//if(!(abbrc && abbrl < 5 && abbrs))
					// Dept. 		et. al.
					// break: A A. ?
					if(abbrc)
					{
						if(abbrl < 6 && abbrs) break;
						if(abbrl < 3) break;
					}
					if(crfNodeSnapshot.stopflag == 2)
						crfNodeSnapshot.stopflag = 2;
					else
						crfNodeSnapshot.stopflag = 1;
					break;
				
				case '.':
					if(abbrc)
					{
						if(abbrl < 6 && abbrs) break;
						if(abbrl < 3) break;
					}
					crfNodeSnapshot.stopflag = 2;
					
					break;
				case '!':
				case '?':
					crfNodeSnapshot.stopflag = 2;
					break;
				
					
			}
			
			if(i == (*currentOffset)) // xxx. Aaa.
			{
				if(isAsciiCode(content[i]))
				{
					abbrl = 0;
					abbrc = 1;
					abbrs = isUppercaseCode(content[i]) ? 1 : 0 ;
				}
			}else if(!isAsciiOrDigit(content[i-1]) && isAsciiCode(content[i])){
				abbrc = 1;
				abbrl = 0;
				abbrs = isUppercaseCode(content[i]) ? 1 : 0 ;
			}else if(abbrc && (content[i]>='a' || content[i] <= 'z')){
				abbrl ++;
			}else
			{
				abbrc = 0;
				abbrl = 0;
			}
		}

		*currentOffset += crfNodeSnapshot.offset;

		int spstr[10];
		int splen = 0;
		
		spilitStr(str,slen,spstr,&splen); // spilit

		sprintf(crfNodeSnapshot.str,"%s",str);
		
		// = isBlank(crfNodeSnapshot.predeli)?(*mpredeli):' ';
		crfNodeSnapshot.predeli = *mpredeli;
		crfNodeSnapshot.mpredeli = *mpredeli;
		
		crfNodeSnapshot.digitl = digitlen(str,slen);
		crfNodeSnapshot.puredigit = puredigit(str,slen);
		crfNodeSnapshot.dval = valofdigit(str,slen);
		crfNodeSnapshot.strtype = strfeature(str,slen);
		crfNodeSnapshot.yearlike = yearlike(str,slen);
		crfNodeSnapshot.monthlike = monthlike(str,slen);
		crfNodeSnapshot.volumnlike = vollkwd(str,slen);
		crfNodeSnapshot.pagelike = pagekwd(str,slen);
		crfNodeSnapshot.edsflag = edsFlag(str,slen);
		crfNodeSnapshot.speflag = specialFlag(str,slen);
		crfNodeSnapshot.procflag =  procFlag(str,slen);		
		for(int z=0;z<splen && (crfNodeSnapshot.procflag == 0);z++)
		{
			//str+flag[i],flag[i+1]-flag[i]
			for(int k=z;k<splen;k++)
			{
				crfNodeSnapshot.procflag = procFlag(str+spstr[z],
								spstr[k+1]-spstr[z]);	
			}
			
		}
		//crfNodeSnapshot.namelike = hasNameafterTheOffset0((*currentOffset)
		//					-crfNodeSnapshot.offset-1,
		//					crfNodeSnapshot.offset+1);
		crfNodeSnapshot.namelike = namelike(str,slen,crfNodeSnapshot.nextdeli,
						crfNodeSnapshot.strtype);
						
		for(int z=0;z<splen && (crfNodeSnapshot.namelike == 0);z++)
		{
			//str+flag[i],flag[i+1]-flag[i]
			for(int k=z;k<splen;k++)
			{
				crfNodeSnapshot.namelike = namelike(str+spstr[z],
								spstr[k+1]-spstr[z],
								k==splen-1?crfNodeSnapshot.nextdeli:' ',
								strfeature(str+spstr[z],spstr[k+1]-spstr[z]));	
			}
			
		}
		
		
		crfNodeSnapshot.isNameDict = isNameInDict(str);
		for(int z=0;z<splen && (crfNodeSnapshot.isNameDict == 0);z++)
		{
			//str+flag[i],flag[i+1]-flag[i]
			crfNodeSnapshot.isNameDict = isNameInDict(str+spstr[z]);
		}
		
		crfNodeSnapshot.rLastNameDict = rateLastNameInDict(str);
		for(int z=0;z<splen && (crfNodeSnapshot.rLastNameDict == 0);z++)
		{
			//str+flag[i],flag[i+1]-flag[i]
			crfNodeSnapshot.rLastNameDict = rateLastNameInDict(str+spstr[z]);
		}
		
		
		crfNodeSnapshot.isCountryDict = isCountryInDict(str);
		for(int z=0;z<splen && (crfNodeSnapshot.isCountryDict == 0);z++)
		{
			//str+flag[i],flag[i+1]-flag[i]
			crfNodeSnapshot.isCountryDict = isCountryInDict(str+spstr[z]);
		}
		
		
		crfNodeSnapshot.isFunWordDict = isFunWordInDict(str);

		crfNodeSnapshot.isPlaceNameDict = isPlaceNameInDict(str);
		for(int z=0;z<splen && (crfNodeSnapshot.isPlaceNameDict == 0);z++)
		{
			//str+flag[i],flag[i+1]-flag[i]
			crfNodeSnapshot.isPlaceNameDict = isPlaceNameInDict(str+spstr[z]);
		}
		
		crfNodeSnapshot.isPubliserDict = isPublisherInDict(str) ||isPublisher;
		
		for(int z=0;z<splen && (crfNodeSnapshot.isPubliserDict == 0);z++)
		{
			//str+flag[i],flag[i+1]-flag[i]
			crfNodeSnapshot.isPubliserDict = isPublisherInDict(str+spstr[z]);
		}
		
		crfNodeSnapshot.isArticle = isArticle(str,slen);
		crfNodeSnapshot.deptflag = deptFlag(str,slen);
		crfNodeSnapshot.uniflag = uniFlag(str,slen);
		for(int z=0;z<splen && (crfNodeSnapshot.uniflag == 0);z++)
		{
			//str+flag[i],flag[i+1]-flag[i]
			for(int k=z;k<splen;k++)
			{
				crfNodeSnapshot.uniflag = uniFlag(str+spstr[z],
								spstr[k+1]-spstr[z]);	
			}
			
		}
		
		crfNodeSnapshot.ltdflag = ltdFlag(str,slen);
		for(int z=0;z<splen && (crfNodeSnapshot.ltdflag == 0);z++)
		{
			//str+flag[i],flag[i+1]-flag[i]
			for(int k=z;k<splen;k++)
			{
				crfNodeSnapshot.ltdflag = ltdFlag(str+spstr[z],
								spstr[k+1]-spstr[z]);	
			}
			
		}
		
		crfNodeSnapshot.domainflag = domainFlag(str,slen);
		
		

		if(crfNodeSnapshot.puredigit)
		{
			int vh = valofdigit(str,slen/2);
			int vl = valofdigit(str+(slen/2),(slen+1)/2);
			if(vh == 0 || vl == 0) 	crfNodeSnapshot.imprnum = 0;
			else crfNodeSnapshot.imprnum = vl > vh ? 1 : -1;
		}else
			crfNodeSnapshot.imprnum = 0;
		
		if(!isBlank(crfNodeSnapshot.nextdeli)) *mpredeli = nextpre;
		
		//*currentOffset = crfNodeSnapshot.offset;
		enQueue(Q,crfNodeSnapshot);
		return 1;
	}else
		return 0;
}



// predict one file
pCitationNode CitationInfoPredict(int startOffset,int endOffset)
{

	
	
	
	
	
	return NULL;
}

pCitationNode CitationInfoPredictFile(const char *fileName,int startOffset,int endOffset)
{
	initContent();
	if(!DEBUGFLAG) setNoParse(1); //for release
	parseFile(fileName);
	
	
	return 
}

pCitationNode CitationInfoPredictString(const char *fileName,int startOffset,int endOffset)
{
	//dict prepare
	
	
	//dict free
	
}






// pre and post

void initCitationInfoPredict()
{
	dbConnect();
	hmDictLoad();
	dbFree();
}

void cleanCitationInfoPredict()
{
	hmDictFree();
}



