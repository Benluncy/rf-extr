#include "ftGenerate.h"
#include "svm_common.h"
#include "hftctl.h"
#include "eftctl.h"

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
}

int freeFindStart()
{
	cleanFilterData();
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
	model=read_model(modelfile);
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

WORD doPredict(const char *fileName,MODEL *model)
{
	DOC *doc;
	//WORD words[3];
	// now is 299
	WORD feature[302];
	MODEL *model;
	char modelfile[200];
	WORD distVal,nowVal;
	
	distVal.weight = -10;
	distVal.wnum = -1;

	// compute weight vector
	if(model->kernel_parm.kernel_type == 0) { /* linear kernel */
		add_weight_vector_to_linear_model(model);
	}
	
	for(j=0;(features[j]).wnum != 0;j++)
		if((features[j]).wnum>model->totwords) (features[j]).wnum=0;

	doc = create_example(-1,0,0,0.0,create_svector(features,"",1.0));
	nowVal.weight=classify_example(model,doc);
		
	free_example(doc,1);


	//printf("dest is :%f",dist);
	return distVal;
}



int predictStartOffset(const char *fileName)
{
	//dirTraversal("data/",1,genStartSampleCtl);
	return 1;
}

int predictEndOffset(const char *fileName)
{
	//dirTraversal("data/",1,genEndSampleCtl);
	return 1;
}


int predictCtl(const char* fileName,int isDir)
{
	if(isDir)
	{
		//printf("ignore dir:%s\n",fileName);
		return 1;
	}
	WORD res;
	int predictedOffset;
	res = doPredict(fileName,mstart);
	if(res.weight > -0.5) predictedOffset = res.wnum;
	
	return 1;
}



