#include "GISSegmentDisplayCountMsgCommand.h"

#include "utils/logger/src/GLogger.h"

GISSegmentDisplayCountMsgCommand::GISSegmentDisplayCountMsgCommand()
{
	LOG_LEVEL4("GISSegmentDisplayCountMsgCommand()");
}

GISSegmentDisplayCountMsgCommand::~GISSegmentDisplayCountMsgCommand()
{
	LOG_LEVEL4("~GISSegmentDisplayCountMsgCommand()");
}

void GISSegmentDisplayCountMsgCommand::DoExecute() throw (GException)
{
	LOG_LEVEL3("DoExecute()");

	// Display Manager does not implement it.
}
