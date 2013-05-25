#include <stdio.h>

char* strLC(char *s, int type)
{
	char *str = s;
	char *res = s;
	if (type == 0)
	{
		while(*s) *str++ = toupper(*s++);
	}else{
		while(*s) *str++ = tolower(*s++);
	}
	return res;
}


int main()
{
	FILE *fi;
	FILE *fo;
	char buf[1024];
	size_t bytes_read;
	fi = fopen("Location","r");
	fo = fopen("LC_location","w");
	while ((bytes_read=fread(buf, 1, 1024, fi)) > 0) {
		fwrite(strLC(buf,1), 1, bytes_read, fo);
	}
	return 0;
}
