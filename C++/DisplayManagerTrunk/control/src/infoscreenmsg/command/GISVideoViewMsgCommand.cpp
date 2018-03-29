#include "GISVideoViewMsgCommand.h"

#include "utils/logger/src/GLogger.h"

GISVideoViewMsgCommand::GISVideoViewMsgCommand()
{
	LOG_LEVEL4("GISVideoViewMsgCommand()");
}

GISVideoViewMsgCommand::~GISVideoViewMsgCommand()
{
	LOG_LEVEL4("~GISVideoViewMsgCommand()");
}

void GISVideoViewMsgCommand::DoExecute() throw (GException)
{
	LOG_LEVEL3("DoExecute()");

	// Display Manager does not implement it.
}
