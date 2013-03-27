#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "ini.h"

typedef struct
{
    int version;
    const char* name;
    const char* email;
} configuration;

int v;
char n[1024];
char e[1024];

static int handler(void* user, const char* section, const char* name,
                   const char* value)
{
	configuration* pconfig = (configuration*)user;

/*
    #define MATCH(s, n) strcmp(section, s) == 0 && strcmp(name, n) == 0
    if (MATCH("protocol", "version")) {
        pconfig->version = atoi(value);
    } else if (MATCH("user", "name")) {
        pconfig->name = strdup(value);
    } else if (MATCH("user", "email")) {
        pconfig->email = strdup(value);
    } else {
        return 0;  // unknown section/name, error
    }
*/
	printf("section : [%s] - name : [%s] value: [%s]\n",section,name,value);
	pconfig->version = v++;
	sprintf(n,"%s:name",n);
	sprintf(e,"%s:email",e);
	pconfig->name = strdup(n);
	pconfig->email = strdup(e);

	return 1;
}

int main(int argc, char* argv[])
{
    configuration config;

//int ini_parse(const char* filename,
//              int (*handler)(void* user, const char* section,
//                             const char* name, const char* value),
//              void* user);

// Same as ini_parse(), but takes a FILE* instead of filename. 
// This doesn't close the file when it's finished 
// -- the caller must do that. 
//int ini_parse_file(FILE* file,
//                   int (*handler)(void* user, const char* section,
//                                  const char* name, const char* value),
//                   void* user);
	v = 0;
	memset(n,0,1024);
	memset(e,0,1024);

    if (ini_parse("test.ini", handler, &config) < 0) {
        printf("Can't load 'test.ini'\n");
        return 1;
    }
    printf("Config loaded from 'test.ini': version=[%d], name=[%s], email=[%s]\n",
        config.version, config.name, config.email);
    return 0;
}


