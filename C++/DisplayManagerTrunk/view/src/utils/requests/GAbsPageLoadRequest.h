#ifndef GAbsPageLoadRequest_H
#define	GAbsPageLoadRequest_H

class GAbsPageLoadRequest
{

public:
    typedef enum
    {
		eUpdatePreloadedPageRequest,
		eUpdateCurrentPageRequest,
		eUpdateURLCurrentPageRequest,
		eSetPreloadedPagesRequest,
		eLoadEntirePageRequest,
		eUnknowType

	} eTypeOfRequest;

public:
    GAbsPageLoadRequest(unsigned int uiSessionId = 0, bool bSynchronize = false);
    virtual ~GAbsPageLoadRequest();

    unsigned int GetSessionId() const;
    bool IsSynchronize() const;
    bool HasNotification() const;
    void SetNotification( bool bNotify = true );
    eTypeOfRequest GetTypeOfRequest() const;
    unsigned int GetRequestId() const;
    void SetRequestId(unsigned int);

protected:
    unsigned int m_uiSessionId;
    bool m_bSynchronize;
    bool m_bNotify;
    eTypeOfRequest m_eTypeOfRequest;
    unsigned int m_uiRequestId;
};




inline unsigned int GAbsPageLoadRequest::GetSessionId() const
{
	return m_uiSessionId;
}

inline bool GAbsPageLoadRequest::IsSynchronize() const
{
	return m_bSynchronize;
}

inline bool GAbsPageLoadRequest::HasNotification() const
{
	return m_bNotify;
}

inline void GAbsPageLoadRequest::SetNotification(bool bNotify)
{
	m_bNotify = bNotify;
}

inline GAbsPageLoadRequest::eTypeOfRequest GAbsPageLoadRequest::GetTypeOfRequest() const
{
	return m_eTypeOfRequest;
}

inline unsigned int GAbsPageLoadRequest::GetRequestId() const
{
	return m_uiRequestId;
}

inline void GAbsPageLoadRequest::SetRequestId(unsigned int uiRequestId)
{
	m_uiRequestId = uiRequestId;
}

#endif

