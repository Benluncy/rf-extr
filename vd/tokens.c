#include "tokens.h"
#include "ini.h"
#include <string.h>
#include <stdlib.h>


static int doTokenInit(void* user, const char* section, const char* name,const char* value)
{
	struct tokens* pRepo = (struct tokens* )user;
	int id = atoi(value);
	#define MSEC(s) strcmp(section, s) == 0
	if(MSEC("tokens"))
	{
		if(id > TOKEN_MAX_LEN -1) return 0;
		sprintf(pRepo->tokens[id],strdup(name));
		pRepo->ac[id] = 1;
	}
	return 1;
}

int tokenInit()
{
	int i;
	tkRepo.max = TOKEN_MAX_LEN;
	for(i=0;i<TOKEN_MAX_LEN;i++) tkRepo.ac[i] = 0;
	if (ini_parse("token.ini", doTokenInit, &tkRepo) < 0) {
		fprintf(stderr,"Can't load 'token.ini'\n");
		return 0;
	}
	/**
	for(i = 0 ; i < tkRepo.max ; i++)
	{
		if(tkRepo.ac[i]) printf("%s=%d\n",tkRepo.tokens[i],i);
	}
	*/
	
	return 1;
}

inline char *id2Token(int id)
{
	return tkRepo.ac[id] ?  tkRepo.tokens[id] : NULL;
}

int tokenToId(const char *token)
{
	int i;
	for(i=0;i<TOKEN_MAX_LEN;i++)
		if(strcmp(tkRepo.tokens[i],token) == 0)
			return i;
	return 0;
}


//static int nowToken;

int tokenPush(unsigned int *tp,unsigned int id)
{
	unsigned int max_uint = 0-1;
	if(id == 0 || id > TOKEN_MAX_LEN) return -2;
	if((*tp )>(max_uint / TOKEN_MAX_LEN)) return -1;
	//printf("%x %x ::",*tp,id);
	*tp = *tp * TOKEN_MAX_LEN;
	*tp = *tp + id - 1;
	//printf("%x\n",*tp);
	return 1;
}

unsigned int tokenPop(unsigned int *tp)
{
	unsigned int id = *tp % (TOKEN_MAX_LEN) + 1;
	//printf("%x %x ::",*tp,id);
	*tp /= TOKEN_MAX_LEN;
	//printf("%x %x ::\n",*tp,id);
	return id;
}




