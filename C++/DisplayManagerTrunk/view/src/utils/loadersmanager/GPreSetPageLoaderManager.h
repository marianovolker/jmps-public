#ifndef GPreSetPageLoaderManager_H
#define	GPreSetPageLoaderManager_H

#include "GAbsPageLoaderManager.h"

class GSetPreloadedPagesRequest;
class GUpdatePreloadedPageRequest;
class GUpdateCurrentPageRequest;
class GAbsPageLoadRequest;
class GLoaderRequestNotify;

class GPreSetPageLoaderManager : public GAbsPageLoaderManager
{

public:
    GPreSetPageLoaderManager(unsigned int uiPageLoaderQty = 2);
    virtual ~GPreSetPageLoaderManager();

    void ProcessRequest(GSetPreloadedPagesRequest* pRequest);
    void ProcessRequest(GUpdatePreloadedPageRequest* pRequest);
    void ProcessRequest(GUpdateCurrentPageRequest* oRequest);

    void CreatePageLoaders(unsigned int uiPageLoadersQty);
    void Notify(GLoaderRequestNotify& oNotify);
    bool CanProcessRequest( GAbsPageLoadRequest* pRequest );

private:
    void UpdateStateOnNotify(GLoaderRequestNotify &oNotify);
    void UpdateStateOnNotifyForPreSetRequest(GLoaderRequestNotify &oNotify);

private:
    typedef enum
    {
    	eIdleIdle 					= 0,
    	eIdleActive 				= 1,
    	eIdleProcessing 			= 2,
    	eActiveIdle 				= 3,
    	eActiveProcessing			= 4,
    	eProcessingIdle 			= 5,
    	eProcessingActive 			= 6,
    	eProcessingProcessing		= 7

    } eState;

    eState m_eState;
};


/*
 * I A P
 *
 * I - I
 * I - A
 * I - P
 * A - I
 * A - A  can't occur
 * A - P
 * P - I
 * P - A
 * P - P
 */

#endif
