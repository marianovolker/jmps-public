#include "GSetForkFailureOptionMsgData.h"

#include "utils/logger/src/GLogger.h"

GSetForkFailureOptionMsgData::GSetForkFailureOptionMsgData():
m_strForkFailureOption("")
{
	LOG_LEVEL4("GSetForkFailureOptionMsgData()");
}

GSetForkFailureOptionMsgData::~GSetForkFailureOptionMsgData()
{
	LOG_LEVEL4("~GSetForkFailureOptionMsgData()");
}

QString GSetForkFailureOptionMsgData::GetType()
{
	LOG_LEVEL4("GetType()");

	return  m_strForkFailureOption;
}

void GSetForkFailureOptionMsgData::SetType(QString strForkFailureOption)
{
	LOG_LEVEL4("SetType()");

	m_strForkFailureOption = strForkFailureOption;
}
