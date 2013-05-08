#include "ftGenerate.h"
#include "svm_common.h"
#include "hftctl.h"
#include "eftctl.h"
#include "eftfun.h"

#include "common.h"
#include "debuginfo.h"

#include <stdlib.h>

double line = -1.5;
double lmt = 1;
int last = 0;


MODEL *mstart,*mend;

int initFindStart()
{
	dbInit();
    
	srand((unsigned int)time(NULL));

	initFilterData();
	insertFilterData(1,"REFERENCES AND BIBLIOGRAPHY",strlen("REFERENCES AND BIBLIOGRAPHY"));
	insertFilterData(1,"REFERENCES",strlen("REFERENCES"));
	insertFilterData(0,"CONFERENCES",strlen("CONFERENCES"));
	insertFilterData(1,"BIBLIOGRAPHY",strlen("BIBLIOGRAPHY"));
	insertFilterData(1,"BIBLIOGRAPHIES",strlen("BIBLIOGRAPHIES"));
	insertFilterData(0,"AUTHOR BIOGRAPHY",strlen("AUTHOR BIOGRAPHY"));
	insertFilterData(0,"AUTHOR BIOGRAPHIES",strlen("AUTHOR BIOGRAPHIES"));

	//dirTraversal("data/",1,genStartSampleCtl);
	
	// load model
	loadModel("model/head.model",1);
	return 1;
}

int freeFindStart()
{
	cleanFilterData();
	return 1;
}

int initFindEnd()
{
	//
	cleanEndKWDContainer();
	
	//
	insertEndKWD("TABLE ");
	insertEndKWD("He is ");
	insertEndKWD("Figure ");
	insertEndKWD("Fig. ");
	insertEndKWD("In this appendix ");
	insertEndKWD("NOTICE OF ");
	insertEndKWD("He has ");
	insertEndKWD("Are there ");
	insertEndKWD("CONFIGURATION ");
	
	// load model
	loadModel("model/end.model",0);
	return 1;
}

int freeFindEnd()
{
	return 1;
}



int loadModel(const char *modelfile,int start)
{
	MODEL *model;
	char mfstr[522];
	sprintf(mfstr,"%s",modelfile);
	model=read_model(mfstr);
	if(model == NULL) return 0;
	if(start) mstart = model;
	else mend = model;
	return 1;
}

int freeModel(int start)
{
	//load modelfile
	//sprintf(modelfile,"svm_model");
	//model=read_model(modelfile);
	free_model(start?mstart:mend,1);
	return 1;
}

WORD doPredictStart(const char *fileName,MODEL *model)
{
	
	//WORD words[3];
	//MODEL *model;
	//char modelfile[200];
	// now is 299
	WORD feature[ITNUM][FTWIDE];
	long offlist[ITNUM];
	int itemnum;
	DOC *doc;
	WORD distVal,nowVal;
	
	//
	int j;
	
	distVal.weight = -10;
	distVal.wnum = -1;

	// compute weight vector
	if(model->kernel_parm.kernel_type == 0) { /* linear kernel */
		add_weight_vector_to_linear_model(model);
	}
	
	
	itemnum = genStartSampleCtlW(fileName,feature,offlist);
	
	
	//for(j=0;(feature[j]).wnum != 0;j++)
	//	if((feature[j]).wnum>model->totwords) (feature[j]).wnum=0;

	for(j=0;j<itemnum;j++)
	{
		doc = create_example(-1,0,0,0.0,create_svector(feature[j],"",1.0));
		nowVal.weight=classify_example(model,doc);
		
//		printf("~%ld,%f VS %ld,%f ",offlist[j],nowVal.weight,
//						distVal.wnum,distVal.weight);
	
		if(nowVal.weight > distVal.weight && !(nowVal.weight > 100))
		{
//			printf("\n%ld:%f => %ld:%f",distVal.wnum,distVal.weight
//					,nowVal.wnum,nowVal.weight);
			distVal.weight = nowVal.weight;
			distVal.wnum = offlist[j];
//			printf("update!");
		}
		free_example(doc,1);
//		printf("\n");
	}
	

	return distVal;
}

int all = 0;
int correct = 0;

int pp = 0;
int cc = 0;

int predictCtl(const char* fileName,int isDir)
{
	if(isDir)
	{
		//printf("ignore dir:%s\n",fileName);
		return 0;
	}
	WORD res;
	int predictedOffset = 0;
	int realoffset;
	int unavailable = 0;

	res = doPredictStart(fileName,mstart);
	
	//unavailable = res.weight < -1.5 ? 1:0;
	unavailable = res.weight < line ? 1:0;
	predictedOffset = res.wnum;
	realoffset = getREFO();
	
	//printf("real is : %d predicted is : %d",getREFO(),predictedOffset);
	
	
	all++;
	//if(!haveDiffernecesE(predictedOffset,realoffset))
		//|| !haveDiffernecesD(predictedOffset,realoffset))
	//if((ABSDIFF(predictedOffset,realoffset) < 30) ^ unavailable)
	if((ABSDIFF(predictedOffset,realoffset) < 30) || unavailable)
		correct++;
	else 
	{
		cc++;
		printf("> ");
	}
	printf("%d/%d(%f)  ",correct,all,100*(double)correct/all);
	printf(" #%d",predictedOffset);
	/*
	if(ABSDIFF(predictedOffset,realoffset) >30) 
	{
		
		printf(" ##%f",res.weight);
	}
	if(ABSDIFF(predictedOffset,realoffset) < 30 && unavailable)
	{
		printf("~%f",res.weight);
		pp++; // >> : lineplus
	}
	*/	
	printf("\n");
	return predictedOffset;
}


int getTotalFile(){return all;}
int getCorrectFile(){return correct;}

void half()
{
	lmt =  lmt <= 0.01 ? 0.01 : lmt/2;
}

void lineplus()
{
	if(last != 1)
	{
		last = 1;
		half();
	}
	line += lmt;
}

void linedec()
{
	if(last != -1)
	{
		last = -1;
		half();
	}
	line -= lmt;	
}

int adjust()
{
	if(cc>pp) lineplus();
	else if(cc<pp) linedec();
	else return 0;
	char str[200];
	sprintf(str,"echo %f >> line",line);
	system(str);
	all = 0;
	correct = 0;
	return 1;
}


