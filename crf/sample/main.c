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
	CNSQ q;
	CrfNodeSnapshot c;
	clearQueue(&q);
	c.offset = 0;
	enQueue(&q,c);
	
	c.offset++;
	enQueue(&q,c);
	
	c.offset++;
	enQueue(&q,c);
	
	c.offset++;
	enQueue(&q,c);
	
	c.offset++;
	enQueue(&q,c);
	
	
	for(int i=1;i<5;i++)
	{
		printf("%d\n",pastNElem(&q,i).offset);
	}
	
	/*

	// load hashmap 	
	dbConnect();
	hmDictLoad();
	dbFree();
	
	genCRFSample("data/");

	hmDictFree();
	*/
	return 0;
}

/*
// TEST OF SPILIT CONTENT

*/

