#include "GURLException.h"


GURLException::GURLException(const QString &strMsg, QString strDetails):
	GException("")
{
	m_strMsg = QString("GURLException(") + strMsg;

	if( !strDetails.isEmpty() )
	{
		m_strMsg += strDetails;
	}

	m_strMsg += ")";
}

GURLException::~GURLException() throw()
{
}

