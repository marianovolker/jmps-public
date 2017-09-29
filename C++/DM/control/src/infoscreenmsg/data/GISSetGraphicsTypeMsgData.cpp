#include "GISSetGraphicsTypeMsgData.h"

#include "control/src/infoscreenmsg/utils/GISCommandStatusEnum.h"
#include "control/src/infoscreenmsg/utils/GISException.h"
#include "model/src/infoscreenmodel/GISModelSingleton.h"
#include "utils/logger/src/GLogger.h"

const char * GISSetGraphicsTypeMsgData::m_CName="SetGraphicsTypeCommand";
const char * GISSetGraphicsTypeMsgData::m_CCommandId="034";

GISSetGraphicsTypeMsgData::GISSetGraphicsTypeMsgData()
{
	LOG_LEVEL4("GISSetGraphicsTypeMsgData()");
}

GISSetGraphicsTypeMsgData::~GISSetGraphicsTypeMsgData()
{
	LOG_LEVEL4("~GISSetGraphicsTypeMsgData()");
}

void GISSetGraphicsTypeMsgData::Validate() throw(GException)
{
	LOG_LEVEL4("Validate()");

	// Do nothing at this level.
}

QString GISSetGraphicsTypeMsgData::GetName()
{
	LOG_LEVEL4("GetName()");

	return QString(m_CName);
}

QString GISSetGraphicsTypeMsgData::GetCommand()
{
	LOG_LEVEL4("GetCommand()");

	return QString(m_CCommandId);
}
