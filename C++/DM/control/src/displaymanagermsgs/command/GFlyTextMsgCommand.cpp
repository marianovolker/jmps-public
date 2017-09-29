#include "GFlyTextMsgCommand.h"

#include "control/src/displaymanagermsgs/xml/GFlyTextMsgDataXML.h"
#include "control/src/displaymanagermsgs/data/GResponseMsgData.h"
#include "control/src/displaymanagermsgs/factory/GMsgCmdDataFactory.h"
#include "utils/logger/src/GLogger.h"

#include <memory>

GFlyTextMsgCommand::GFlyTextMsgCommand(GAbsData * pData)
	: GAbsMsgCommand(pData)
{
	LOG_LEVEL4("GFlyTextMsgCommand()");
}

GFlyTextMsgCommand::~GFlyTextMsgCommand()
{
	LOG_LEVEL4("~GFlyTextMsgCommand()");
}

void GFlyTextMsgCommand::Execute() throw(GException)
{
	LOG_LEVEL4("Execute()");
	
	// Dummy Response
	std::auto_ptr<GResponseMsgData> poResponse(CreateInitializedResponse(GFlyTextMsgDataXML::CXMLTag, GMsgCmdDataFactory::CInterfaceVersion));
	poResponse->SetResponseCode(GResponseMsgData::eOk);
	
	SetResponseNamespace(poResponse);
	m_pResponse=poResponse.release();
}
