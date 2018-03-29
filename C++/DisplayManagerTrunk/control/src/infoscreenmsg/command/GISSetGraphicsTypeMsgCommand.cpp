#include "GISSetGraphicsTypeMsgCommand.h"

#include "utils/logger/src/GLogger.h"

GISSetGraphicsTypeMsgCommand::GISSetGraphicsTypeMsgCommand()
{
	LOG_LEVEL4("GISSetGraphicsTypeMsgCommand()");
}

GISSetGraphicsTypeMsgCommand::~GISSetGraphicsTypeMsgCommand()
{
	LOG_LEVEL4("~GISSetGraphicsTypeMsgCommand()");
}

void GISSetGraphicsTypeMsgCommand::DoExecute() throw (GException)
{
	LOG_LEVEL3("DoExecute()");

	// Display Manager does not implement it.
}
