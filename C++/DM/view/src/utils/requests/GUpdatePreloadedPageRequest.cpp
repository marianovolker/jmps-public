#include "GUpdatePreloadedPageRequest.h"

#include "utils/logger/src/GLogger.h"

GUpdatePreloadedPageRequest::GUpdatePreloadedPageRequest( unsigned int uiSessionId, bool bSynchronize, const QStringList& qtlStrListJSCalls )
	: 	GAbsPageLoadRequest(uiSessionId, bSynchronize),
		m_qtlStrListJSCalls(qtlStrListJSCalls)

{
	LOG_LEVEL4("GUpdatePreloadedPageRequest()");

	m_eTypeOfRequest = GAbsPageLoadRequest::eUpdatePreloadedPageRequest;
}

GUpdatePreloadedPageRequest::~GUpdatePreloadedPageRequest()
{
	LOG_LEVEL4("~GUpdatePreloadedPageRequest()");
}

