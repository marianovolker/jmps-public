#include "GKeyEventMsgCommand.h"

#include "control/src/displaymanagermsgs/xml/GKeyEventMsgDataXML.h"
#include "control/src/displaymanagermsgs/data/GResponseMsgData.h"
#include "control/src/displaymanagermsgs/factory/GMsgCmdDataFactory.h"
#include "utils/logger/src/GLogger.h"

#include <memory>

GKeyEventMsgCommand::GKeyEventMsgCommand(GAbsData * pData)
	: GAbsMsgCommand(pData)
{
	LOG_LEVEL4("GKeyEventMsgCommand()");
}

GKeyEventMsgCommand::~GKeyEventMsgCommand()
{
	LOG_LEVEL4("~GKeyEventMsgCommand()");
}

void GKeyEventMsgCommand::Execute() throw(GException)
{
	LOG_LEVEL4("Execute()");
	
	// Dummy Response
	std::auto_ptr<GResponseMsgData> poResponse(CreateInitializedResponse(GKeyEventMsgDataXML::CXMLTag, GMsgCmdDataFactory::CInterfaceVersion));
	poResponse->SetResponseCode(GResponseMsgData::eOk);
	
	SetResponseNamespace(poResponse);
	m_pResponse=poResponse.release();
}
