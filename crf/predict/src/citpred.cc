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
	char samplestr[2048];
	int refAreaStart;
	int refAreaEnd;
	int currentOffset;
	//char *content;
	char mpredeli=' ';
	CNSQ preCNSQ;
	CNSQ nextCNSQ;
	
	clearQueue(&preCNSQ);
	clearQueue(&nextCNSQ);
	
	pCrfNodeSnapshot pCNS;
	
	//
	refAreaStart = startOffset;
	refAreaEnd = endOffset;
	currentOffset = refAreaStart;

	
	
	
	int httpStatus = 0;
	//int httpTime = 0;
	
	int quotStatus[3];//  AT/NA IN/NIN OUT/NOUT
	int quotTime = 0;
	
	int pareStatus[3];// AT/NA IN/NIN OUT/NOUT 
	int paraFlag = 0;
	
	int sqbStatus[3];// AT/NAT IN/NIN OUT/NOUT 
	int sqbFlag = 0;
	
	int braStatus[3];// AT/NAT IN/NIN OUT/NOUT
	int braFlag = 0;
	
	
	// to make sure data is like this : )<<ignore this |from here to calculate>> ((()))
	int paraCache = 0; 
	int sqbCache = 0;
	int braCache = 0;
	
	int isbnEffect = 0;
	
	CRFPP::Tagger *tagger = CRFPP::createTagger("-m model -v 3 -n2");

	//make queue full
	while(predftEnQueue(&nextCNSQ,&currentOffset,&mpredeli,refAreaEnd));
	while((pCNS = ftDeQueue(&nextCNSQ)) != NULL)
	{
		
		memcpy(samplestr,0,2048); // init
		// 0. PREPARE : FLAGS
		
		
		// 0.0 PAST ONE INFO && NEXT ONE INFO
		pCrfNodeSnapshot lpCNS = pastNElem(&preCNSQ,1);  // previous one node
		pCrfNodeSnapshot npCNS = nextNElem(&nextCNSQ,1); // next one node
		
		// 0.1 TRAVERSAL ALL PAST && NEXT INFO , GET FLAGS
		int thesisFlag = 0;
		int ltdFlag = 0;
		int edsFlag = 0;
		int uniFlag = 0;
		int mitFlag = 0;
		int groupFlag = 0;
		int pressFlag = 0;
		int confFlag = 0; // conferences
		int orgFlag = 0; // organization (as,etc.)
		int labFlag = 0; // 
		int techFlag = 0;
		int repFlag = 0;
		int procFlag = 0;
		int andFlag = 0;
		//int resFlag = 0; // research
		
		int nextPDigit = 0; // next pure digit
		int domainFlag = 0;
		int domainNoStop = 1;
		int inStatus = 0;
		int i;
		int edNoStop = 1;
		
		
		
		int stopEffect = 0; // XXXX XXXX XXXX
			// 2:  ',' 
			// 1:  '.','?','!'..  
		
		// 0.1.1 NEXT 
		for(i=1;i < sizeQueue(&nextCNSQ); i++)
		{
			pCrfNodeSnapshot tCNS = (i==0)? pCNS : nextNElem(&nextCNSQ,i);

			if(stopEffect < 2) // 0 , 1
			{
				int sw = 1+stopEffect; // stop weight 1,2
				if(tCNS->speflag == 55 ) // rep/rept == report
				{
					repFlag = sw;
				}
				if((tCNS->speflag == 50 ) // tech
					|| (tCNS->speflag == 53 ) // rfc 
					|| (tCNS->speflag == 54 )) // tr
				{
					techFlag = sw;
				}
				
				// 71: lab
				// 72: Library
				// 73: Laboratory
				// 74: Center
				// 75: institute [X?] << institute
				if(((tCNS->speflag >= 71 )&&(tCNS->speflag < 75 ))
					|| 0) // institute
				{
					labFlag = sw;
				}
				if((tCNS->speflag == 77 ) //Corporation / Co.
					|| (tCNS->speflag == 78 )) // Organiza­tion
				{
					orgFlag = sw;
				}
				
				if((tCNS->speflag == 30 ) // Press
					|| (tCNS->speflag == 31 ) // Publishing
					|| (tCNS->speflag == 32 ))  // Publisher/Pub.
				{
					pressFlag = sw;
				}
				
				if((tCNS->speflag == 90) // conference
					|| (tCNS->speflag == 79) // Journal
					|| (tCNS->speflag == 60)) // communications
				{
					confFlag = sw; // journal
				}
				
				if((tCNS->deptflag == 1)  // dept.
					||(tCNS->speflag == 56) // group
					||(tCNS->speflag == 75) //institute
					) 
				{
					groupFlag = sw; // institute
				}
				
				if(tCNS->procflag == 1) // In xxx
				{
					inStatus = sw;
				}
				if(tCNS->procflag > 1) // Proc.
				{
					procFlag = sw;
				}
				
				if(tCNS->edsflag == 1 ) edsFlag = sw;
				//if(tCNS->edsflag == 1 && edNoStop) edsFlag = sw;
				
				if(tCNS->uniflag == 1) uniFlag = sw; // un of xxx in
									// un of ... press 
				
				if(tCNS->speflag == 25) thesisFlag = sw;
				
				if(tCNS->speflag == 70) mitFlag = sw;
				
				if(tCNS->ltdflag) ltdFlag = sw;
				
				
				// name & name
				if(strcmp(tCNS->str,"and") == 0) andFlag = sw;
			}
			if(stopEffect < 1) // 0
			{
			
			}
			///////////////////////////////////////////////////////////////////////
			
			if(!isBlank(tCNS->predeli) && (tCNS->predeli !=':')
							&& (tCNS->predeli !='/')
							&& (tCNS->predeli !='.')
							&& (tCNS->predeli !=',')) // for err
			{
				domainNoStop = 0;
			}
			
			if(domainNoStop)
			{
				if((tCNS->speflag == 20 || tCNS->speflag == 21) 
					&&!isBlank(tCNS->nextdeli))
					httpStatus = 1;
				if((tCNS->speflag == 22)&&(tCNS->nextdeli !=':')) httpStatus = 1;
				if(tCNS->domainflag)
					domainFlag = 1;
			}
			
			if(tCNS->puredigit > 0 && !tCNS->yearlike) nextPDigit ++ ;
			
			// stop effect
			if(i>0)
			{
				if(tCNS->stopflag  == 1) stopEffect = (stopEffect == 2) ? 2 : 1;
				if(tCNS->stopflag  == 2) stopEffect = 2;
			}
		}
		
		// 0.1.2 PREVIOUS
		httpStatus = 0;
		int seqFlag = 0;
		
		int puredata = 1;
		
		paraFlag = 0;
		sqbFlag = 0;
		braFlag = 0;
		paraCache = 0; 
		sqbCache = 0;
		braCache = 0;
		
		stopEffect = 0;
		domainNoStop = 1;
		edNoStop = 1;
		
		// << -- | 
		for(i=0;i < sizeQueue(&preCNSQ) ; i++)
		{
			pCrfNodeSnapshot tCNS = (i==0) ? pCNS : pastNElem(&preCNSQ,i);
			
			if(i>0)
			{
				if(tCNS->stopflag  == 1) stopEffect = (stopEffect == 2) ? 2 : 1;
				if(tCNS->stopflag  == 2) stopEffect = 2;
			}
	
			if(!tCNS->puredigit && tCNS->strtype!=4) puredata = 0;
			if( puredata && tCNS->speflag == 40) isbnEffect = 1;
				
			if(!isBlank(tCNS->predeli) && (tCNS->predeli !=':')
							&& (tCNS->predeli !='/')
							&& (tCNS->predeli !='.')
							&& (tCNS->predeli !=',')) // for err
			{
				domainNoStop = 0;
			}
			
				
			
			if(domainNoStop)
			{
				if((tCNS->speflag == 20 || tCNS->speflag == 21) 
					&&!isBlank(tCNS->nextdeli))
					httpStatus = 2;
				if((tCNS->speflag == 22)&&(tCNS->nextdeli !=':')) httpStatus = 2;
			}
	
			
			////////////////////////////////////////////////////////////////////
			
			
			if(stopEffect < 2) // 0 , 1
			{
				int sw = 3+stopEffect; // stop weight 3,4
				
				if(tCNS->speflag == 55 ) // rep/rept == report
				{
					repFlag = sw;
				}
				if((tCNS->speflag == 50 ) // tech
					|| (tCNS->speflag == 53 ) // rfc 
					|| (tCNS->speflag == 54 )) // tr
				{
					techFlag = sw;
				}
				
				// 71: lab
				// 72: Library
				// 73: Laboratory
				// 74: Center
				// 75: institute [X?]
				if(((tCNS->speflag >= 71 )&&(tCNS->speflag < 75 ))
					|| 0) // institute
				{
					labFlag = sw;
				}
				if((tCNS->speflag == 77 ) //Corporation / Co.
					|| (tCNS->speflag == 78 )) // Organiza­tion
				{
					orgFlag = sw;
				}
				
				if((tCNS->speflag == 30 ) // Press
					|| (tCNS->speflag == 31 ) // Publishing
					|| (tCNS->speflag == 32 ))  // Publisher/Pub.
				{
					pressFlag = sw;
				}
				
				if((tCNS->speflag == 90) // conference
					|| (tCNS->speflag == 79) // Journal
					|| (tCNS->speflag == 60)) // communications
				{
					confFlag = sw; // journal
				}
				
				if((tCNS->deptflag == 1)  // dept.
					||(tCNS->speflag == 56) // group
					||(tCNS->speflag == 75) //institute
					) 
				{
					groupFlag = sw; // institute
				}
				
				if(tCNS->procflag == 1) // In xxx
				{
					inStatus = sw;
				}
				
				if(tCNS->edsflag == 1  && edNoStop) edsFlag = sw;
				//if(tCNS->edsflag == 1 ) edsFlag = sw;
				
				if(tCNS->uniflag == 1) uniFlag = sw; // un of xxx in
									// un of ... press 
			
				if(tCNS->procflag > 1) // Proc.
				{
					procFlag = sw;
				}
				
				if(tCNS->speflag == 25) thesisFlag = sw;
				
				if(tCNS->speflag == 70) mitFlag = sw;
				
				if(tCNS->ltdflag) ltdFlag = sw;
				
				if((strcmp(tCNS->str,"and") == 0) && (i < 2)) 
					andFlag = sw;
			}
			if(stopEffect < 1) // 0
			{
			
			}
			
			////////////////////////////////////////////////////////////////////		

			
			// couple delimiter
			if(tCNS->pareEflag) paraCache++;
			if(tCNS->pareSflag)
			{
				paraFlag = paraFlag + 1 - paraCache;
				paraCache = 0;
			}
			if(tCNS->sqbEflag) sqbCache++;
			if(tCNS->sqbSflag)
			{
				sqbFlag = sqbFlag + 1 - sqbCache;
				sqbCache = 0;
			}
			if(tCNS->braEflag) braCache++;
			if(tCNS->braSflag)
			{
				braFlag = braFlag + 1 - braCache;
				braCache = 0;
			}
			
			if(i<3 && tCNS->sqbEflag)
			{
				seqFlag = 1;
			}
			
			
			////////////////END STOP//////////////////////
			if(tCNS->puredigit == 2 )
			{
				edNoStop = 0;
			}
			// [In] xxx
			if(tCNS->procflag == 1) edNoStop = 0;
		}

		pareStatus[0] = paraFlag > 0 ;
		sqbStatus[0] = sqbFlag > 0;
		braStatus[0] = braFlag > 0;

		
		
		
		// 0.2 SOME MIX FEATURE
		// \"
		quotTime--;
		
		if(quotTime <= 0)
		{
			quotStatus[0] = 0;
			quotTime = 0;
		}else
		{
			quotStatus[0] = 1;
		}
		quotStatus[1] = 0;
		quotStatus[2] = 0;
		if(pCNS->quotflag%2 == 1)
		{
			quotStatus[0] = !quotStatus[0];
			if(quotStatus[0])
				quotTime = 10;
			else
				quotTime = -10;
		}else if(pCNS->quotflag > 0 && pCNS->quotflag%2 == 0)
		{
			quotStatus[1] = quotStatus[2] = 1;
			quotTime = 0;
		}
		if(quotTime == 10) quotStatus[1] =  1;
		else if(quotTime == -10) quotStatus[2] = 1;
		//((quotTime==10||quotTime==-10)?(quotStatus?"IN":"OUT"):(quotStatus?"AT":"NA"))
		
		
		//////////////////////////////////////////////////////////////////////
		
		pareStatus[1] = pareStatus[2] = 0;
		// para
		if(pCNS->pareSflag == 1)
		{
			pareStatus[1] = 1;
		}
		if(pCNS->pareEflag == 1)
		{
			pareStatus[2] = 1;
		}
		
		//////////////////////////////////////////////////////////////////////
		// sqb
		sqbStatus[1] = sqbStatus[2] = 0;
		if(pCNS->sqbSflag == 1)
		{
			sqbStatus[1] = 1;
		}
		
		if(pCNS->sqbEflag == 1)
		{
			sqbStatus[2] = 1;
		}
		
		
		//////////////////////////////////////////////////////////////////////
		// bra
		braStatus[1] = braStatus[2] = 0;
		if(pCNS->braSflag == 1)
		{
			braStatus[1] = 1;
		}
		
		if(pCNS->braEflag == 1)
		{
			braStatus[2] = 1;
		}
		
		// xxxx ( abcd )  or xxxx , (abcd)
		// in template
		int contentConnect = 0;
		if(pareStatus[0] == 1)
		{
			for(i=1;i < sizeQueue(&preCNSQ) ; i++)
			{
				pCrfNodeSnapshot tCNS = pastNElem(&preCNSQ,i);
				if(tCNS->predeli == '(')
				{
					tCNS = pastNElem(&preCNSQ,i+1);
					if(tCNS->nextdeli == '(') contentConnect = 1;
					break;
				}
			}
		}
		
		
		// combined string , combine with next string
		char combinedStr[1024];
		sprintf(combinedStr,"%s%s",pCNS->str,((npCNS==NULL)?"":npCNS->str));
		
		
		// 1. OUTPUT : PRINT FEATURES
		
		// basic
		// 0: string data
		fprintf(fp,"%s\t",pCNS->str); 
		
		// 1: length of string data 0,1,2,3,4,5,6 >6
		fprintf(fp,"串长/");
		fprintf(fp,"%d\t",pCNS->slen<7?pCNS->slen:9); 

		
		// base::string
		// 2: string type 0:AAA 1:aaa 2:Aaa 3:aAa 4:123
		fprintf(fp,"类型/");
		fprintf(fp,"%d\t",pCNS->strtype);
		
		// 3: prefix 
		fprintf(fp,"前缀/");
		fprintf(fp,"%c/%c\t",tolower(pCNS->str[0]),(pCNS->slen>1)?tolower(pCNS->str[1]):'X');
		
		// 4: sufix 
		fprintf(fp,"后缀/");
		fprintf(fp,"%c/%c\t",(pCNS->slen>1)?tolower(pCNS->str[pCNS->slen-2]):'X',
					tolower(pCNS->str[pCNS->slen-1]));
		
		
		// base::digit
		// 5: digit value  > 0 ?
		fprintf(fp,"数值/");
		fprintf(fp,"%d\t",pCNS->dval > 0 );
		
		// 6: digit bigger than previours one
		fprintf(fp,"比前值长/");
		fprintf(fp,"%d\t",lpCNS==NULL?-1:(lpCNS->dval == 0?-1:(pCNS->dval > lpCNS->dval)));
		
		// 7: next one is bigger than this digit 
		fprintf(fp,"比后值短/");
		fprintf(fp,"%d\t",npCNS==NULL?-1:(npCNS->dval == 0?-1:(npCNS->dval > pCNS->dval)));
		
		// 8: digit a improve digit ? 123456 456 > 123
		fprintf(fp,"升数/");
		fprintf(fp,"%d\t",pCNS->imprnum );
		
		// 9: is pure digit ? see 'I' 'l' 'O' etc. as digit
		fprintf(fp,"纯数/");
		fprintf(fp,"%d\t",pCNS->puredigit);
		
		
		// base::delimiter
		// 10: last delimiter
		fprintf(fp,"前分隔符/");
		fprintf(fp,"%d\t",pCNS->predeli);
		
		// 11: last useful delimiter
		fprintf(fp,"前有用分隔符/");
		fprintf(fp,"%d\t",pCNS->mpredeli);
		
		// 12: next delimiter
		fprintf(fp,"后分隔符/");
		fprintf(fp,"%d\t",pCNS->nextdeli);
		
		
		
		// base::string orthographic
		// 13: year like || month like >> time like
		fprintf(fp,"似年/");
		fprintf(fp,"%d\t",(pCNS->yearlike > 0 )|| (pCNS->monthlike > 0 ));
		// 14: volume like  start of volume ?  vol. X num. no. number
		fprintf(fp,"似卷/");
		if(pCNS->volumnlike < 3) fprintf(fp,"%d\t",pCNS->volumnlike);
		else if(!isBlank(pCNS->nextdeli)) fprintf(fp,"%d\t",pCNS->volumnlike-2);
		else fprintf(fp,"0\t");
			
		// 15: page like
		fprintf(fp,"似页/");
		fprintf(fp,"%d\t",pCNS->pagelike);

		
		// base::dict
		// 16: name in dict
		fprintf(fp,"字典/名/");
		fprintf(fp,"%d\t",pCNS->isNameDict || pCNS->rLastNameDict > 0);
		
		// 17: place in dict
		pCNS->isPlaceNameDict = pCNS->isPlaceNameDict || isPlaceNameInDict(combinedStr);		
		pCNS->isCountryDict = pCNS->isCountryDict || isCountryInDict(combinedStr);
		
		fprintf(fp,"字典/地/");
		fprintf(fp,"%d\t",pCNS->isPlaceNameDict>0 || pCNS->isCountryDict > 0);
		
		// 18: publisher in dict
		pCNS->isPubliserDict = pCNS->isPubliserDict || isPublisherInDict(combinedStr);
		fprintf(fp,"字典/出版社/");
		fprintf(fp,"%d\t",pCNS->isPubliserDict);
		
		// 19: fun word in dict 
		fprintf(fp,"字典/功能词汇/");
		fprintf(fp,"%d\t",pCNS->isFunWordDict);

		// base::couple flag ststus
		
		// 20,21,22 quots AT IN OUT
		fprintf(fp,"引号123/");
		fprintf(fp,"%d\t",quotStatus[0]);
		fprintf(fp,"%d\t",quotStatus[1]);
		fprintf(fp,"%d\t",quotStatus[2]);
		
		
		// 23,24,25 Parentheses AT IN OUT
		fprintf(fp,"花括号123/");
		fprintf(fp,"%d\t",pareStatus[0]);
		fprintf(fp,"%d\t",pareStatus[1]);
		fprintf(fp,"%d\t",pareStatus[2]);
		
		// 26,27,28 Square brackets AT IN OUT
		fprintf(fp,"方括号123/");
		fprintf(fp,"%d\t",sqbStatus[0]);
		fprintf(fp,"%d\t",sqbStatus[1]);
		fprintf(fp,"%d\t",sqbStatus[2]);
		
		// 29,30,31 Braces AT IN OUT
		fprintf(fp,"括号123/");
		fprintf(fp,"%d\t",braStatus[0]);
		fprintf(fp,"%d\t",braStatus[1]);
		fprintf(fp,"%d\t",braStatus[2]);
		
		// base::flags
		
		// 32 basic flags
		// special flag (mixed)
		fprintf(fp,"特殊标记/");
		fprintf(fp,"%d\t",pCNS->speflag);
		
		// 33 stop flag  && effect
		fprintf(fp,"停止/括号1/2/3/");  
		fprintf(fp,"%d/%d/%d/%d\t",pCNS->stopflag,
					(quotStatus[0]||pareStatus[0]||sqbStatus[0]||braStatus[0]),
					(quotStatus[1]||pareStatus[1]||sqbStatus[1]||braStatus[1]),
					(quotStatus[2]||pareStatus[2]||sqbStatus[2]||braStatus[2]));
		
		// 34 eds flag
		fprintf(fp,"编辑标记/");
		fprintf(fp,"%d\t",edsFlag);
		
		// 35: name like
		fprintf(fp,"似名/");
		fprintf(fp,"%d\t",pCNS->namelike);
		
		
		// extend::flags effect
		// 36 number of next pure digit
		fprintf(fp,"后面数字个数/journal/");
		fprintf(fp,"%d\t",nextPDigit);
		
		
		// 37 http effect  domain
		fprintf(fp,"HTTP态/");
		fprintf(fp,"DOMAIN态/");
		fprintf(fp,"%d/%d\t",httpStatus,domainFlag);
		
		// extend::mix effect
		// 38 [abc def] author @ abc
		fprintf(fp,"组合顺序/前/");
		if(pCNS->mpredeli == '[' && npCNS != NULL)
		{
			if(npCNS->nextdeli == ']') fprintf(fp,"1\t");
			else fprintf(fp,"0\t");
		}else
			fprintf(fp,"0\t");
		
		// [abc def] author @ def
		// 39
		fprintf(fp,"组合顺序/后/");
		if(pCNS->nextdeli == ']' && seqFlag == 1)
		{
			fprintf(fp,"1\t");
		}else
			fprintf(fp,"0\t");
		

		// 40 article xxxx, A process of ...
		fprintf(fp,"AAnTheOn/终/");
		fprintf(fp,"%d/%d\t",pCNS->isArticle,pCNS->stopflag);
		
		// 41 tech
		fprintf(fp,"TECH/");
		int phdflag = 0;
		if((strcasecmp("ph",pCNS->str)== 0 && npCNS->str[0] == 'D')
			|| strcasecmp("phD",pCNS->str)== 0)
			phdflag = 1;
		else
			phdflag = 0;

		fprintf(fp,"%d\t",phdflag);
		
		// 42 xxx thesis thesis : 25
		fprintf(fp,"Thesis/");
		fprintf(fp,"%d\t",thesisFlag);
		
		// 43 inc ltd limited  : ltdflag 1 2 3
		fprintf(fp,"Inc/Ltd/Limit/");
		fprintf(fp,"%d\t",ltdFlag);
		
		// 44 45 46 ACM / ICPC / IEEE
		fprintf(fp,"ACM/ICPC/IEEE/");
		fprintf(fp,"%d\t",pCNS->speflag == 1); // ISO
		fprintf(fp,"%d\t",pCNS->speflag == 2); // IEEE
		fprintf(fp,"%d\t",pCNS->speflag == 3); // ACM
		
		// 47 CNAC AECSA SRCD ... // contain C
		fprintf(fp,"CONF/"); 
		//fprintf(fp,"%d\t",pCNS->strtype == 0 &&  pCNS->slen < 6 && pCNS->slen > 2);
		fprintf(fp,"%d\t",conferencelike(pCNS->str,pCNS->slen));
		
		
		// 48 technical report
		fprintf(fp,"TR/");
		fprintf(fp,"%d\t",techFlag || phdflag || repFlag);
		//fprintf(fp,"%d/\t",repFlag);
		
		
		// 49 MIT
		fprintf(fp,"MIT/");
		fprintf(fp,"%d\t",mitFlag);
		
		// 50 51 52 university of 
		fprintf(fp,"学校/");
		fprintf(fp,"%d\t",pCNS->uniflag);
		
		fprintf(fp,"学校/影响/");
		fprintf(fp,"%d\t",uniFlag);
		
		//fprintf(fp,"%d/%d\t",pCNS->predeli,pCNS->uniflag);
		fprintf(fp,"终止/前一个/学校/");
		fprintf(fp,"%d/%d\t",lpCNS == NULL ? 3 : lpCNS->stopflag,pCNS->uniflag);
		
		// (August 1-2 2013)
		// how about ?
		
		// 53 xxxx , (adfadf) or xxxx (adfasf)
		fprintf(fp,"逗号隔开/");
		fprintf(fp,"%d\t",contentConnect);
		
		// rfc || request (in tr) 
		//fprintf(fp,"%d\t",rfc);
		
		// 54 55 56 57 58 59 in proceedings of
		// 54
		fprintf(fp,"In/点/");
		fprintf(fp,"%d\t",pCNS->procflag == 1); // In point
		// 55
		fprintf(fp,"In点/名字/");
		fprintf(fp,"%d\t",(pCNS->procflag == 1) && 
				((npCNS->namelike)||(npCNS->isNameDict )
					|| (npCNS->rLastNameDict > 0)));
		// 56
		fprintf(fp,"In/状态/");
		fprintf(fp,"%d\t",inStatus);
		// 57
		fprintf(fp,"In/状态/名字/");
		fprintf(fp,"%d\t",inStatus && ((pCNS->namelike)||(pCNS->isNameDict ) || (pCNS->rLastNameDict > 0)));
		
		// 58
		fprintf(fp,"In/Proc/状态/");
		fprintf(fp,"%d\t",(pCNS->procflag == 1) || procFlag);
		// 59
		fprintf(fp,"Proc/状态/");
		fprintf(fp,"%d\t",procFlag);
		
		// 60  department of / dept. of
		fprintf(fp,"Dept/点/"); 
		fprintf(fp,"%d\t",pCNS->deptflag);
		
		// 61 press
		fprintf(fp,"出版社状态/");
		fprintf(fp,"%d\t",pressFlag);
		
		
		// 62 conf / journal  
		fprintf(fp,"似期刊/");
		fprintf(fp,"%d\t",confFlag);

		
		// 63 org
		fprintf(fp,"组织|实验室/");
		fprintf(fp,"%d\t",orgFlag || labFlag);
		
		// 64 group
		fprintf(fp,"小组/");
		fprintf(fp,"%d\t",groupFlag);
		
		// 65 lib ins/pub
		fprintf(fp,"实验室/");
		fprintf(fp,"%d\t",labFlag);
		
		// 66 isbn
		fprintf(fp,"ISBN/状态/");
		fprintf(fp,"%d\t",isbnEffect);
		
		// 67 group lab or dept
		fprintf(fp,"%d\t",labFlag > 0 || groupFlag > 0 || uniFlag > 0);
		
		
		// 68 eds point
		fprintf(fp,"%d/%d\t",pCNS->edsflag,pCNS->nextdeli);// 
		
		
		// 69
		fprintf(fp,"%d/%d\t",pCNS->isPubliserDict,
				pressFlag &&(labFlag > 0 || groupFlag > 0 || uniFlag > 0) ); 
				// university , 
		
		
		// 70 et , al
		if(lpCNS != NULL && npCNS != NULL)
		{
			if((strcmp(lpCNS->str,"et")==0) && (strcmp(pCNS->str,"al")==0))
				fprintf(fp,"1\t");
			else if((strcmp(pCNS->str,"et")==0) && (strcmp(npCNS->str,"al")==0))
				fprintf(fp,"2\t");
			else 
				fprintf(fp,"0\t");
		}else
			fprintf(fp,"0\t");
		
		
		
		// 71
		// jump to note : Available ... Submitted Submitted to 
		//Unpublished Published as
		if(lpCNS != NULL && npCNS != NULL)
		{
			if ( (strcmp(pCNS->str,"Available") == 0)
				|| ((strcmp(pCNS->str,"Submitted")== 0)&&(strcmp(npCNS->str,"to")))
				|| (strcmp(pCNS->str,"Unpublished")== 0)
				|| (strcmp(pCNS->str,"Published")== 0)
				|| ((strcmp(pCNS->str,"To")== 0) 
					&& (strcmp(npCNS->str,"appear")== 0))
				)
			{
				fprintf(fp,"1\t");
			}else
				fprintf(fp,"0\t");
			
		}else
			fprintf(fp,"0\t");
		// note start
		
		
		// 72 and
		fprintf(fp,"%d\t",andFlag);
	
		// 73 endSign
		fprintf(fp,"%d\t",pCNS->stopflag);


		
		//enQueue && store
		enQueueWithDrop(&preCNSQ,*pCNS);
		predftEnQueue(&nextCNSQ,&currentOffset,&mpredeli,refAreaEnd);
	}
	
        return 1;
	
	
	
	
	return NULL;
}

pCitationNode CitationInfoPredictFile(const char *fileName,int startOffset,int endOffset)
{
	initContent();
	if(!DEBUGFLAG) setNoParse(1); //for release
	parseFile(fileName);
	
	cleanContent();
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



