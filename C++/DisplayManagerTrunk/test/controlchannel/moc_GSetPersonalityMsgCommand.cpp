#include "control/src/displaymanagermsgs/command/GSetPersonalityMsgCommand.h"

#include <memory>

#include "control/src/displaymanagermsgs/xml/GSetPersonalityMsgDataXML.h"
#include "control/src/displaymanagermsgs/data/GResponseMsgData.h"
#include "control/src/displaymanagermsgs/factory/GMsgCmdDataFactory.h"
#include "config/GDisplayManagerConfig.h"
#include "utils/personality/src/GPersonality.h"
#include "utils/logger/src/GLogger.h"

#include "model/src/infoscreenmodel/GISModelSingleton.h"

GSetPersonalityMsgCommand::GSetPersonalityMsgCommand(GAbsData * pData)
	: GAbsMsgCommand(pData)
{
	LOG_LEVEL4("GSetPersonalityMsgCommand()");
}

GSetPersonalityMsgCommand::~GSetPersonalityMsgCommand()
{
	LOG_LEVEL4("~GSetPersonalityMsgCommand()");
}

void GSetPersonalityMsgCommand::Execute() throw(GException)
{
	LOG_LEVEL4("Execute()");
	
	std::auto_ptr<GResponseMsgData> poResponse(new GResponseMsgData());
	if (poResponse.get() == NULL)
	{
		QString strExMsg("Error creating response message for ");
		strExMsg += GSetPersonalityMsgDataXML::CXMLTag;
		strExMsg += " message.";
		throw GEXCEPTION(strExMsg);
	}

	poResponse->SetInterfaceVersion(GMsgCmdDataFactory::CInterfaceVersion);
	poResponse->SetCommand(GSetPersonalityMsgDataXML::CXMLTag);
	poResponse->SetResponseCode(GResponseMsgData::eOk);
	
	{
		GAbsMsgData * pRequestData(static_cast<GAbsMsgData *>(m_pData));
		if (pRequestData != NULL)
		{
			poResponse->SetNamespacePrefix(pRequestData->GetNamespacePrefix());
			poResponse->SetNamespaceURI(pRequestData->GetNamespaceURI());
		}
	}

	m_pResponse = poResponse.release();
}
