#include "GISVideoFrameSeekMsgData.h"

#include "control/src/infoscreenmsg/utils/GISCommandStatusEnum.h"
#include "control/src/infoscreenmsg/utils/GISException.h"
#include "model/src/infoscreenmodel/GISModelSingleton.h"
#include "utils/logger/src/GLogger.h"

const char * GISVideoFrameSeekMsgData::m_CName="VideoFrameSeekCommand";
const char * GISVideoFrameSeekMsgData::m_CCommandId="023";

GISVideoFrameSeekMsgData::GISVideoFrameSeekMsgData()
{
	LOG_LEVEL4("GISVideoFrameSeekMsgData()");
}

GISVideoFrameSeekMsgData::~GISVideoFrameSeekMsgData()
{
	LOG_LEVEL4("~GISVideoFrameSeekMsgData()");
}

void GISVideoFrameSeekMsgData::Validate() throw(GException)
{
	LOG_LEVEL4("Validate()");

	// Do nothing
}

QString GISVideoFrameSeekMsgData::GetName()
{
	LOG_LEVEL4("GetName()");

	return QString(m_CName);
}

QString GISVideoFrameSeekMsgData::GetCommand()
{
	LOG_LEVEL4("GetCommand()");

	return QString(m_CCommandId);
}
