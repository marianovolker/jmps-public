#include "GISException.h"

GISException::GISException(GISCommandStatusEnum oCommandStatus, QString strDetails):
	GException(""),
	m_oCommandStatus(oCommandStatus.GetError())
{
	m_strMsg = QString("GISException(") + m_oCommandStatus.toString();

	if( !strDetails.isEmpty() )
	{
		m_strMsg += strDetails;
	}

	m_strMsg += ")";
}

GISException::~GISException() throw()
{
}

GISCommandStatusEnum GISException::GetError()
{
	return m_oCommandStatus;
}
