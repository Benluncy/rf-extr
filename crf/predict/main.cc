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

	CitationInfoPredictFile(testfile,startoffset,endoffset);
	
	cleanCitationInfoPredict();
	return 0;
}

/*
// TEST OF SPILIT CONTENT

*/

