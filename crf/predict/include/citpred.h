#ifndef __CIT_PREDICT_H__
#define __CIT_PREDICT_H__


typedef struct _CitationNode
{
	char author[10][50];
	int aunum;
	
	
	struct _CitationNode * next;
}CitationNode;

typedef CitationNode * pCitationNode;


#ifdef __cplusplus
extern "C"
{
#endif // __cplusplus

int initCitationNode(pCitationNode node);


#ifdef __cplusplus
}
#endif // __cplusplus


#endif // __CIT_PREDICT_H__
