#include "GSetPersonalityMsgData.h"

#include "utils/logger/src/GLogger.h"

GSetPersonalityMsgData::GSetPersonalityMsgData()
{
	LOG_LEVEL4("GSetPersonalityMsgData()");
}

GSetPersonalityMsgData::~GSetPersonalityMsgData()
{
	LOG_LEVEL4("~GSetPersonalityMsgData()");
}

QString GSetPersonalityMsgData::GetPersonality()
{
	LOG_LEVEL4("GetPersonality()");

	return  m_strPersonality;
}

void GSetPersonalityMsgData::SetPersonality(QString strPersonality)
{
	LOG_LEVEL4("SetPersonality()");

	m_strPersonality = strPersonality;
}
