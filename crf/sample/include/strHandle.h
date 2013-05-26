#ifndef __STR_HANDLE_H__
#define __STR_HANDLE_H__


#define isDelimiter(a) (a==' '||a=='\"' || a == '\'' || a=='`' || a==',' || a=='.' \
			|| a=='\n' || a=='\t' || a=='\r' || a=='|' || a=='(' || a==')'\
			|| a=='<' || a=='>' || a=='[' || a==']' || a=='{' || a=='}' \
			|| a=='/' || a=='$' || a=='#' || a=='^' || a=='&' || a=='*'||a=='@'\
			|| a=='!' || a=='?' || a==';' || a==':' || a=='|' || a=='\\' || a=='_'\
			|| a=='+')
			
#define isBlank(a) (a==' '|| a=='\n' || a=='\t' || a=='\r')

#define isAsciiOrDigit(a) ((a>='A' && a<='Z') || (a>='a' && a<='z') || (a>='0' && a<='9'))

#define isAsciiCode(a) ((a>='A' && a<='Z') || (a>='a' && a<='z'))

#define isUppercaseCode(a) (a>='A' && a<='Z')
#define isLowercaseCode(a) (a>='a' && a<='z')

#define isConnectCh(a) (a == '~' || a == '-' || a == ':')

#define DIGITLIKE(x) ((x<='9'&&x>='0')||x=='l'||x=='I'||x=='i'||x=='o'||x=='O')

#define VALDIG(x) ((x<='9'&&x>='0')?(x-'0'):((x=='I'||x=='i'||x=='l'||x=='['||x==']')?1:((x=='o'||x=='O')?0:-1)))


int spilitContent(char *dest,int dlen,const char *src,int len,char *predeli,char *nextdeli);

//type :1 low case  :0 big
char* strCaseTransfer(char *s, int type);
#endif // __STR_HANDLE_H__

