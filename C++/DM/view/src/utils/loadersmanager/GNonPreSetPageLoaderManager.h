#ifndef GNonPreSetPageLoaderManager_H
#define	GNonPreSetPageLoaderManager_H

#include "GAbsPageLoaderManager.h"

class GLoadEntirePageRequest;
class GUpdateURLCurrentPageRequest;
class GUpdateCurrentPageRequest;
class GLoaderRequestNotify;

class GNonPreSetPageLoaderManager : public GAbsPageLoaderManager
{

public:
    GNonPreSetPageLoaderManager(unsigned int uiPageLoaderQty = 1);
    virtual ~GNonPreSetPageLoaderManager();

	virtual void ProcessRequest(GLoadEntirePageRequest* pRequest);
	virtual void ProcessRequest(GUpdateURLCurrentPageRequest* oRequest);
	virtual void ProcessRequest(GUpdateCurrentPageRequest* pRequest);

	void CreatePageLoaders(unsigned int uiPageLoadersQty);
	void Notify(GLoaderRequestNotify &oNotify);
	bool CanProcessRequest(GAbsPageLoadRequest* pRequest);
	void FlushVideoPatch(bool bForcePageUnload = true);
};

#endif

