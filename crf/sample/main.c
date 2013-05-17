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
	
	//genCRFSample("data/");
	char str[30];
	int slen;

	sprintf(str,"123456");
	slen = strlen(str);
	int vh = valofdigit(str,slen/2);
	int vl = valofdigit(str+(slen/2),(slen+1)/2);
	printf("%d~%d\n",vh,vl);


	hmDictFree();


	return 0;
}

/*
// TEST OF SPILIT CONTENT

*/

