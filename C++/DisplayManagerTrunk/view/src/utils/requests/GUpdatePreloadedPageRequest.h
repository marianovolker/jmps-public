#ifndef GUpdatePreloadedPageRequest_H
#define	GUpdatePreloadedPageRequest_H

#include <QStringList>

#include "GAbsPageLoadRequest.h"

class GUpdatePreloadedPageRequest : public GAbsPageLoadRequest
{

public:
    GUpdatePreloadedPageRequest( unsigned int uiSessionId, bool bSynchronize, const QStringList& qtlStrListJSCalls );
    virtual ~GUpdatePreloadedPageRequest();

    const QStringList& GetListJSCalls() const;

private:
    QStringList m_qtlStrListJSCalls;
};



inline const QStringList& GUpdatePreloadedPageRequest::GetListJSCalls() const
{
	return m_qtlStrListJSCalls;
}

#endif

