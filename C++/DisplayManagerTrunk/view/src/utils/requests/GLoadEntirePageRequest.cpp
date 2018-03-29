#include "GLoadEntirePageRequest.h"

#include "utils/logger/src/GLogger.h"

GLoadEntirePageRequest::GLoadEntirePageRequest(	unsigned int uiSessionId, bool bSynchronize, bool bApplyZoomFactor,
												const QStringList& qtlStrListJSCalls, const QStringList& qtlStrListJSLibraries, const QString& strURL )
	: 	GAbsPageLoadRequest(uiSessionId, bSynchronize),
		m_bApplyZoomFactor(bApplyZoomFactor),
		m_qtlStrListJSCalls(qtlStrListJSCalls),
		m_qtlStrListJSLibraries(qtlStrListJSLibraries),
		m_strURL(strURL)
{
	LOG_LEVEL4("GLoadEntirePageRequest()");

	m_eTypeOfRequest=GAbsPageLoadRequest::eLoadEntirePageRequest;
}

GLoadEntirePageRequest::~GLoadEntirePageRequest()
{
	LOG_LEVEL4("~GLoadEntirePageRequest()");
}
