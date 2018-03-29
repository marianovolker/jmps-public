#include "GKeyEventMsgData.h"

#include "utils/logger/src/GLogger.h"

GKeyEventMsgData::GKeyEventMsgData()
{
	LOG_LEVEL4("GKeyEventMsgData()");
}

GKeyEventMsgData::~GKeyEventMsgData()
{
	LOG_LEVEL4("~GKeyEventMsgData()");
}

QString GKeyEventMsgData::GetKeyCode()
{
	LOG_LEVEL4("GetKeyCode()");

	return m_strKeyCode;
}

void GKeyEventMsgData::SetKeyCode(QString strKeyCode)
{
	LOG_LEVEL4("SetKeyCode()");

	m_strKeyCode = strKeyCode;
}
