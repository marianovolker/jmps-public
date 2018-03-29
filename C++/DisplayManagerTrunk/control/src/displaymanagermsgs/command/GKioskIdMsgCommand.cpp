#include "GKioskIdMsgCommand.h"

#include "control/src/displaymanagermsgs/xml/GKioskIdMsgDataXML.h"
#include "control/src/displaymanagermsgs/data/GResponseMsgData.h"
#include "control/src/displaymanagermsgs/factory/GMsgCmdDataFactory.h"
#include "utils/logger/src/GLogger.h"

#include <memory>

GKioskIdMsgCommand::GKioskIdMsgCommand(GAbsData * pData)
	: GAbsMsgCommand(pData)
{
	LOG_LEVEL4("GKioskIdMsgCommand()");
}

GKioskIdMsgCommand::~GKioskIdMsgCommand()
{
	LOG_LEVEL4("~GKioskIdMsgCommand()");
}

void GKioskIdMsgCommand::Execute() throw(GException)
{
	LOG_LEVEL4("Execute()");
	
	// Dummy Response
	std::auto_ptr<GResponseMsgData> poResponse(CreateInitializedResponse(GKioskIdMsgDataXML::CXMLTag, GMsgCmdDataFactory::CInterfaceVersion));
	poResponse->SetResponseCode(GResponseMsgData::eOk);
	
	SetResponseNamespace(poResponse);
	m_pResponse=poResponse.release();
}
