#include "queue.h"
#include <stdio.h> // NULL

void clearQueue(pCNSQ Q)
{ 
    Q->front=Q->rear=0;
}

// test if the queue is empty
int testQueue(pCNSQ Q)
{
    return Q->front==Q->rear ;
}

int enQueue(pCNSQ Q,CrfNodeSnapshot e)
{
    if((Q->rear+1)%CNSQSIZE==Q->front){ // full
    	return 0;
    }else{ 
        Q->rear=(Q->rear+1)%CNSQSIZE;
        Q->data[Q->rear]=e;
        return 1;
    }
}

pCrfNodeSnapshot deQueue(pCNSQ Q)
{    
    if(testQueue(Q)){ // empty
        return NULL;
    }else {
        Q->front=(Q->front+1)%CNSQSIZE;
        return &(Q->data[Q->front]);
    }
}

pCrfNodeSnapshot nextNElem(pCNSQ Q,int N)
{
    if((Q->front+N)%CNSQSIZE >=Q->rear || N >= CNSQSIZE || N < 0){ // no such
        return NULL;
    }else {
        return &(Q->data[(Q->front+N)%CNSQSIZE]);
    }	
}

int sizeQueue(pCNSQ Q)
{
    int i=(Q->rear-Q->front+CNSQSIZE)%CNSQSIZE;
    return i;
}

