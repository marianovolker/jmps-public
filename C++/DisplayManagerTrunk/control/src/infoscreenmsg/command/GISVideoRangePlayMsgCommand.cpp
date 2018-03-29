#include "GISVideoRangePlayMsgCommand.h"

#include "utils/logger/src/GLogger.h"

GISVideoRangePlayMsgCommand::GISVideoRangePlayMsgCommand()
{
	LOG_LEVEL4("GISVideoRangePlayMsgCommand()");
}

GISVideoRangePlayMsgCommand::~GISVideoRangePlayMsgCommand()
{
	LOG_LEVEL4("~GISVideoRangePlayMsgCommand()");
}

void GISVideoRangePlayMsgCommand::DoExecute() throw (GException)
{
	LOG_LEVEL3("DoExecute()");

	// Display Manager does not implement it.
}
