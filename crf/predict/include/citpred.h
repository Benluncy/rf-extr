#ifndef __CIT_PREDICT_H__
#define __CIT_PREDICT_H__


typedef struct _CitationNode
{
	char author[10][50];
	char title[50];
	char editor[10][50];
	char journal[50];
	char booktitle[50];
	char publisher[50];
	char institution[50];
	char volume[50];
	char location[50];
	char pages[50]; // TODO : may int pages[2];?
	char date[50];
	char url[50];
	char isbn[50];
	char tech[50];
	char other[50];
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
