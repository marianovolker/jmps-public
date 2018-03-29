#include "GAbsMsgCommand.h"

#include "control/src/displaymanagermsgs/xml/GResponseMsgDataXML.h"
#include "utils/logger/src/GLogger.h"




GAbsMsgCommand::GAbsMsgCommand(GAbsData * pData)
	: GAbsCommand(pData)
{
	LOG_LEVEL4("GAbsMsgCommand()");

}

GAbsMsgCommand::~GAbsMsgCommand()
{
	LOG_LEVEL4("~GAbsMsgCommand()");

}




