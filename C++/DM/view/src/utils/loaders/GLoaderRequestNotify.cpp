#include "GLoaderRequestNotify.h"

GLoaderRequestNotify::GLoaderRequestNotify(unsigned int uiPageLoaderId, int uiSessionId, GLoaderRequestNotify::eNotifyTypeEnum eType)
	: m_uiPageLoaderId(uiPageLoaderId),
	  m_uiSessionId(uiSessionId),
	  m_eNotifyType(eType),
	  m_pRequest(0)
{
}

GLoaderRequestNotify::~GLoaderRequestNotify()
{
}
