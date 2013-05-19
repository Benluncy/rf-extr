#include "crftctl.h"
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

#include <strings.h>

#define SINGLEWORDLEN 256



FILE *fpTrain; // train.txt
FILE *fpTest; // test.txt



int filteredTokenId(int offset)
{
	int mypclen = getPclen();
	unsigned int myTags;
	unsigned int nowTag;
	if(offset >= mypclen) return 0;
	myTags = *(getTags()+offset);
	int finalTag = 0;
	while((nowTag=tokenPop(&myTags)) > 0)
	{
		//ignored: 
		// note 10
		// RefA 1
		// SinRef 2
		// tech 13
		if(nowTag != 1 && nowTag != 2 && nowTag != 10 ) // ingore RefA , SinRef , note
		{
			finalTag =  nowTag;
		}	
	}		
	return finalTag;
}


int ftEnQueue(pCNSQ Q,int *currentOffset,char *mpredeli)
{
	if(isFullQueue(Q)) return 0;

	int refAreaEnd = getReferenceEndOffset();
	char *content = getPcontent();
	char str[SINGLEWORDLEN];
	
	CrfNodeSnapshot crfNodeSnapshot;
	
	int isPublisher = 0;
	
	//spilitContent(char *dest,int dlen,const char *src,int len)
	if((crfNodeSnapshot.offset = spilitContent(str,SINGLEWORDLEN,
			content+(*currentOffset),
			refAreaEnd-(*currentOffset),
			&(crfNodeSnapshot.predeli),
			&(crfNodeSnapshot.nextdeli))) != 0)
	{
		int slen = strlen(str);
		sprintf(crfNodeSnapshot.str,"%s",str);
		crfNodeSnapshot.slen = slen;
		//int dval;
		int tkcheck;
		int hstkcheck = 0;
		char partStr[1024];
		int psI = 0;
		for(int i=(*currentOffset);i<crfNodeSnapshot.offset+(*currentOffset);i++)
		{
			if(!isDelimiter(content[i]) && !hstkcheck)
			{
				tkcheck = i;
				//break;
				hstkcheck = 1;
			}
			if(DIGITLIKE(content[i]))
			{
				partStr[psI] = content[i];
				psI++;
				
			}else if(psI>0)
			{
				partStr[psI]='\0';
				isPublisher = isPublisher || isPublisherInDict(partStr); 
			}
			crfNodeSnapshot.quotflag = 0;
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
				case '{':
					crfNodeSnapshot.braSflag = 1;
					break;
				case '}':
					crfNodeSnapshot.braEflag = 1;
					break;
				case '?':
				case '.':
				case '!':
					crfNodeSnapshot.stopflag = 1;
					break;
				case ',':
					crfNodeSnapshot.stopflag = crfNodeSnapshot.stopflag == 1 ? 1: 2;
					break;
			}
		}

		crfNodeSnapshot.token = filteredTokenId(tkcheck);//offsum+(offset+1)/2
		
		*currentOffset += crfNodeSnapshot.offset;

		sprintf(crfNodeSnapshot.str,"%s",str);
		
		crfNodeSnapshot.mpredeli = isBlank(crfNodeSnapshot.predeli)?(*mpredeli):' ';
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
		crfNodeSnapshot.namelike = hasNameafterTheOffset0((*currentOffset)
							-crfNodeSnapshot.offset-1,
							crfNodeSnapshot.offset+1);
		crfNodeSnapshot.isNameDict = isNameInDict(str);
		crfNodeSnapshot.rLastNameDict = rateLastNameInDict(str);
		crfNodeSnapshot.isCountryDict = isCountryInDict(str);
		crfNodeSnapshot.isFunWordDict = isFunWordInDict(str);
		crfNodeSnapshot.isPlaceNameDict = isPlaceNameInDict(str);
		crfNodeSnapshot.isPubliserDict = isPublisherInDict(str) ||isPublisher;
		crfNodeSnapshot.isArticle = isArticle(str,slen);
		crfNodeSnapshot.deptflag = deptFlag(str);
		crfNodeSnapshot.uniflag = uniFlag(str);
		crfNodeSnapshot.ltdflag = ltdFlag(str);
		crfNodeSnapshot.domainflag = domainFlag(str);
		

		if(crfNodeSnapshot.puredigit)
		{
			int vh = valofdigit(str,slen/2);
			int vl = valofdigit(str+(slen/2),(slen+1)/2);
			if(vh == 0 || vl == 0) 	crfNodeSnapshot.imprnum = 0;
			else crfNodeSnapshot.imprnum = vl > vh ? 1 : -1;
		}else
			crfNodeSnapshot.imprnum = 0;
		
		if(!isBlank(crfNodeSnapshot.nextdeli)) *mpredeli = crfNodeSnapshot.nextdeli;
		
		//*currentOffset = crfNodeSnapshot.offset;
		enQueue(Q,crfNodeSnapshot);
		return 1;
	}else
		return 0;
}


