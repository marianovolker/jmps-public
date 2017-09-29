#include "GInfoScreenMsgData.h"

#include "utils/logger/src/GLogger.h"

GInfoScreenMsgData::GInfoScreenMsgData()
{
	LOG_LEVEL4("GInfoScreenMsgData()");
}

GInfoScreenMsgData::~GInfoScreenMsgData()
{
	LOG_LEVEL4("~GInfoScreenMsgData()");
}

QByteArray GInfoScreenMsgData::GetData()
{
	LOG_LEVEL4("GetData()");

	return m_oData;
}

void GInfoScreenMsgData::SetData(QByteArray oData)
{
	LOG_LEVEL4("SetData()");

	m_oData = oData;
}
