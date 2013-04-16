#include "eftfun.h"
#include "virtualcontent.h"
#include "persistence.h"
featureData mfd;
featureDataContainer mfdc;


int basicFilter()
{
	
	return 1;
}


//int getLastElement(int (*callback)(int offset,int limit));
	




featureDataContainer getFeatureDataContainer(void)
{
	return mfdc;
}

featureData getFeatureData(void)
{
	return mfd;
}






