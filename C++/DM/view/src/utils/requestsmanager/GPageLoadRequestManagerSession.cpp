#include "GPageLoadRequestManagerSession.h"

GPageLoadRequestManagerSession::GPageLoadRequestManagerSession(unsigned int uiSessionId, QString sName, bool bIsView)
	:	m_iId(uiSessionId),
		m_sName(QString("Session(%1=%2)").arg(sName).arg(uiSessionId)),
		m_bIsView(bIsView)
{
}

GPageLoadRequestManagerSession::~GPageLoadRequestManagerSession()
{
}
