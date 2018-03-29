#include "GISVideoPlayMsgData.h"

#include "control/src/infoscreenmsg/utils/GISCommandStatusEnum.h"
#include "control/src/infoscreenmsg/utils/GISException.h"
#include "model/src/infoscreenmodel/GISModelSingleton.h"
#include "utils/logger/src/GLogger.h"

const char * GISVideoPlayMsgData::m_CName="VideoPlayCommand";
const char * GISVideoPlayMsgData::m_CCommandId="024";

GISVideoPlayMsgData::GISVideoPlayMsgData()
{
	LOG_LEVEL4("GISVideoPlayMsgData()");
}

GISVideoPlayMsgData::~GISVideoPlayMsgData()
{
	LOG_LEVEL4("~GISVideoPlayMsgData()");
}

void GISVideoPlayMsgData::Validate() throw(GException)
{
	LOG_LEVEL4("Validate()");

	// Do nothing
}

QString GISVideoPlayMsgData::GetName()
{
	LOG_LEVEL4("GetName()");

	return QString(m_CName);
}

QString GISVideoPlayMsgData::GetCommand()
{
	LOG_LEVEL4("GetCommand()");

	return QString(m_CCommandId);
}
