#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "strHandle.h"
#include "crftctl.h"
/*
void testStrSpilit()
{
	char str[20];
	int offset=0;
	int offsum = 0;
	char predeli;
	char mpredeli = ' ';
	char nextdeli;
	const char *src = "abc def ,Mic, M. XX-ab cd  -- ef xx~s  'and jjk tab\tzzz-aftend";
	//spilitContent(char *dest,int dlen,const char *src,int len)
	printf("%s\n",src);
	while((offset = spilitContent(str,20,src+offsum,strlen(src+offsum),&predeli,&nextdeli)) != 0)
	{
		offsum += offset;
		if(isBlank(predeli)) predeli = mpredeli;
		printf("string:[%s],[%c,%c]  %d-%d -- next:[%c]\n",str,predeli,
						nextdeli,offset,offsum,src[offsum]);
		//system("sleep 1");
		if(!isBlank(nextdeli)) mpredeli = nextdeli;
	}
}
*/


int main(int argc,char *argv[])
{
	//testStrSpilit();
	genCRFSample("data/");
	return 0;
}

/*
// TEST OF SPILIT CONTENT

*/

