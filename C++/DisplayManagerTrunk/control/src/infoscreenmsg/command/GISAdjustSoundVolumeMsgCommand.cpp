#include "GISAdjustSoundVolumeMsgCommand.h"

#include "utils/logger/src/GLogger.h"

GISAdjustSoundVolumeMsgCommand::GISAdjustSoundVolumeMsgCommand()
{
	LOG_LEVEL4("GISAdjustSoundVolumeMsgCommand()");
}

GISAdjustSoundVolumeMsgCommand::~GISAdjustSoundVolumeMsgCommand()
{
	LOG_LEVEL4("~GISAdjustSoundVolumeMsgCommand()");
}

void GISAdjustSoundVolumeMsgCommand::DoExecute() throw (GException)
{
	LOG_LEVEL3("DoExecute()");

	// Display Manager does not implement it.
}
