#include "GISVideoRangePlayMsgData.h"

#include "control/src/infoscreenmsg/utils/GISCommandStatusEnum.h"
#include "control/src/infoscreenmsg/utils/GISException.h"
#include "model/src/infoscreenmodel/GISModelSingleton.h"
#include "utils/logger/src/GLogger.h"

const char * GISVideoRangePlayMsgData::m_CName="VideoRangePlayCommand";
const char * GISVideoRangePlayMsgData::m_CCommandId="026";

GISVideoRangePlayMsgData::GISVideoRangePlayMsgData()
{
	LOG_LEVEL4("GISVideoRangePlayMsgData()");
}

GISVideoRangePlayMsgData::~GISVideoRangePlayMsgData()
{
	LOG_LEVEL4("~GISVideoRangePlayMsgData()");
}

void GISVideoRangePlayMsgData::Validate() throw(GException)
{
	LOG_LEVEL4("Validate()");

	// Do nothing
}

QString GISVideoRangePlayMsgData::GetName()
{
	LOG_LEVEL4("GetName()");

	return QString(m_CName);
}

QString GISVideoRangePlayMsgData::GetCommand()
{
	LOG_LEVEL4("GetCommand()");

	return QString(m_CCommandId);
}
