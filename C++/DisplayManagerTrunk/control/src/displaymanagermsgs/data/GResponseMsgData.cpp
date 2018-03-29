#include "GResponseMsgData.h"

#include "utils/logger/src/GLogger.h"

const QString GResponseMsgData::CResponseOk						("ok");
const QString GResponseMsgData::CResponseParseError				("parse_error");
const QString GResponseMsgData::CResponseInvalidPersonality		("invalid_personality");
const QString GResponseMsgData::CResponseUnknownError			("unknown_error");
const QString GResponseMsgData::CResponseError					("error");
const QString GResponseMsgData::CResponseInfoScreenError		("infoscreen_error");
const QString GResponseMsgData::CResponseURLError				("error"); //TODO: For backward compatibility, error made equal to Old DM's Error (may change in the future)
const QString GResponseMsgData::CResponseGDIError				("error"); //TODO: For backward compatibility, error made equal to Old DM's Error (may change in the future)
const QString GResponseMsgData::CResponseCantGetPersonalityList	("Can't get personality list");
const QString GResponseMsgData::CResponseInvalidMediaManagerType    ("invalid_media_manager_type");
const QString GResponseMsgData::CResponseCantGetMediaManagerList	("Can't get media manager list");
const QString GResponseMsgData::CResponseInvalidForkFailureOption    ("invalid_fork_failure_option");
const QString GResponseMsgData::CResponseCantGetForkFailureOptionsList	("Can't get fork failure options list");

GResponseMsgData::GResponseMsgData()
{
	LOG_LEVEL4("GResponseMsgData()");
}

GResponseMsgData::~GResponseMsgData()
{
	LOG_LEVEL4("~GResponseMsgData()");
}

const QString & GResponseMsgData::GetCommand() const
{
	LOG_LEVEL4("GetCommand()");

	return m_strCommand;
}

void GResponseMsgData::SetCommand(const QString & strCommand)
{
	LOG_LEVEL4("SetCommand()");

	m_strCommand = strCommand;
}

const QByteArray & GResponseMsgData::GetData()
{
	LOG_LEVEL4("GetData()");

	return m_oData;
}

void GResponseMsgData::SetData(const QByteArray &strData)
{
	LOG_LEVEL4("SetData()");

	m_oData = strData;
}

GResponseMsgData::eResponseCode GResponseMsgData::GetResponseCode()
{
	LOG_LEVEL4("GetResponseCode()");

	return m_eCode;
}

void GResponseMsgData::SetResponseCode(eResponseCode eCode)
{
	LOG_LEVEL4("SetResponseCode()");

	m_eCode = eCode;
}
