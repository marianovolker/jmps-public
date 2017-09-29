#ifndef GLoadersManagerRequestNotify_H
#define	GLoadersManagerRequestNotify_H

#include <QWebPage>

class GAbsPageLoadRequest;

class GLoadersManagerRequestNotify
{

public:
   typedef enum
	{eContentLoadComplete = 0, eContentLoadTimeout = 1 } eNotifyTypeEnum;

	GLoadersManagerRequestNotify(unsigned int uiLoadersManagerId, int uiSessionId, eNotifyTypeEnum eType = eContentLoadComplete);
    virtual ~GLoadersManagerRequestNotify();

public:
    void SetNotifyType(eNotifyTypeEnum eType);
    void SetWebPage (QWebPage *);
    void SetRequest(GAbsPageLoadRequest*);

    eNotifyTypeEnum GetNofifyType() const;
    QWebPage * GetWebPage () const;
    unsigned int GetSessionId() const;
    unsigned int GetLoadersManagerId() const;
    GAbsPageLoadRequest* GetRequest();

private:

    unsigned int m_uiLoadersManagerId;
    unsigned int m_uiSessionId;
    eNotifyTypeEnum m_eNotifyType;
    QWebPage *m_pPage;
    GAbsPageLoadRequest* m_pRequest;
};



inline void GLoadersManagerRequestNotify::SetNotifyType(GLoadersManagerRequestNotify::eNotifyTypeEnum eType)
{
    m_eNotifyType = eType;
}

inline GLoadersManagerRequestNotify::eNotifyTypeEnum GLoadersManagerRequestNotify::GetNofifyType() const
{
    return m_eNotifyType;
}

inline QWebPage * GLoadersManagerRequestNotify::GetWebPage () const
{
    return m_pPage;
}

inline void GLoadersManagerRequestNotify::SetWebPage (QWebPage *pPage)
{
	m_pPage = pPage;
}

inline unsigned int GLoadersManagerRequestNotify::GetSessionId () const
{
    return m_uiSessionId;
}

inline unsigned int GLoadersManagerRequestNotify::GetLoadersManagerId() const
{
    return m_uiLoadersManagerId;
}

inline GAbsPageLoadRequest* GLoadersManagerRequestNotify::GetRequest()
{
	return m_pRequest;
}

inline void GLoadersManagerRequestNotify::SetRequest(GAbsPageLoadRequest* pRequest)
{
	m_pRequest = pRequest;
}

#endif
