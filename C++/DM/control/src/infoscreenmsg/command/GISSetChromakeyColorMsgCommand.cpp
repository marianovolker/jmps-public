#include "GISSetChromakeyColorMsgCommand.h"

#include "utils/logger/src/GLogger.h"

GISSetChromakeyColorMsgCommand::GISSetChromakeyColorMsgCommand()
{
	LOG_LEVEL4("GISSetChromakeyColorMsgCommand()");
}

GISSetChromakeyColorMsgCommand::~GISSetChromakeyColorMsgCommand()
{
	LOG_LEVEL4("~GISSetChromakeyColorMsgCommand()");
}

void GISSetChromakeyColorMsgCommand::DoExecute() throw (GException)
{
	LOG_LEVEL3("DoExecute()");

	// Display Manager does not implement it.
}
