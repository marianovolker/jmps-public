#include "GUpdateCurrentPageRequest.h"

#include "utils/logger/src/GLogger.h"

GUpdateCurrentPageRequest::GUpdateCurrentPageRequest( unsigned int uiSessionId, bool bSynchronize, const QStringList& qtlStrListJSCalls )
	: 	GAbsPageLoadRequest(uiSessionId, bSynchronize),
		m_qtlStrListJSCalls(qtlStrListJSCalls)
{
	LOG_LEVEL4("GUpdateCurrentPageRequest()");

	m_eTypeOfRequest = GAbsPageLoadRequest::eUpdateCurrentPageRequest;
}

GUpdateCurrentPageRequest::~GUpdateCurrentPageRequest()
{
	LOG_LEVEL4("~GUpdateCurrentPageRequest()");
}

