#include "GResponseMsgCommand.h"

#include "utils/logger/src/GLogger.h"


GResponseMsgCommand::GResponseMsgCommand(GAbsData * pData)
: GAbsMsgCommand(pData)
{
	LOG_LEVEL4("GResponseMsgCommand()");
}

GResponseMsgCommand::~GResponseMsgCommand()
{
	LOG_LEVEL4("~GResponseMsgCommand()");
}

void GResponseMsgCommand::Execute() throw(GException)
{
	LOG_LEVEL4("Execute()");
}
