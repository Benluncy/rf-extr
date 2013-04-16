#include <stdio.h>
#include <stdlib.h> // for system xxx

#define MATCH(a,b) (strcmp(a,b) == 0)

void remove_file(void)
{
	unlink("efgh");
	unlink("abcd");

}


void touch_file(void)
{
	system("touch abcd");
}

void do_rename(void)
{
	rename("abcd","efgh");
}

int main(int argc ,char *argv[])
{
	int i;
	if(argc == 0)
	{
		remove_file();
		touch_file();
		do_rename();
	}else
	{
			
		for(i=0;i<argc;i++)
		{
			if(MATCH(argv[i],"remove")) remove_file();
			if(MATCH(argv[i],"touch")) touch_file();
			if(MATCH(argv[i],"rename")) do_rename(); 
		}
	
	}
	return 0;
}

