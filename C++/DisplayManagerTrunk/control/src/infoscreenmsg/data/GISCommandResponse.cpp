#include "GISCommandResponse.h"

#include "utils/logger/src/GLogger.h"


GISCommandResponse::GISCommandResponse()
{
	LOG_LEVEL4("GISCommandResponse()");
}

GISCommandResponse::~GISCommandResponse()
{
	LOG_LEVEL4("~GISCommandResponse()");
}

int GISCommandResponse::GetCommand()
{
	LOG_LEVEL4("GetCommand()");

	return m_iCommand;
}

void GISCommandResponse::SetCommand(int iCommand)
{
	LOG_LEVEL4("SetCommand()");

	m_iCommand = iCommand;
}

GISCommandStatusEnum GISCommandResponse::GetCommandStatus()
{
	LOG_LEVEL4("GetCommandStatus()");

	return m_oCommandStatus;
}

void GISCommandResponse::SetCommandStatus(GISCommandStatusEnum oCommandStatus)
{
	LOG_LEVEL4("SetCommandStatus()");

	m_oCommandStatus = oCommandStatus;
}

QString GISCommandResponse::GetData()
{
	LOG_LEVEL4("GetData()");

	return m_strData;
}

void GISCommandResponse::SetData(QString strData)
{
	LOG_LEVEL4("SetData()");

	m_strData = strData;
}

int GISCommandResponse::GetDisplay()
{
	LOG_LEVEL4("GetDisplay()");

	return m_iDisplay;
}

void GISCommandResponse::SetDisplay(int iDisplay)
{
	LOG_LEVEL4("SetDisplay()");

	m_iDisplay = iDisplay;
}

QString GISCommandResponse::ToString()
{
	LOG_LEVEL4("ToString()");

	QByteArray strRet;

	strRet.append( QString("").sprintf("%03d", GetCommand()) );
	strRet.append( QChar(0x0000) );
	strRet.append( QString("").sprintf("%01d", GetDisplay()) );	
	strRet.append( QChar(0x0000) );
	strRet.append( QString("").sprintf("%02d", GetCommandStatus().GetError()) );
	strRet.append( QChar(0x0000) );
	if( !m_strData.isEmpty() )
	{
		strRet.append(GetData());
	}
	
	strRet = strRet.toHex();

	return strRet;
}

void GISCommandResponse::Validate() throw(GException)
{
	LOG_LEVEL4("Validate()");

	// Do nothing at this level.
}
