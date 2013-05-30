#include <stdio.h>
char str[10];

static char * getName()
{
	int i;
	str[8]++;
	for(i=8;i>4;i--)
	{
		if(str[i]>'g')
		{
			str[i] = 'a';
			str[i-1]++;
		}
	}
	return str;

}

int main()
{
	FILE *fi;
	FILE *fo;
	int i;
	int endOfFile = 0;
	char buf[1024];
	fi = fopen("tr.txt","r");
	sprintf(str,"res/aaaaa");
	do
	{
		fo = fopen(getName(),"w");
		fprintf(fo,"<RefA>");
		for(i=0;i<13;i++)
		{
			if(fgets(buf,1024, fi) != NULL)
			{
				fprintf(fo,"%s",buf);
			}else
			{
				endOfFile = 1;
				break;
			}
		}
		fprintf(fo,"</RefA>");
		fclose(fo);
	}while(!endOfFile);
	return 0;
}



