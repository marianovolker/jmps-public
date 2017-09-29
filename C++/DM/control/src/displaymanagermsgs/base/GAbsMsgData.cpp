#include "GAbsMsgData.h"

#include "utils/logger/src/GLogger.h"

GAbsMsgData::GAbsMsgData()
	:	m_strInterfaceVersion(""),
		m_strNamespacePrefix (""),
		m_strNamespaceURI	 (""),
		m_bSynchronousRequest(false)
{
	LOG_LEVEL4("GAbsMsgData()");
}

GAbsMsgData::~GAbsMsgData()
{
	LOG_LEVEL4("~GAbsMsgData()");
}

QString GAbsMsgData::GetInterfaceVersion()
{
	LOG_LEVEL4("GetInterfaceVersion()");

	return  m_strInterfaceVersion;
}

void GAbsMsgData::SetInterfaceVersion(QString strInterfaceVersion)
{
	LOG_LEVEL4("SetInterfaceVersion()");

	m_strInterfaceVersion = strInterfaceVersion;
}

QString GAbsMsgData::GetNamespacePrefix()
{
	LOG_LEVEL4("GetNamespacePrefix()");

	return  m_strNamespacePrefix;
}

void GAbsMsgData::SetNamespacePrefix(QString strNamespacePrefix)
{
	LOG_LEVEL4("SetNamespacePrefix()");

	m_strNamespacePrefix = strNamespacePrefix;
}

QString GAbsMsgData::GetNamespaceURI()
{
	LOG_LEVEL4("GetNamespaceURI()");

	return  m_strNamespaceURI;
}

void GAbsMsgData::SetNamespaceURI(QString strNamespaceURI)
{
	LOG_LEVEL4("SetNamespaceURI()");

	m_strNamespaceURI = strNamespaceURI;
}

bool GAbsMsgData::GetSynchronousRequest()
{
	LOG_LEVEL3("GetSynchronousRequest()");

	return  m_bSynchronousRequest;
}

void GAbsMsgData::SetSynchronousRequest(bool bSynchronousRequest)
{
	LOG_LEVEL3("SetSynchronousRequest()");

	m_bSynchronousRequest = bSynchronousRequest;

	if( m_bSynchronousRequest )
	{
		LOG_LEVEL3("Setting the response as Synchronous.");
	}
}

void GAbsMsgData::Validate() throw(GException)
{
	LOG_LEVEL4("Validate()");
}
