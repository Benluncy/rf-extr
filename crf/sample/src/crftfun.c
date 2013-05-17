#include "crftfun.h"
#include "minEditDistance.h"
#include "strHandle.h"
#include <string.h>
#include <strings.h>


// digits ..
// digit base
int digitlen(const char *str,int len)
{
	int i;
	int sum = 0;
	for(i=0;i<len;i++)
	{
		if(DIGITLIKE(str[i]))
		{
			sum++;
		}
	}
	return sum;
}


int puredigit(const char *str,int len)
{
	int i;
	int sum = 0;
	for(i=0;i<len;i++)
	{
		if(DIGITLIKE(str[i]))
		{
			sum++;
		}
	}
	if(sum == 0 ) return 0;
	if(sum < len ) return 1;
	return 2;
}

int valofdigit(const char *str,int len)
{
	int i;
	int sum = 0;
	for(i=0;i<len;i++)
	{
		if(VALDIG(str[i]) == -1)
		{
			if(len - i == 2)
			{
				return sum;
			}
			return 0;
		}
		sum = sum * 10 + VALDIG(str[i]);
	}

	return sum;
}

// digit special
int yearlike(const char *str,int len)
{
	if((str[0]=='1' && str[1]=='9')||(str[0]=='2' && str[1]=='0'))
	{
		if(DIGITLIKE(str[2]) && DIGITLIKE(str[3])) return 1;
	}
	return 0;
}





// strings ..
// str base

// 纯大写:0?纯小写:1?仅首字母大写:2?混杂:4?其他(纯数字)5
int strfeature(const char *str,int len) 
{
	int hc = 0;
	int lc = 0;
	int i;
	int fl = (str[0] <='Z' && str[0] >='A')?1:0;//首字母大写
	if(fl) hc++;
	else lc ++;
	
	for(i=1;i<len;i++)
	{
		if(str[0] <='Z' && str[0] >='A')
		{
			hc++;
			fl = 0;
		}
		if(str[0] <='z' && str[0] >='a')
		{
			lc++;
		}
	}
	
	if(fl) return 2;
	
	if(hc > 0)
	{
		if(lc > 0 ) return 4;
		else return 0;
	}else // hc == 0
	{
		if(lc > 0) return 1;
		else return 5;
	}
	
}


// str special
int monthlike(const char *str,int len)
{
	int j;
	char kwds[12][10]={"January","February","March","April","May","June","July",
			"August","September","October","November","December"};
	for(j=0;j<12;j++)
	{
		if(strncasecmp(str,kwds[j],strlen(kwds[j])) == 0) return 1;
		if((strncasecmp(kwds[j],str,3) == 0)) return 2;
	}
	if(strncasecmp("Sept",str,4) == 0) return 2;

	return 0;
}



int vollkwd(const char *str,int len)
{
	if(strncasecmp(str,"vol",3)==0) return 1;
	if(strncasecmp(str,"num",3)==0) return 2;
	if(strncasecmp(str,"No",2)==0) return 2;
	if(len == 1 && str[0] == 'n') return 2;
	return 0;
}

int pagekwd(const char *str,int len)
{
	if(strncasecmp(str,"pp",2)==0) return 1;
	if(strcasecmp(str,"p")==0) return 1;
	if(strncasecmp(str,"page",4)==0) return 1;
	return 0;
}



int edsFlag(const char *str,int len)
{
	if(strncasecmp(str,"ed",len)==0) return 1;
	if(strncasecmp(str,"cd",len)==0) return 1;
	if(strncasecmp(str,"eds",len)==0) return 1;
	if(strncasecmp(str,"editor",len)==0) return 1;
	if(strncasecmp(str,"editors",len)==0) return 1;
	return 0;
}

int procFlag(const char *str,int len)
{
	if(strncasecmp(str,"in",len)==0) return 1;
	if(strncasecmp(str,"proc",len)==0) return 2;
	if(strncasecmp(str,"proceeding",len)==0) return 2;
	if(strncasecmp(str,"proceedings",len)==0) return 2;
	return 0;
}

