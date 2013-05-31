#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "dict.h"
#include "strHandle.h"
#include "crftctl.h"
#include "virtualcontent.h"

#include "khash.h"
#include "queue.h"
#include "citpred.h"
#include <iostream>

#include "hftctl.h"
#include "eftfun.h"

void printCitation(pCitationNode p)
{
	if(p == NULL) return ;
	int i;
	std::cout << "------------------" << std::endl;
	for(i=0;i<10;i++)
	{
		if(p->author[i][0] != 0) std::cout << "author("<<(i+1)
				<<"): " << p->author[i] << std::endl;
		else break;
	}
	
	for(i=0;i<10;i++)
	{
		if(p->editor[i][0] != 0) std::cout << "editor("<<(i+1)
				<<"): " << p->editor[i] << std::endl;
		else break;
	}
	
	
	
	printCitation(p->next);
}

int main(int argc,char *argv[])
{	
	/*
	dbConnect();
	hmDictLoad();
	dbFree();
	
	genCRFSample("data/");
	
	hmDictFree();
	*/
	const char *testfile="data/fa/A case study of file system workload in a large-scale distributed environment.txt";
	//const char *testfile="3D gadgets for business process visualization&#8212;a case study.txt";
	initContent();
	parseFile(testfile);
	int startoffset = getReferenceHeadOffset();
	int endoffset = getReferenceEndOffset();
	cleanContent();
	
	
	initCitationInfoPredict();
	
	pCitationNode p ;

	p = CitationInfoPredictFile(testfile,startoffset,endoffset);
	
	printCitation(p);
	
	freeCitationNode(&p);
	
	cleanCitationInfoPredict();
	return 0;
}

/*
// TEST OF SPILIT CONTENT

*/

