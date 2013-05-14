#ifndef __STR_HANDLE_H__
#define __STR_HANDLE_H__


#define isDelimiter(a) (a==' '||a=='\"' || a == '\'' || a=='`' || a==',' || a=='.' \
			|| a=='\n' || a=='\t' || a=='\r' || a=='|' || a=='(' || a==')'\
			|| a=='<' || a=='>' || a=='[' || a==']' || a=='{' || a=='}')
			
#define isBlank(a) (a==' '|| a=='\n' || a=='\t' || a=='\r')

#define isAsciiOrDigit(a) ((a>='A' && a<='Z') || (a>='a'&&a<='z') || (a>='0'&&a<='9'))

#define isConnectCh(a) (a == '~' || a == '-' || a == ':')


int spilitContent(char *dest,int dlen,const char *src,int len,char *predeli,char *nextdeli);

#endif // __STR_HANDLE_H__

