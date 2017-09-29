#ifndef GPreSetPageLoader_H
#define	GPreSetPageLoader_H

#include "GAbsPageLoader.h"

class GSetPreloadedPagesRequest;
class GUpdatePreloadedPageRequest;
class GUpdateCurrentPageRequest;

class GPreSetPageLoader: public GAbsPageLoader
{

public:
	GPreSetPageLoader(unsigned int uiId, GAbsPageLoaderManager *pObserver=0);
	virtual ~GPreSetPageLoader();

	void LoadContent( GSetPreloadedPagesRequest* pRequest );
	void UpdateContent( GUpdatePreloadedPageRequest* pRequest );
	void UpdateContent( GUpdateCurrentPageRequest* pRequest );

protected:
	void DoCompleted();
	void DoTimeout();

protected:
	bool m_bPartialProcessing;
};

#endif

