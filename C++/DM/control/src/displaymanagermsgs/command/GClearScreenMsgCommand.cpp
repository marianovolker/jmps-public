#include "GClearScreenMsgCommand.h"

#include "control/src/displaymanagermsgs/xml/GClearScreenMsgDataXML.h"
#include "control/src/displaymanagermsgs/data/GResponseMsgData.h"
#include "control/src/displaymanagermsgs/factory/GMsgCmdDataFactory.h"
#include "utils/logger/src/GLogger.h"

#include <memory>

GClearScreenMsgCommand::GClearScreenMsgCommand(GAbsData * pData)
	: GAbsMsgCommand(pData)
{
	LOG_LEVEL4("GClearScreenMsgCommand()");
}

GClearScreenMsgCommand::~GClearScreenMsgCommand()
{
	LOG_LEVEL4("~GClearScreenMsgCommand()");
}

void GClearScreenMsgCommand::Execute() throw(GException)
{
	LOG_LEVEL4("Execute()");
	
	// Dummy Response
	std::auto_ptr<GResponseMsgData> poResponse(CreateInitializedResponse(GClearScreenMsgDataXML::CXMLTag, GMsgCmdDataFactory::CInterfaceVersion));
	poResponse->SetResponseCode(GResponseMsgData::eOk);
	
	SetResponseNamespace(poResponse);
	m_pResponse=poResponse.release();
}
