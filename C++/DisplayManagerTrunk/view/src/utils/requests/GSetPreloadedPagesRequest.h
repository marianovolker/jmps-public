#ifndef GSetPreloadedPagesRequest_H
#define	GSetPreloadedPagesRequest_H

#include <QStringList>

#include "GAbsPageLoadRequest.h"

class GSetPreloadedPagesRequest : public GAbsPageLoadRequest
{

public:
    GSetPreloadedPagesRequest(	unsigned int uiSessionId, bool bSynchronize,
								const QStringList& qtlStrListJSCalls, const QStringList& qtlStrListJSLibraries, const QString& strURL );
    virtual ~GSetPreloadedPagesRequest();

    const QStringList& GetListJSCalls() const;
    const QStringList& GetListJSLibraries() const;
    const QString& GetUrl() const;

private:
    QStringList m_qtlStrListJSCalls;
    QStringList m_qtlStrListJSLibraries;
    QString m_strURL;
};



inline const QStringList& GSetPreloadedPagesRequest::GetListJSCalls() const
{
	return m_qtlStrListJSCalls;
}

inline const QStringList& GSetPreloadedPagesRequest::GetListJSLibraries() const
{
	return m_qtlStrListJSLibraries;
}

inline const QString& GSetPreloadedPagesRequest::GetUrl() const
{
	return m_strURL;
}


#endif

