#include "GISAdjustBacklightIntensityMsgCommand.h"

#include "utils/logger/src/GLogger.h"

GISAdjustBacklightIntensityMsgCommand::GISAdjustBacklightIntensityMsgCommand()
{
	LOG_LEVEL4("GISAdjustBacklightIntensityMsgCommand()");
}

GISAdjustBacklightIntensityMsgCommand::~GISAdjustBacklightIntensityMsgCommand()
{
	LOG_LEVEL4("~GISAdjustBacklightIntensityMsgCommand()");
}

void GISAdjustBacklightIntensityMsgCommand::DoExecute() throw (GException)
{
	LOG_LEVEL3("DoExecute()");

	// CRIND BIOS does not do any attribute checking on this command, and Display Manager does not implement it.
}
