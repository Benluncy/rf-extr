#include "hftnameandpp.h"
#include "minEditDistance.h"

inline isPageNumber(const char *content)
{
	if()
}
/**
 * int offset : offset of pcontent
 * int limit : 
 *
 */
int hasPPafterTheOffset(int offset,int limit)
{
	int i;
	int offend = offset + limit;
	char *content = getPcontent();
	for(i=offset;i<offend;i++)
	{
		if(content[i]=='p' || content[i] == 'P')
		{
			if(content[i+1] == 'p'  || content[i] == 'P')
			{
				
			}else if(editDistanceS("page",content+i,4) <= 1) // threshold == 1
			{
				
			}
		}
	}
	return 0;
}
