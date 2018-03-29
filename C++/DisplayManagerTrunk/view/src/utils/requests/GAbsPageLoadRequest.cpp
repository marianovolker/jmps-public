#include "GAbsPageLoadRequest.h"

#include "utils/logger/src/GLogger.h"

GAbsPageLoadRequest::GAbsPageLoadRequest(unsigned int uiSessionId, bool bSynchronize)
	: 	m_uiSessionId	(uiSessionId),
		m_bSynchronize	(bSynchronize),
		m_bNotify		(true),
		m_eTypeOfRequest(GAbsPageLoadRequest::eUnknowType),
		m_uiRequestId   (0)
{
	LOG_LEVEL4("GAbsPageLoadRequest()");
}

GAbsPageLoadRequest::~GAbsPageLoadRequest()
{
	LOG_LEVEL4("~GAbsPageLoadRequest()");
}

