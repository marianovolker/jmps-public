#include "GSetPreloadedPagesRequest.h"

#include "utils/logger/src/GLogger.h"

GSetPreloadedPagesRequest::GSetPreloadedPagesRequest(	unsigned int uiSessionId, bool bSynchronize,
														const QStringList& qtlStrListJSCalls, const QStringList& qtlStrListJSLibraries, const QString& strURL )
	: 	GAbsPageLoadRequest(uiSessionId, bSynchronize),
		m_qtlStrListJSCalls(qtlStrListJSCalls),
		m_qtlStrListJSLibraries(qtlStrListJSLibraries),
		m_strURL(strURL)
{
	LOG_LEVEL4("GSetPreloadedPagesRequest()");

	m_eTypeOfRequest = GAbsPageLoadRequest::eSetPreloadedPagesRequest;
}

GSetPreloadedPagesRequest::~GSetPreloadedPagesRequest()
{
	LOG_LEVEL4("~GSetPreloadedPagesRequest()");
}

