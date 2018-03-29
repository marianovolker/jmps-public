#ifndef GLoaderRequestNotify_H
#define	GLoaderRequestNotify_H

#include "view/src/utils/requests/GAbsPageLoadRequest.h"

class GLoaderRequestNotify
{

public:
   typedef enum
	{eContentLoadComplete = 0, eContentLoadTimeout = 1 } eNotifyTypeEnum;

	GLoaderRequestNotify(unsigned int uiPageLoaderId, int uiSessionId, eNotifyTypeEnum eType = eContentLoadComplete);
    virtual ~GLoaderRequestNotify();

public:
    void SetNotifyType(eNotifyTypeEnum eType);
    void SetRequest(GAbsPageLoadRequest*);

    eNotifyTypeEnum GetNofifyType() const;
    unsigned int GetSessionId() const;
    unsigned int GetPageLoaderId() const;
    GAbsPageLoadRequest* GetRequest();

private:

    unsigned int m_uiPageLoaderId;
    unsigned int m_uiSessionId;
    eNotifyTypeEnum m_eNotifyType;
    GAbsPageLoadRequest* m_pRequest;
};



inline void GLoaderRequestNotify::SetNotifyType(GLoaderRequestNotify::eNotifyTypeEnum eType)
{
    m_eNotifyType = eType;
}

inline GLoaderRequestNotify::eNotifyTypeEnum GLoaderRequestNotify::GetNofifyType() const
{
    return m_eNotifyType;
}

inline unsigned int GLoaderRequestNotify::GetSessionId () const
{
    return m_uiSessionId;
}

inline unsigned int GLoaderRequestNotify::GetPageLoaderId() const
{
    return m_uiPageLoaderId;
}

inline GAbsPageLoadRequest* GLoaderRequestNotify::GetRequest()
{
	return m_pRequest;
}

inline void GLoaderRequestNotify::SetRequest(GAbsPageLoadRequest* pRequest)
{
	m_pRequest = pRequest;
}

#endif
