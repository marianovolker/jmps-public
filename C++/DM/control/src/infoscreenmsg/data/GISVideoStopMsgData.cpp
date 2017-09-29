#include "GISVideoStopMsgData.h"

#include "control/src/infoscreenmsg/utils/GISCommandStatusEnum.h"
#include "control/src/infoscreenmsg/utils/GISException.h"
#include "model/src/infoscreenmodel/GISModelSingleton.h"
#include "utils/logger/src/GLogger.h"

const char * GISVideoStopMsgData::m_CName="VideoStopCommand";
const char * GISVideoStopMsgData::m_CCommandId="025";

GISVideoStopMsgData::GISVideoStopMsgData()
{
	LOG_LEVEL4("GISVideoStopMsgData()");
}

GISVideoStopMsgData::~GISVideoStopMsgData()
{
	LOG_LEVEL4("~GISVideoStopMsgData()");
}

void GISVideoStopMsgData::Validate() throw(GException)
{
	LOG_LEVEL4("Validate()");

	// Do nothing
}

QString GISVideoStopMsgData::GetName()
{
	LOG_LEVEL4("GetName()");

	return QString(m_CName);
}

QString GISVideoStopMsgData::GetCommand()
{
	LOG_LEVEL4("GetCommand()");

	return QString(m_CCommandId);
}
