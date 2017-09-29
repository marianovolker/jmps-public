#include "GISVideoPlayMsgCommand.h"

#include "utils/logger/src/GLogger.h"

GISVideoPlayMsgCommand::GISVideoPlayMsgCommand()
{
	LOG_LEVEL4("GISVideoPlayMsgCommand()");
}

GISVideoPlayMsgCommand::~GISVideoPlayMsgCommand()
{
	LOG_LEVEL4("~GISVideoPlayMsgCommand()");
}

void GISVideoPlayMsgCommand::DoExecute() throw (GException)
{
	LOG_LEVEL3("DoExecute()");

	// Display Manager does not implement it.
}
