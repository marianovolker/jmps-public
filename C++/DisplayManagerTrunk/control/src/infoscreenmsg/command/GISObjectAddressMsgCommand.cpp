#include "GISObjectAddressMsgCommand.h"

#include "utils/logger/src/GLogger.h"

GISObjectAddressMsgCommand::GISObjectAddressMsgCommand()
{
	LOG_LEVEL4("GISObjectAddressMsgCommand()");
}

GISObjectAddressMsgCommand::~GISObjectAddressMsgCommand()
{
	LOG_LEVEL4("~GISObjectAddressMsgCommand()");
}

void GISObjectAddressMsgCommand::DoExecute() throw (GException)
{
	LOG_LEVEL3("DoExecute()");

	// Display Manager does not implement it.
}
