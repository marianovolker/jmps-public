#ifndef GUpdateCurrentPageRequest_H
#define	GUpdateCurrentPageRequest_H

#include <QStringList>

#include "GAbsPageLoadRequest.h"

class GUpdateCurrentPageRequest : public GAbsPageLoadRequest
{

public:
    GUpdateCurrentPageRequest( unsigned int uiSessionId, bool bSynchronize, const QStringList& qtlStrListJSCalls );

    virtual ~GUpdateCurrentPageRequest();

    const QStringList& GetListJSCalls() const;

private:
    QStringList m_qtlStrListJSCalls;
};




inline const QStringList& GUpdateCurrentPageRequest::GetListJSCalls() const
{
	return m_qtlStrListJSCalls;
}

#endif

