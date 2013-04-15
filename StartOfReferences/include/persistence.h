#ifndef __PERSISTENCE_H__
#define __PERSISTENCE_H__

#ifndef CallackLen
#define CallackLen 8
#endif // CallackLen

typedef struct sFeatureData
{
	int qid;
	//char fileName[1024];	
	int t[5];
	int positive;
	int offset;
	int adjacencyOffset[2]; //0: prev 1:next 
	int fid[CallackLen][2];//feature info data fid[x][0] == beforeData fid[x][1] == totalData
	//fid means all collected key words before && total of the offset
	int seq[CallackLen][3];//6:method 2:0)all 1)prev&&next 2)beforeXXX(int)  
	double density[CallackLen][3][2];//6:method 3: all prev&next(kwd) prev&next(100*words)  2: prev next
} featureData;



typedef struct
{
	int top;
	featureData data[200];
} featureDataContainer;


//init
int dbInit(void);
int doCreateTable(void);


//int doGetFeatureData(void *a_param, int argc, char **argv, char **column);
//int doGetFeatureDataInfo(void *a_param, int argc, char **argv, char **column);

int insertFeatureInfo(const char *fileName,int num);
int insertFeature(const char* fileName,featureData data);
int cleanFeature(const char* fileName);
int getFeature(const char *fileName,featureDataContainer *data);

char *queryEscape(const char *str);

#endif // __PERSISTENCE_H__

