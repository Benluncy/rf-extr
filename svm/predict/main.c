/**
 *
 * The MIT License
 * Copyright (C) 2013 Yu Jing <yujing5b5d@gmail.com>
 *
 * Permission is hereby granted, free of charge, to any person obtaining
 * a copy of this software and associated documentation files (the "Software"),
 * to deal in the Software without restriction, including without limitation
 * the rights to use, copy, modify, merge, publish, distribute,sublicense,
 * and/or sell copies of the Software, and to permit persons to whom the
 * Software is furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included
 * in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS
 * OR IMPLIED,INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL
 * THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE,ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 *
 */
/**
 * Module Name :
 *      main.c
 *
 * Abstract :
 *      predict the start and end of references' offset
 *
 * Author :
 *      Yu Jing
 *
 * Last Updated :
 *      2013-5-06
 *
 **/

#include "svm_common.h"
#include "ftGenerate.h"
#include "dirTraversal.h"
#include <stdio.h>

const char *testfile="data/orbz/Analysis of algorithms, a case study.txt";



int main (int argc, char* argv[])
{
	//int offset;
	initFindStart();
	initFindEnd();
	int total;
	int correct;

	do
	{
		dirTraversal("data/",1,predictCtl);
	}while(adjust());
	//offset = predictCtl(testfile,0);
	
	total = getTotalFile();
	correct = getCorrectFile();
	
	printf(" %d:%d %f\n",correct,total,(double)correct/total);
	
	//printf("offset:%d\n",offset);
	
	freeFindStart();
	freeFindEnd();
	return 0;
}


/*
int main (int argc, char* argv[])
{
	DOC *doc;
	WORD words[3];
	char *comment = "";
	MODEL *model;
	char modelfile[200];
	double dist;
	WORD distVal;
	//load modelfile
	sprintf(modelfile,"svm_model");
	
	model=read_model(modelfile);

	words[0].wnum = 1;
	words[0].weight = 1;
	
	words[1].wnum = 2 ;
	words[1].weight = 0;
	
	words[2].wnum = 0;
	
	// compute weight vector
	if(model->kernel_parm.kernel_type == 0) { // linear kernel 
		add_weight_vector_to_linear_model(model);
	}
	
	for(j=0;(words[j]).wnum != 0;j++)
		if((words[j]).wnum>model->totwords) (words[j]).wnum=0;

	doc = create_example(-1,0,0,0.0,create_svector(words,comment,1.0));
	dist=classify_example(model,doc);
		
	free_example(doc,1);

	free_model(model,1);

	printf("dest is :%f",dist);

	return(0);
}
*/