pCrfNodeSnapshot ftDeQueue(pCNSQ Q)
{
	if(isEmptyQueue(Q)) return NULL;
	return deQueue(Q);
}

int genCRFSampleCtl(const char* fileName,int isDir)
{
	static int id = 1;
	FILE *fp; // local train
	int trainOrTest;
	int refAreaStart;
	//int refAreaEnd;
	int currentOffset;
	//char *content;
	char mpredeli=' ';
        if(isDir)
        {
                printf("ignore dir:%s\n",fileName);
                return 1;
        }
	if(rand()%2) //train is 50% and test is 50%
	{
		// train
		fp = fpTrain;
		trainOrTest = 1;
	}else
	{
		// test
		fp = fpTest;
		trainOrTest = 0;
	}
	
	CNSQ preCNSQ;
	CNSQ nextCNSQ;
	clearQueue(&preCNSQ);
	clearQueue(&nextCNSQ);
	pCrfNodeSnapshot pCNS;
	
	//
        printf("[%d] %s:%s",id,(trainOrTest?"train":"test"),fileName);
        fflush(NULL);
        
        // parse tag or etc ,move data to RAM
 	initContent();
	if(!parseFile(fileName))
	{
		fprintf(stderr,"[E] error parsing file : #%s#%s\n",fileName,__FILE__);
		return 0;
	}
	
	//
	refAreaStart = getReferenceHeadOffset();
	//refAreaEnd = getReferenceEndOffset();
	currentOffset = refAreaStart;

	//make queue full
	while(ftEnQueue(&nextCNSQ,&currentOffset,&mpredeli));
	
	
	int httpStatus = 0;
	//int httpTime = 0;
	
	int quotStatus[2];//  AT/NA IN/NIN OUT/NOUT
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

	while((pCNS = ftDeQueue(&nextCNSQ)) != NULL)
	{
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
		//int resFlag = 0; // research
		
		int nextPDigit = 0; // next pure digit
		int domainFlag = 0;
		int domainNoStop = 1;
		int i;
		
		
		
		int stopEffect = 0; // XXXX XXXX XXXX
			// 2:  ',' 
			// 1:  '.','?','!'..  
		int stopEffectEA = 0;// ExceptAbbreviation
		
		// 0.1.1 NEXT 
		for(i=1;i < sizeQueue(&nextCNSQ) ; i++)
		{
			pCrfNodeSnapshot tCNS = nextNElem(&nextCNSQ,i);
			if(tCNS->ltdflag == 1 && i < 4)
				ltdFlag = 1;
			if(tCNS->edsflag == 1)
				edsFlag = 1;
			if(tCNS->uniflag == 1 && i < 3)
				uniFlag = 1;
				
			// effect : 1:'.''?''!'  2:','	
			if(tCNS->stopflag  == 1) stopEffectEA = 1; 
			if(tCNS->stopflag  == 2 && tCNS->slen > 4) stopEffectEA = (stopEffectEA == 1) ? 1 : 2;
			
			if(tCNS->stopflag  == 1 ) stopEffect = 1; 
			if(tCNS->stopflag  == 2 ) stopEffect = (stopEffect == 1) ? 1 : 2;
			
			if(stopEffectEA == 0 && (tCNS->speflag == 56)) //"group"
			{
				groupFlag = 1; // institute
			}
			
			
			if((stopEffectEA == 0 )&& ((tCNS->speflag == 90)||
							(tCNS->speflag == 79))) //"conference(s)"
			{
				confFlag = 1; // journal
			}
			
			if((stopEffectEA == 0) && ((tCNS->speflag == 30 )
						|| (tCNS->speflag == 31 )
						|| (tCNS->speflag == 32 ))) // publisher
			{
				pressFlag = 1;
			}
			
			if((stopEffectEA == 0) && ((tCNS->speflag == 77 )
						|| (tCNS->speflag == 78 ))) // publisher||institute
			{
				orgFlag = 1;
			}
			
			if((stopEffectEA == 0) && ((tCNS->speflag >= 71 )
						&&(tCNS->speflag <= 75 ))) // institute
			{
				labFlag = 1;
			}
			
			if((stopEffectEA == 0) && ((tCNS->speflag == 50 )
						|| (tCNS->speflag == 53 )
						|| (tCNS->speflag == 54 ))) // publisher
			{
				techFlag = 1;
			}
			
			if((stopEffectEA == 0) && ((tCNS->speflag == 55 ))) // publisher
			{
				repFlag = 1;
			}
			
			if((stopEffectEA == 0) && ((tCNS->procflag > 1))) // publisher
			{
				procFlag = 1;
			}
			
			if(stopEffectEA == 0 && (tCNS->speflag == 25)) thesisFlag = 1;
			
			
			// number of pure digit except 19xx/20xx
			if(tCNS->puredigit > 0 && tCNS->yearlike) nextPDigit ++ ;
			
			if(stopEffectEA == 0 && (tCNS->speflag == 70)) mitFlag = 1;
			
			if((stopEffectEA == 0 ||  stopEffectEA == 2)&& tCNS->ltdflag) ltdFlag = 1;
			
			
			if(!isBlank(tCNS->predeli) && (tCNS->predeli !=':')
							&& (tCNS->predeli !='/')
							&& (tCNS->predeli !='.')
							&& (tCNS->predeli !=',')) // for err
			{
				domainNoStop = 0;
			}
			
			if(domainNoStop && tCNS->domainflag)
			{
				domainFlag = 1;
			}
			
		}
		
		// 0.1.2 PREVIOUS
		httpStatus = 0;
		int inStatus = 0;
		int seqFlag = 0;
		
		int puredata = 1;
		
		paraFlag = 0;
		sqbFlag = 0;
		braFlag = 0;
		paraCache = 0; 
		sqbCache = 0;
		braCache = 0;
		
		stopEffect = 0;
		stopEffectEA = 0;
		domainNoStop = 1;
		for(i=1;i < sizeQueue(&preCNSQ) ; i++)
		{
			pCrfNodeSnapshot tCNS = pastNElem(&preCNSQ,i);
			
			if(tCNS->stopflag  == 1 ) stopEffect = 1; 
			if(tCNS->stopflag  == 2 ) stopEffect = (stopEffect == 1) ? 1 : 2;
			
			if(tCNS->stopflag  == 1) stopEffectEA = 1; 
			if(tCNS->stopflag  == 2 && tCNS->slen > 4) stopEffectEA = (stopEffectEA == 1) ? 1 : 2;
	
			if(!tCNS->puredigit && tCNS->strtype!=4) puredata = 0;
			
			if(puredata && tCNS->speflag == 40) isbnEffect = 1;
				
			if(!isBlank(tCNS->predeli) && (tCNS->predeli !=':')
							&& (tCNS->predeli !='/')
							&& (tCNS->predeli !='.')
							&& (tCNS->predeli !=',')) // for err
			{
				domainNoStop = 0;
			}

			if(tCNS->speflag == 20 && domainNoStop ) httpStatus = 1;
			if(tCNS->procflag == 1 ) inStatus = 1;
			
			////////////////////////////////////////////////////////////////////
			if(stopEffectEA == 0 && (tCNS->speflag == 56)) //"group" institute
			{
				groupFlag = 2; // institute
			}
			
			
			if((stopEffectEA == 0 )&& ((tCNS->speflag == 90)||
							(tCNS->speflag == 79))) //"conference(s)"
			{
				confFlag = 2; // journal
			}
			
			if((stopEffectEA == 0) && ((tCNS->speflag == 30 )
						|| (tCNS->speflag == 31 )
						|| (tCNS->speflag == 32 ))) // publisher
			{
				pressFlag = 2;
			}
			
			if((stopEffectEA == 0) && ((tCNS->speflag == 77 )
						|| (tCNS->speflag == 78 ))) // publisher||institute
			{
				orgFlag = 2;
			}
			
			if((stopEffectEA == 0) && ((tCNS->speflag >= 71 )
						&&(tCNS->speflag <= 75 ))) // institute
			{
				labFlag = 2;
			}
			
			if((stopEffectEA == 0) && ((tCNS->speflag == 50 )
						|| (tCNS->speflag == 53 )
						|| (tCNS->speflag == 54 ))) // publisher
			{
				techFlag = 2;
			}
			
			if((stopEffectEA == 0) && ((tCNS->speflag == 55 ))) // publisher
			{
				repFlag = 2;
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
		fprintf(fp,"%d\t",pCNS->slen<7?pCNS->slen:9); 

		
		// base::string
		// 2: string type 0:AAA 1:aaa 2:Aaa 3:aAa 4:123
		fprintf(fp,"%d\t",pCNS->strtype);
		
		// 3: prefix 
		fprintf(fp,"%c%c\t",tolower(pCNS->str[0]),(pCNS->slen>1)?tolower(pCNS->str[1]):'X');
		
		// 4: sufix 
		fprintf(fp,"%c%c\t",(pCNS->slen>1)?tolower(pCNS->str[pCNS->slen-2]):'X',
					tolower(pCNS->str[pCNS->slen-1]));
		
		
		// base::digit
		// 5: digit value  > 0 ?
		fprintf(fp,"%d\t",pCNS->dval > 0 );
		
		// 6: digit bigger than previours one
		fprintf(fp,"%d\t",lpCNS==NULL?-1:(lpCNS->dval == 0?-1:(pCNS->dval > lpCNS->dval)));
		
		// 7: next one is bigger than this digit 
		fprintf(fp,"%d\t",npCNS==NULL?-1:(npCNS->dval == 0?-1:(npCNS->dval > pCNS->dval)));
		
		// 8: digit a improve digit ? 123456 456 > 123
		fprintf(fp,"%d\t",pCNS->imprnum );
		
		// 9: is pure digit ? see 'I' 'l' 'O' etc. as digit
		fprintf(fp,"%d\t",pCNS->puredigit);
		
		
		// base::delimiter
		// 10: last delimiter
		fprintf(fp,"%d\t",pCNS->predeli);
		
		// 11: last useful delimiter
		fprintf(fp,"%d\t",pCNS->mpredeli);
		
		// 12: next delimiter
		fprintf(fp,"%d\t",pCNS->nextdeli);
		
		
		
		// base::string orthographic
		// 13: year like || month like >> time like
		fprintf(fp,"%d\t",(pCNS->yearlike > 0 )|| (pCNS->monthlike > 0 ));
		// 14: volume like  start of volume ?  vol. X num. no. number
		if(pCNS->volumnlike < 3) fprintf(fp,"%d\t",pCNS->volumnlike);
		else if(!isBlank(pCNS->nextdeli)) fprintf(fp,"%d\t",pCNS->volumnlike-2);
		else fprintf(fp,"0\t");
			
		// 15: page like
		fprintf(fp,"%d\t",pCNS->pagelike);

		
		// base::dict
		// 16: name in dict
		fprintf(fp,"%d\t",pCNS->isNameDict || pCNS->rLastNameDict > 0);
		
		// 17: place in dict
		pCNS->isPlaceNameDict = pCNS->isPlaceNameDict || isPlaceNameInDict(combinedStr);		
		pCNS->isCountryDict = pCNS->isCountryDict || isCountryInDict(combinedStr);
		fprintf(fp,"%d\t",pCNS->isPlaceNameDict>0 || pCNS->isCountryDict > 0);
		
		// 18: publisher in dict
		pCNS->isPubliserDict = pCNS->isPubliserDict || isPublisherInDict(combinedStr);
		fprintf(fp,"%d\t",pCNS->isPubliserDict);
		
		// 19: fun word in dict 
		fprintf(fp,"%d\t",pCNS->isFunWordDict);

		// base::couple flag ststus
		
		// 20,21,22 quots AT IN OUT
		fprintf(fp,"%d\t",quotStatus[0]);
		fprintf(fp,"%d\t",quotStatus[1]);
		fprintf(fp,"%d\t",quotStatus[2]);
		
		
		// 23,24,25 Parentheses AT IN OUT
		fprintf(fp,"%d\t",pareStatus[0]);
		fprintf(fp,"%d\t",pareStatus[1]);
		fprintf(fp,"%d\t",pareStatus[2]);
		
		// 26,27,28 Square brackets AT IN OUT
		fprintf(fp,"%d\t",sqbStatus[0]);
		fprintf(fp,"%d\t",sqbStatus[1]);
		fprintf(fp,"%d\t",sqbStatus[2]);
		
		// 29,30,31 Braces AT IN OUT
		fprintf(fp,"%d\t",braStatus[0]);
		fprintf(fp,"%d\t",braStatus[1]);
		fprintf(fp,"%d\t",braStatus[2]);
		
		// base::flags
		
		// 32 basic flags
		// special flag (mixed)
		fprintf(fp,"%d\t",pCNS->speflag);
		
		// 33 stop flag 1:'.','!','?' 2:',' 
		fprintf(fp,"%d\t",pCNS->stopflag);
		
		// 34 eds flag
		fprintf(fp,"%d\t",edsFlag);
		
		// 35: name like
		fprintf(fp,"%d\t",pCNS->namelike);
		
		
		// extend::flags effect
		// 36 number of next pure digit
		fprintf(fp,"%d\t",nextPDigit);
		
		
		// 37 http effect
		fprintf(fp,"%d\t",httpStatus);
		
		// 38
		fprintf(fp,"%d\t",domainFlag);
		
		
		// extend::mix effect
		// 39 [abc def] author @ abc
		if(pCNS->mpredeli == '[' && npCNS != NULL)
		{
			if(npCNS->nextdeli == ']') fprintf(fp,"1\t");
			else fprintf(fp,"0\t");
		}else
			fprintf(fp,"0\t");
		
		// [abc def] author @ def
		if(pCNS->nextdeli == ']' && seqFlag == 1)
		{
			fprintf(fp,"1\t");
		}else
			fprintf(fp,"0\t");
		
		// 40 article xxxx, A process of ...
		fprintf(fp,"%d/%d\t",pCNS->isArticle,pCNS->stopflag);
		
		// 41 ph D  str cmp
		if((strcasecmp("ph",pCNS->str)== 0 && npCNS->str[0] == 'D')
			|| strcasecmp("phD",pCNS->str)== 0)
			fprintf(fp,"1\t");
		else
			fprintf(fp,"0\t");
		
		// 42 xxx thesis thesis : 25
		fprintf(fp,"%d\t",thesisFlag);
		
		// 43 inc ltd limited  : ltdflag 1 2 3
		fprintf(fp,"%d\t",ltdFlag);
		
		// 44 45 46 ACM / ICPC / IEEE
		fprintf(fp,"%d\t",pCNS->speflag == 1); // ISO
		fprintf(fp,"%d\t",pCNS->speflag == 2); // IEEE
		fprintf(fp,"%d\t",pCNS->speflag == 3); // ACM
		
		// 47 CNAC AECSA SRCD ... 
		fprintf(fp,"%d\t",pCNS->strtype == 0 &&  pCNS->slen < 6 && pCNS->slen > 2);
		
		
		// 48 technical report
		fprintf(fp,"%d\t",techFlag);
		fprintf(fp,"%d\t",repFlag);
		
		
		// 49 MIT
		fprintf(fp,"%d\t",mitFlag);
		
		// 50 51 52 university of 
		fprintf(fp,"%d\t",uniFlag);
		
		fprintf(fp,"%d\t",pCNS->uniflag);
		
		//fprintf(fp,"%d/%d\t",pCNS->predeli,pCNS->uniflag);
		fprintf(fp,"%d/%d\t",lpCNS == NULL ? 0 : lpCNS->stopflag,pCNS->uniflag);
		
		// (August 1-2 2013)
		// how about ?
		
		// 53 xxxx , (adfadf) or xxxx (adfasf)
		fprintf(fp,"%d\t",contentConnect);
		
		// rfc || request (in tr) 
		//fprintf(fp,"%d\t",rfc);
		
		// 54 55 56 57 58 59 in proceedings of
		// 54
		fprintf(fp,"%d\t",pCNS->procflag == 1); // In
		// 55
		fprintf(fp,"%d\t",(pCNS->procflag == 1) && ((npCNS->namelike)||(npCNS->isNameDict )
									|| (npCNS->rLastNameDict > 0)));
		// 56
		fprintf(fp,"%d\t",inStatus);
		// 57
		fprintf(fp,"%d\t",inStatus && ((pCNS->namelike)||(pCNS->isNameDict ) || (pCNS->rLastNameDict > 0)));
		
		// 58
		fprintf(fp,"%d\t",(pCNS->procflag == 1) && procFlag);
		// 59
		fprintf(fp,"%d\t",procFlag);
		
		// 60  department of / dept. of 
		fprintf(fp,"%d\t",pCNS->deptflag);
		
		// 61 press
		fprintf(fp,"%d\t",pressFlag);
		
		
		// 62 conf / journal
		fprintf(fp,"%d\t",confFlag);

		
		// 63 org
		fprintf(fp,"%d\t",orgFlag);
		
		// 64 group
		fprintf(fp,"%d\t",groupFlag);
		
		// 65 lib ins/pub
		fprintf(fp,"%d\t",labFlag);
		
		// 66 isbn
		fprintf(fp,"%d\t",isbnEffect);
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		// 2. END : PRINT RESULT
		if(lpCNS != NULL && npCNS != NULL)
		{
			if((lpCNS->token == 3 && npCNS->token == 3) || 
				(lpCNS->token == 6 && npCNS->token == 6))
				fprintf(fp,"%s\n",id2Token(lpCNS->token));
			else
				fprintf(fp,"%s\n",pCNS->token == 0 ? "other":id2Token(pCNS->token));	
		}else
			fprintf(fp,"%s\n",pCNS->token == 0 ? "other":id2Token(pCNS->token));
		
		//enQueue && store
		enQueueWithDrop(&preCNSQ,*pCNS);
		ftEnQueue(&nextCNSQ,&currentOffset,&mpredeli);
	}
	
	//
	id++;
        cleanContent();
        printf(" ok\n");
        return 1;
}


int genCRFSample(const char *path)
{
	FILE *fp;
	srand((unsigned int)time(NULL));
	
	//open train.data
	if((fp = fopen("res/train.data","w")) == NULL)
	{
		fprintf(stderr,"error in opening train.data\n");
		return 0;
	}
	setCRFTrainFile(fp);
	
	//open test.data
	if((fp = fopen("res/test.data","w")) == NULL)
	{
		fprintf(stderr,"error in opening train.data\n");
		return 0;
	}
	setCRFTestFile(fp);
	
	// do traversal
	dirTraversal(path,1,genCRFSampleCtl);
	
	
	//close train.data
	fclose(getCRFTrainFile());
	setCRFTrainFile(NULL);
	//close test.data
	fclose(getCRFTestFile());
	setCRFTestFile(NULL);
	return 1;
}


void setCRFTrainFile(FILE *fp){ fpTrain = fp;}
void setCRFTestFile(FILE *fp){fpTest = fp;}
FILE * getCRFTrainFile(){return fpTrain;}
FILE * getCRFTestFile(){return fpTest;}


