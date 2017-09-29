#include "GClearScreenMsgData.h"

#include "utils/logger/src/GLogger.h"


GClearScreenMsgData::GClearScreenMsgData()
{
	LOG_LEVEL4("GClearScreenMsgData()");
}

GClearScreenMsgData::~GClearScreenMsgData()
{
	LOG_LEVEL4("~GClearScreenMsgData()");
}

QString GClearScreenMsgData::GetColor()
{
	LOG_LEVEL4("GetColor()");

	return m_strColor;
}

void GClearScreenMsgData::SetColor(QString strColor)
{
	LOG_LEVEL4("SetColor()");

	m_strColor = strColor;
}
