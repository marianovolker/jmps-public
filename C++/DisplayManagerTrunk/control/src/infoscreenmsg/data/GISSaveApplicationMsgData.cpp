#include "GISSaveApplicationMsgData.h"

#include "control/src/infoscreenmsg/utils/GISCommandStatusEnum.h"
#include "control/src/infoscreenmsg/utils/GISException.h"
#include "model/src/infoscreenmodel/GISModelSingleton.h"
#include "utils/logger/src/GLogger.h"

const char * GISSaveApplicationMsgData::m_CName="SaveApplicationCommand";
const char * GISSaveApplicationMsgData::m_CCommandId="030";

GISSaveApplicationMsgData::GISSaveApplicationMsgData()
{
	LOG_LEVEL4("GISSaveApplicationMsgData()");
}

GISSaveApplicationMsgData::~GISSaveApplicationMsgData()
{
	LOG_LEVEL4("~GISSaveApplicationMsgData()");
}

void GISSaveApplicationMsgData::Validate() throw(GException)
{
	LOG_LEVEL4("Validate()");

	// Do nothing at this level.
}

QString GISSaveApplicationMsgData::GetName()
{
	LOG_LEVEL4("GetName()");

	return QString(m_CName);
}

QString GISSaveApplicationMsgData::GetCommand()
{
	LOG_LEVEL4("GetCommand()");

	return QString(m_CCommandId);
}
