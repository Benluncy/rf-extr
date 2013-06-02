#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "dao.h"
/*
int dbConnect();
void dbFree();
int dbLock();
int dbUnlock();

int dbCreateNameDict(const char *name);
int dbCreateValDict(const char *name);
int dbCreateTable();

int dbAddNameDict(const char *dict,const char *name);
int dbAddValDict(const char *dict,const char *name,double val);


int dbHandleGetNameDict(void *ch, int argc, char **argv, char **column);
int dbHandleGetValDict(void *ch, int argc, char **argv, char **column);

int dbGetNameDict(const char *dict,const char *name);
int dbGetNameLikeDict(const char *dict,const char *name);
double dbGetValDict(const char *dict,const char *name);
double dbGetValLikeDict(const char *dict,const char *name);
*/

void spilitVal(char *name,double *val,const char *src)
{
	int i;
	int j=0;
	int len = strlen(src);
	for(i=0;i<len;i++)
	{
		if(src[i] == '\t')
		{
			name[j]='\0';
			break;
		}
		name[j]=src[i];
		j++;
	}
	(*val)=atof(src+i+1);
}

char *removeCR(char *src)
{
	int i = strlen(src) -1;
	while(i>0)
	{
		if(src[i]==' '||src[i]=='\t'||src[i]=='\r'||src[i]=='\n')
		{
			src[i] = '\0';
			i--;
		}else
		{
			break;
		}
	}
	return src;
}

int main()
{
	FILE *fp;
	char dict[][50] = {"ChineseFamilyNames","CountryNames","FemaleFirstNames",
			"FuncWord","MaleFirstNames","Months","PlaceNames","PublisherNames","CityNames"};
	char valdict[50] = "LastNames";
	char fileName[1024];
	char data[1024];
	char name[1024];
	double val;
	dbConnect();
	
	dbLock();
	dbCreateTable();
	int i;
	for(i=0;i<9;i++)
	{
		sprintf(fileName,"res/%s",dict[i]);
		fp = fopen(fileName,"r");
		while(fgets(data,1024,fp))
		{
			dbAddNameDict(dict[i],removeCR(data));
			//printf("[%s]\n",removeCR(data));
		}
		fclose(fp);
	}
	sprintf(fileName,"res/%s",valdict);
	fp = fopen(fileName,"r");
	while(fgets(data,1024,fp))
	{
		spilitVal(name,&val,data);
		dbAddValDict(valdict,name,val);
		//printf("[%s]|[%f]\n",name,val);
	}
	dbUnlock();
	fclose(fp);
	
	//////////////////////////////////////////////////////////////
	// boolean
	printf("CityNames,abidjan:%d\n",dbGetNameLikeDict("CityNames","abigail"));
	printf("FemaleFirstNames,abigail:%d\n",dbGetNameDict("FemaleFirstNames","abigail"));
	printf("FemaleFirstNames,Abigail:%d\n",dbGetNameLikeDict("FemaleFirstNames","Abigail"));
	// double
	printf("LastNames,wilson:%f\n",dbGetValDict("LastNames","wilson"));
	printf("LastNames,Wilson:%f\n",dbGetValLikeDict("LastNames","Wilson"));
	//////////////////////////////////////////////////////////////
	
	dbFree();
	
	return 0;
}
