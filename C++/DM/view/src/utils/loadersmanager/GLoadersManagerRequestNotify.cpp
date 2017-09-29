#include "GLoadersManagerRequestNotify.h"

GLoadersManagerRequestNotify::GLoadersManagerRequestNotify(unsigned int uiLoadersManagerId, int uiSessionId, GLoadersManagerRequestNotify::eNotifyTypeEnum eType)
	: m_uiLoadersManagerId(uiLoadersManagerId),
	  m_uiSessionId(uiSessionId),
	  m_eNotifyType(eType),
	  m_pPage(0),
	  m_pRequest(0)
{
}

GLoadersManagerRequestNotify::~GLoadersManagerRequestNotify()
{
}
