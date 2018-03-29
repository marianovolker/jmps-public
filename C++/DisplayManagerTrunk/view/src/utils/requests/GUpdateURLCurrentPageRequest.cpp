#include "GUpdateURLCurrentPageRequest.h"

#include "utils/logger/src/GLogger.h"

GUpdateURLCurrentPageRequest::GUpdateURLCurrentPageRequest( unsigned int uiSessionId, bool bSynchronize, const QStringList& qtlStrListJSCalls )
	: 	GAbsPageLoadRequest(uiSessionId, bSynchronize),
		m_qtlStrListJSCalls(qtlStrListJSCalls)
{
	LOG_LEVEL4("GUpdateURLCurrentPageRequest()");

	m_eTypeOfRequest = GAbsPageLoadRequest::eUpdateURLCurrentPageRequest;
}

GUpdateURLCurrentPageRequest::~GUpdateURLCurrentPageRequest()
{
	LOG_LEVEL4("~GUpdateURLCurrentPageRequest()");
}

