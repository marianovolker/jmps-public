#include "GSetMediaManagerMsgData.h"

#include "utils/logger/src/GLogger.h"

GSetMediaManagerMsgData::GSetMediaManagerMsgData():
m_strMediaManagerType("")
{
	LOG_LEVEL4("GSetMediaManagerMsgData()");
}

GSetMediaManagerMsgData::~GSetMediaManagerMsgData()
{
	LOG_LEVEL4("~GSetMediaManagerMsgData()");
}

QString GSetMediaManagerMsgData::GetType()
{
	LOG_LEVEL4("GetType()");

	return  m_strMediaManagerType;
}

void GSetMediaManagerMsgData::SetType(QString strMediaManagerType)
{
	LOG_LEVEL4("SetType()");

	m_strMediaManagerType = strMediaManagerType;
}