int specialFlag(const char *str,int len)
{
	if(strncasecmp(str,"ISO",3)==0) return 1;
	if(strncasecmp(str,"IEEE",4)==0) return 2;
	if(strncasecmp(str,"ACM",3)==0) return 3;
	//if(strncasecmp(str,"Inc",3)==0) return 4;
	if(strcasecmp(str,"Proc")==0) return 5;
	
	if(strncasecmp(str,"pp",2)==0) return 6;
	if(strcasecmp(str,"p")==0) return 6;
	if(strcasecmp(str,"pages")==0) return 6;
	if(strcasecmp(str,"page")==0) return 6;

	if(strncasecmp(str,"http",4)==0) return 7;
	if(strncasecmp(str,"www",4)==0) return 7;

	if(strncasecmp(str,"ftp",4)==0) return 7;
	if(strncasecmp(str,"No",2)==0) return 8;
	
	if(strncasecmp(str,"SIGCSE",6)==0) return 9;
	if(strncasecmp(str,"Bulletin",8)==0) return 10;
	if(strncasecmp(str,"Press",5)==0) return 11;
	if(strncasecmp(str,"PROCEEDING",10)==0) return 12;
	
	if(strcasecmp(str,"In")==0) return 13;
	if(strncasecmp(str,"ISBN",11)==0) return 14;
	
	if(strncasecmp(str,"technical",9) == 0 ) return 15;//
	if(strncasecmp(str,"Research",9) == 0 ) return 15;//
	if(strncasecmp(str,"Request",9) == 0 ) return 15;
	if(strcasecmp(str,"RFC") == 0 ) return 15;
	if(strncasecmp(str,"TR",2) == 0 ) return 15;
	if(strncasecmp(str,"report",6) == 0 ) return 16;

	if(strncasecmp(str,"communications",10) == 0 ) return 20;

	if(strncasecmp(str,"group",5) == 0 ) return 25;
	
	if(strcasecmp(str,"Thesis") == 0 ) return 25;
	if(strcasecmp(str,"pH") == 0 ) return 25;
	if(strcasecmp(str,"pHD") == 0 ) return 25;
	if(strcasecmp(str,"D") == 0 ) return 25;
	
	if(strncasecmp(str,"MIT",3)==0) return 40;
	
	
	
	if(strcasecmp(str,"Library") == 0 ) return 45;
	if(strcasecmp(str,"Laboratory")==0) return 45;
	if(strcasecmp(str,"Center")==0) return 45;
	
	if(strcasecmp(str,"institute")==0) return 45;
	
	
	if(strncasecmp(str,"Journal",7)==0) return 50;

	//if(strcasecmp(str,"of") == 0 ) return 17;
	
	//Corporation
	return 0;
}

int deptFlag(const char *str)
{
	
	if(strncasecmp(str,"dept",4)==0) return 1;
	if(strcasecmp(str,"Department")==0) return 1;
	return 0;
}


int uniFlag(const char *str)
{
	if(strncasecmp(str,"university",10)==0) return 1;
	if(strncasecmp(str,"lab",3)==0) return 1;
	
	return 0;
}

int ltdFlag(const char *str)
{
	if(strncasecmp(str,"ltd",3)==0) return 1;
	if(strncasecmp(str,"Itd",3)==0) return 1;
	if(strncasecmp(str,"inc",3)==0) return 1;
	if(strncasecmp(str,"lnc",3)==0) return 1;
	if(strcasecmp(str,"Limited")==0) return 1;

	return 0;
}


int isArticle(const char *str,int len)
{
	if(strcasecmp(str,"A")==0) return 1;
	if(strcasecmp(str,"AN")==0) return 1;
	if(strcasecmp(str,"The")==0) return 1;
	return 0;
}

int domainFlag(const char *str)
{
	if(strcasecmp(str,"net")==0) return 1;
	if(strcasecmp(str,"edu")==0) return 1;
	if(strcasecmp(str,"com")==0) return 1;
	return 0;
}


