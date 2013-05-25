#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "dict.h"
#include "strHandle.h"
#include "crftctl.h"
#include "khash.h"


#include "queue.h"


int main(int argc,char *argv[])
{
	//testStrSpilit();
	//dbConnect();
	//dbLock();
	//dbUnlock();
	//dbFree();
	//
	

	// load hashmap 	
	dbConnect();
	hmDictLoad();
	dbFree();
	
	genCRFSample("data/");

	hmDictFree();


	return 0;
}

/*
// TEST OF SPILIT CONTENT

*/

