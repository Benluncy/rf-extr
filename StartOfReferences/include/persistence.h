#ifndef __PERSISTENCE_H__
#define __PERSISTENCE_H__

typedef struct sFeatureData
{
	int qid;
	//char fileName[1024];	
	int t[5];
	int positive;
	int offset;
} featureData;

typedef struct
{
	int top;
	featureData data[20];
} featureDataContainer;


//init
int dbInit(void);
int doCreateTable(void);


//int doGetFeatureData(void *a_param, int argc, char **argv, char **column);
//int doGetFeatureDataInfo(void *a_param, int argc, char **argv, char **column);

int insertFeatureInfo(const char *fileName,int num);
int insertFeature(const char* fileName,featureData data);
int getFeature(const char *fileName,featureDataContainer *data);

char *queryEscape(const char *str);

#endif // __PERSISTENCE_H__
