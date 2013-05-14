#include <stdio.h>
#include <string.h>

int main()
{
	FILE *fin,*fout;
	char info[1024];
	fin = fopen("ntr.txt","r");
	fout = fopen("tr.txt","w");
	if(fin == NULL || fout == NULL) return -1;
	rewind(fin);
	rewind(fout);
	while(fgets(info,1024,fin))
	{
		if(info[strlen(info)-1] == '\n') info[strlen(info)-1] = '\0';
		fprintf(fout,"<SinRef>%s</SinRef>\n",info);
	}
	fclose(fin);
	fclose(fout);
	return 0;
}


