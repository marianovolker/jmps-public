#include "GISAdjustBacklightIntensityMsgData.h"

#include "control/src/infoscreenmsg/utils/GISCommandStatusEnum.h"
#include "control/src/infoscreenmsg/utils/GISException.h"
#include "model/src/infoscreenmodel/GISModelSingleton.h"
#include "utils/logger/src/GLogger.h"

const char * GISAdjustBacklightIntensityMsgData::m_CName="AdjustBacklightIntensityCommand";
const char * GISAdjustBacklightIntensityMsgData::m_CCommandId="012";

GISAdjustBacklightIntensityMsgData::GISAdjustBacklightIntensityMsgData()
{
	LOG_LEVEL4("GISAdjustBacklightIntensityMsgData()");
}

GISAdjustBacklightIntensityMsgData::~GISAdjustBacklightIntensityMsgData()
{
	LOG_LEVEL4("~GISAdjustBacklightIntensityMsgData()");
}

void GISAdjustBacklightIntensityMsgData::Validate() throw(GException)
{
	LOG_LEVEL4("Validate()");

	// CRIND BIOS does not do any attribute checking on this command, and does not implement it.
}

QString GISAdjustBacklightIntensityMsgData::GetName()
{
	LOG_LEVEL4("GetName()");

	return QString(m_CName);
}

QString GISAdjustBacklightIntensityMsgData::GetCommand()
{
	LOG_LEVEL4("GetCommand()");

	return QString(m_CCommandId);
}
