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

// 纯大写:0?
// 纯小写:1?
// 仅首字母大写:2?
// 混杂:3?
// 纯数字4?
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
		if(lc > 0 ) return 3;
		else return 0;
	}else // hc == 0
	{
		if(lc > 0) return 1;
		else return 4;
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
	if(strcasecmp(str,"vol")==0) return 3;
	if(strcasecmp(str,"voI")==0) return 3;
	if(strcasecmp(str,"v")==0) return 3;
	if(strncasecmp(str,"volume",6)==0) return 1;
	
	if(strcasecmp(str,"number")==0) return 2;
	if(strcasecmp(str,"num")==0) return 2;
	if(strcasecmp(str,"No")==0) return 4;
	if(len == 1 && str[0] == 'n') return 4;
	
	if(strcasecmp(str,"article")==0) return 1;

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
	if(strncasecmp(str,"proceeding",len)==0) return 3;
	if(strncasecmp(str,"proceedings",len)==0) return 4;
	return 0;
}

int specialFlag(const char *str,int len)
{
	// Journal
	if(strncasecmp(str,"ISO",3)==0) return 1; // ISO/XXX TECH
	if(strncasecmp(str,"IEEE",4)==0) return 2;
	if(strncasecmp(str,"ACM",3)==0) return 3;
	
	// pages
	if(strncasecmp(str,"pp",2)==0) return 10;
	if(strcasecmp(str,"p")==0) return 11;
	if(strcasecmp(str,"pages")==0) return 12;
	if(strcasecmp(str,"page")==0) return 13;

	// url
	if(strncasecmp(str,"http",4)==0) return 20;
	if(strncasecmp(str,"ftp",4)==0) return 21;
	if(strncasecmp(str,"www",4)==0) return 22;

	
	//if(strncasecmp(str,"SIGCSE",6)==0) return 9;
	//if(strncasecmp(str,"Bulletin",8)==0) return 10;
	
	// publisher 
	if(strncasecmp(str,"Press",5)==0) return 30;
	if(strncasecmp(str,"Publishing",10)==0) return 31;
	if(strncasecmp(str,"Publisher",9)==0) return 32;


	// isbn	
	if(strncasecmp(str,"ISBN",11)==0) return 40;
	
	
	// tech
	if(strncasecmp(str,"technical",9) == 0 ) return 50;
	
	if(strncasecmp(str,"request",9) == 0 ) return 52;
	if(strcasecmp(str,"rfc") == 0 ) return 53;
	if(strcasecmp(str,"tr") == 0 ) return 54; // INSTITUTION , xxxx tr
	if(strncasecmp(str,"report",6) == 0 ) return 55; // technical report
	if(strcasecmp(str,"Thesis") == 0 ) return 25;
	
	if(strncasecmp(str,"group",5) == 0 ) return 56; // INSTITUTION xxx group


	// INSTITUTION
	if(strncasecmp(str,"research",9) == 0 ) return 51;
	if(strncasecmp(str,"communications",10) == 0 ) return 60;

	
	// tech
	
	
	if(strncasecmp(str,"MIT",3)==0) return 70;
	
	
	//institute
	if(strncasecmp(str,"lab",3)==0) return 71;
	if(strcasecmp(str,"Library") == 0 ) return 72;
	if(strcasecmp(str,"Laboratory")==0) return 73;
	if(strcasecmp(str,"Center")==0) return 74;
	
	if(strcasecmp(str,"institute")==0) return 75;
	
	if(strcasecmp(str,"association")==0) return 76;
	
	
	if(strcasecmp(str,"Corporation")==0) return 77;
	if(strcasecmp(str,"Organiza­tion")==0) return 78;
	
	if(strncasecmp(str,"Journal",7)==0) return 79;

	if(strncasecmp(str,"conference",10)==0) return 90;
	if(strcasecmp(str,"conf")==0) return 90;
	
	//
	if(strcasecmp(str,"of") == 0 ) return 80;
	
	
	
	//Comp
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
	return 0;
}

int ltdFlag(const char *str)
{
	if(strncasecmp(str,"ltd",3)==0) return 1;
	if(strncasecmp(str,"Itd",3)==0) return 1;
	if(strncasecmp(str,"inc",3)==0) return 2;
	if(strncasecmp(str,"lnc",3)==0) return 2;
	if(strcasecmp(str,"Limited")==0) return 3;
	return 0;
}


int isArticle(const char *str,int len)
{
	if(strcasecmp(str,"A")==0) return 1;
	if(strcasecmp(str,"AN")==0) return 1;
	if(strcasecmp(str,"The")==0) return 1;
	if(strcasecmp(str,"On")==0) return 2; // simial function
	return 0;
}

int domainFlag(const char *str)
{
	if(strcasecmp(str,"net")==0) return 1;
	if(strcasecmp(str,"edu")==0) return 1;
	if(strcasecmp(str,"com")==0) return 1;
	return 0;
}






