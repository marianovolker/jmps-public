#include "GISVideoStopMsgCommand.h"

#include "utils/logger/src/GLogger.h"

GISVideoStopMsgCommand::GISVideoStopMsgCommand()
{
	LOG_LEVEL4("GISVideoStopMsgCommand()");
}

GISVideoStopMsgCommand::~GISVideoStopMsgCommand()
{
	LOG_LEVEL4("~GISVideoStopMsgCommand()");
}

void GISVideoStopMsgCommand::DoExecute() throw (GException)
{
	LOG_LEVEL3("DoExecute()");

	// Display Manager does not implement it.
}
