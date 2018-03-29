#include "GISVideoFrameSeekMsgCommand.h"

#include "utils/logger/src/GLogger.h"

GISVideoFrameSeekMsgCommand::GISVideoFrameSeekMsgCommand()
{
	LOG_LEVEL4("GISVideoFrameSeekMsgCommand()");
}

GISVideoFrameSeekMsgCommand::~GISVideoFrameSeekMsgCommand()
{
	LOG_LEVEL4("~GISVideoFrameSeekMsgCommand()");
}

void GISVideoFrameSeekMsgCommand::DoExecute() throw (GException)
{
	LOG_LEVEL3("DoExecute()");

	// Display Manager does not implement it.
}
