#include <stdio.h>
#include <stdlib.h>
#include <time.h>
//A,B,C 人名:N，主:Z，宾:B  
//D,E,F 动词:V  动作:A
//G 物品O 主:B	
//H	叹词H	动作:A
//. 标点符号D	结束:E

void ZZ(FILE *fp);
void BB(FILE *fp);
void HH(FILE *fp);
void VV(FILE *fp);
void DLG(FILE *fp);
void input(FILE *fp,char val[3]);

void input(FILE *fp,char val[3])
{
	fprintf(fp,"%c\t%c\t%c\n",val[0],val[1],val[2]);
	printf("::%c\t%c\t%c\n",val[0],val[1],val[2]);
//	fflush(NULL);
}


void ZZ(FILE *fp)
{
	char val[3];
	if(rand()%4 !=3)
	{
		val[0] = rand()%3 + 'A';
		val[1] = 'N';
		val[2] = 'Z';
		input(fp,val);
	}else
	{
		DLG(fp);
	}
		
	
}

void BB(FILE *fp)
{
	char val[3];
	if((rand()%4) !=3)
	{
		val[0] = rand()%3 + 'A';
		val[1] = 'N';
		val[2] = 'B';
		input(fp,val);
	}else
	{
		//DLG(fp);
		DLG(fp);
	}
}

void HH(FILE *fp)
{
	char val[3];
	val[0] = 'H';
	val[1] = 'H';
	val[2] = 'A';
	input(fp,val);
}

void VV(FILE *fp)
{
	char val[3];
	val[0] = rand()%3 + 'D';
	val[1] = 'V';
	val[2] = 'A';
	input(fp,val);	
}


void DLG(FILE *fp)
{
	if(rand()%2)
	{
		HH(fp);
	}else
	{
		ZZ(fp);
		VV(fp);	
	}
	BB(fp);
}


int main()
{
	FILE *fp;
	int i;
	char val[3];
	srand((unsigned int)time(NULL));
	fp = fopen("train.data","w");
	
	if(fp == NULL) return -1;
	
	for(i=0;i<1000;i++)
	{
		DLG(fp);
		val[0] = '.';
		val[1] = 'D';
		val[2] = 'E';
		input(fp,val);	
	}
	fclose(fp);
	fp = fopen("test.data","w");
	for(i=0;i<1000;i++)
	{
		DLG(fp);
		val[0] = '.';
		val[1] = 'D';
		val[2] = 'E';
		input(fp,val);	
	}
	fclose(fp);
	printf("ok\n");
	return 0;
}



