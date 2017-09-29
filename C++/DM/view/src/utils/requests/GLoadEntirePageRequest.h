#ifndef GLoadEntirePageRequest_H
#define	GLoadEntirePageRequest_H

#include <QStringList>

#include "GAbsPageLoadRequest.h"

class GLoadEntirePageRequest : public GAbsPageLoadRequest
{

public:
    GLoadEntirePageRequest(	unsigned int uiSessionId, bool bSynchronize, bool bApplyZoomFactor,
							const QStringList& qtlStrListJSCalls, const QStringList& qtlStrListJSLibraries, const QString& strURL );

    virtual ~GLoadEntirePageRequest();


    const QStringList& GetListJSCalls() const;
    const QStringList& GetListJSLibraries() const;
    const QString& GetUrl() const;
    bool GetApplyZoom() const;

private:

    bool m_bApplyZoomFactor;
    QStringList m_qtlStrListJSCalls;
    QStringList m_qtlStrListJSLibraries;
    QString m_strURL;

};

inline const QStringList& GLoadEntirePageRequest::GetListJSCalls() const
{
	return m_qtlStrListJSCalls;
}

inline const QStringList& GLoadEntirePageRequest::GetListJSLibraries() const
{
	return m_qtlStrListJSLibraries;
}

inline const QString& GLoadEntirePageRequest::GetUrl() const
{
	return m_strURL;
}

inline bool GLoadEntirePageRequest::GetApplyZoom() const
{
	return m_bApplyZoomFactor;
}


#endif

