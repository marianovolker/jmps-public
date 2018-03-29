#include "GISAdjustSoundVolumeMsgData.h"

#include "control/src/infoscreenmsg/utils/GISCommandStatusEnum.h"
#include "control/src/infoscreenmsg/utils/GISException.h"
#include "model/src/infoscreenmodel/GISModelSingleton.h"
#include "utils/logger/src/GLogger.h"

const char * GISAdjustSoundVolumeMsgData::m_CName="AdjustSoundVolumeCommand";
const char * GISAdjustSoundVolumeMsgData::m_CCommandId="027";

GISAdjustSoundVolumeMsgData::GISAdjustSoundVolumeMsgData()
{
	LOG_LEVEL4("GISAdjustSoundVolumeMsgData()");
}

GISAdjustSoundVolumeMsgData::~GISAdjustSoundVolumeMsgData()
{
	LOG_LEVEL4("~GISAdjustSoundVolumeMsgData()");
}

void GISAdjustSoundVolumeMsgData::Validate() throw(GException)
{
	LOG_LEVEL4("Validate()");

	// Do nothing.
}

QString GISAdjustSoundVolumeMsgData::GetName()
{
	LOG_LEVEL4("GetName()");

	return QString(m_CName);
}

QString GISAdjustSoundVolumeMsgData::GetCommand()
{
	LOG_LEVEL4("GetCommand()");

	return QString(m_CCommandId);
}
