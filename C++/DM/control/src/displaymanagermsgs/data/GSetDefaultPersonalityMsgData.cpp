#include "GSetDefaultPersonalityMsgData.h"

#include "utils/logger/src/GLogger.h"

GSetDefaultPersonalityMsgData::GSetDefaultPersonalityMsgData()
{
	LOG_LEVEL4("GSetDefaultPersonalityMsgData()");
}

GSetDefaultPersonalityMsgData::~GSetDefaultPersonalityMsgData()
{
	LOG_LEVEL4("~GSetDefaultPersonalityMsgData()");
}

QString GSetDefaultPersonalityMsgData::GetPersonality()
{
	LOG_LEVEL4("GetPersonality()");

	return  m_strPersonality;
}

void GSetDefaultPersonalityMsgData::SetPersonality(QString strPersonality)
{
	LOG_LEVEL4("SetPersonality()");

	m_strPersonality = strPersonality;
}
