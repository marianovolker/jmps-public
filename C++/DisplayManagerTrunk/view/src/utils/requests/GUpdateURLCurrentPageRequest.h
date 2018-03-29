#ifndef GUpdateURLCurrentPageRequest_H
#define	GUpdateURLCurrentPageRequest_H

#include <QStringList>

#include "GAbsPageLoadRequest.h"

class GUpdateURLCurrentPageRequest : public GAbsPageLoadRequest
{

public:
	GUpdateURLCurrentPageRequest( unsigned int uiSessionId, bool bSynchronize, const QStringList& qtlStrListJSCalls );

    virtual ~GUpdateURLCurrentPageRequest();

    const QStringList& GetListJSCalls() const;

private:
    QStringList m_qtlStrListJSCalls;
};




inline const QStringList& GUpdateURLCurrentPageRequest::GetListJSCalls() const
{
	return m_qtlStrListJSCalls;
}

#endif

