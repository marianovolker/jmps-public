#ifndef GNonPreSetPageLoader_H
#define	GNonPreSetPageLoader_H

#include "GAbsPageLoader.h"

class GLoadEntirePageRequest;
class GUpdateURLCurrentPageRequest;
class GUpdateCurrentPageRequest;


class GNonPreSetPageLoader : public GAbsPageLoader
{

public:
    GNonPreSetPageLoader(unsigned int uiId, GAbsPageLoaderManager *pObserver=0);
    virtual ~GNonPreSetPageLoader();

    void LoadContent( GLoadEntirePageRequest* pRequest );
    void UpdateContent( GUpdateURLCurrentPageRequest* pRequest );
    void UpdateContent( GUpdateCurrentPageRequest* pRequest );

protected:
	void DoCompleted();
	void DoTimeout();
};

#endif

