#include "GISVideoViewMsgData.h"

#include "control/src/infoscreenmsg/utils/GISCommandStatusEnum.h"
#include "control/src/infoscreenmsg/utils/GISException.h"
#include "model/src/infoscreenmodel/GISModelSingleton.h"
#include "utils/logger/src/GLogger.h"

const char * GISVideoViewMsgData::m_CName="VideoViewCommand";
const char * GISVideoViewMsgData::m_CCommandId="022";

GISVideoViewMsgData::GISVideoViewMsgData()
{
	LOG_LEVEL4("GISVideoViewMsgData()");
}

GISVideoViewMsgData::~GISVideoViewMsgData()
{
	LOG_LEVEL4("~GISVideoViewMsgData()");
}

void GISVideoViewMsgData::Validate() throw(GException)
{
	LOG_LEVEL4("Validate()");

	// Do nothing
}

QString GISVideoViewMsgData::GetName()
{
	LOG_LEVEL4("GetName()");

	return QString(m_CName);
}

QString GISVideoViewMsgData::GetCommand()
{
	LOG_LEVEL4("GetCommand()");

	return QString(m_CCommandId);
}
