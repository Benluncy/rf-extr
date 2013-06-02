#include "dao.h"
#include "sqlite3.h"
#include <stdio.h>
#include <stdlib.h>

const char *dbfile = "db/dict.db";
typedef sqlite3 * dbsrc;
dbsrc msrc;

int dbCreateNameDict(const char *name)
{
	//id integer  primary key,
	char query[300];
	sprintf(query,"create table %s(id integer primary key,name varchar(100) unique)",name);
	char *emsg;
	//printf("%s\n",query);
	sqlite3_exec(msrc,query,0,0,&emsg);
	if(emsg != NULL) fprintf(stderr,"E: %s",emsg);
	return 0;
}

int dbCreateValDict(const char *name)
{
	//id integer  primary key,
	char query[300];
	sprintf(query,"create table %s(id integer primary key,name varchar(100) unique,val double)",name);
	char *emsg;
	//printf("%s\n",query);
	sqlite3_exec(msrc,query,0,0,&emsg);
	if(emsg != NULL) fprintf(stderr,"E: %s",emsg);
	return 0;
}

int dbCreateTable()
{
	dbCreateNameDict("ChineseFamilyNames");
	dbCreateNameDict("CountryNames");
	dbCreateNameDict("FemaleFirstNames");
	dbCreateNameDict("MaleFirstNames");
	dbCreateNameDict("FuncWord");
	dbCreateNameDict("Months");
	dbCreateNameDict("PlaceNames");
	dbCreateNameDict("PublisherNames");
	dbCreateNameDict("CityNames");
	dbCreateValDict("LastNames");
	return 0;
}


int dbConnect()
{
	// open db
	if(sqlite3_open(dbfile,&msrc) != 0) 
		fprintf(stderr,"E:db connect refuese (%d)\n",sqlite3_errcode(msrc));	
	return 1;
}

int dbLock()
{
	//return sqlite3_exec(msrc,"BEGIN TRANSACTION",0,0,0);
	return sqlite3_exec(msrc,"BEGIN",0,0,0);
}

int dbUnlock()
{
	return sqlite3_exec(msrc,"COMMIT",0,0,0);
}


int dbAddNameDict(const char *dict,const char *name)
{
	char query[1024];
	sprintf(query,"insert into %s(name) values(\'%s\')",dict,sqlite3_mprintf("%q",name));
	return sqlite3_exec(msrc,query,0,0,0);
}

int dbAddValDict(const char *dict,const char *name,double val)
{
	char query[1024];
	sprintf(query,"insert into %s(name,val) values(\'%s\',%f)",dict,sqlite3_mprintf("%q",name),val);
	return sqlite3_exec(msrc,query,0,0,0);
}


int dbHandleGetNameDict(void *ch, int argc, char **argv, char **column)
{
	int *ich = (int *)ch;
	(*ich) = 1;
	//printf("## ch is :%d|%d|%s\n",*ich,atoi(argv[0]),column[0]);
	return 0;
}


int dbHandleGetValDict(void *ch, int argc, char **argv, char **column)
{
	double *vch = (double *)ch;
	(*vch) = atof(argv[0]);
	//printf("##ch is :%f\n",*((double *)ch));
	return 0;
}


int dbGetNameDict(const char *dict,const char *name)
{
	int ch = 0;
	char query[1024];
	char *e;
	sprintf(query,"select * from %s where name = \'%s\'",dict,sqlite3_mprintf("%q",name));
	//printf("query:[%s]\n",query);
	sqlite3_exec(msrc,query,dbHandleGetNameDict,&ch,&e);
	if(e != NULL)
		fprintf(stderr,"E:%d:%s\n%s::%d!\n",sqlite3_errcode(msrc),e,__FILE__,__LINE__);
	return ch;
}

int dbGetNameLikeDict(const char *dict,const char *name)
{
	int ch = 0;
	char query[1024];
	char *e;
	sprintf(query,"select * from %s where name like \'%s\'",dict,sqlite3_mprintf("%q",name));
	//printf("query:[%s]\n",query);
	sqlite3_exec(msrc,query,dbHandleGetNameDict,&ch,&e);
	if(e != NULL)
		fprintf(stderr,"E:%d:%s\n%s::%d!\n",sqlite3_errcode(msrc),e,__FILE__,__LINE__);
	return ch;
}


double dbGetValDict(const char *dict,const char *name)
{
	double val = 0;
	char query[1024];
	char *e;
	sprintf(query,"select val from %s where name = \'%s\'",dict,sqlite3_mprintf("%q",name));
	//printf("query:[%s]\n",query);
	sqlite3_exec(msrc,query,dbHandleGetValDict,&val,&e);
	if(e != NULL)
		fprintf(stderr,"E:%d:%s\n%s::%d!\n",sqlite3_errcode(msrc),e,__FILE__,__LINE__);
	return val;
}

double dbGetValLikeDict(const char *dict,const char *name)
{
	double val = 0;
	char query[1024];
	char *e;
	sprintf(query,"select val from %s where name like \'%s\'",dict,sqlite3_mprintf("%q",name));
	//printf("query:[%s]\n",query);
	sqlite3_exec(msrc,query,dbHandleGetValDict,&val,&e);
	if(e != NULL)
		fprintf(stderr,"E:%d:%s\n%s::%d!\n",sqlite3_errcode(msrc),e,__FILE__,__LINE__);
	return val;
}


void dbFree()
{
	sqlite3_close(msrc);
}


