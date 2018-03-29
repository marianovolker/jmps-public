#include "GSetDefaultPersonalityMsgCommand.h"

#include "control/src/displaymanagermsgs/xml/GSetDefaultPersonalityMsgDataXML.h"
#include "control/src/displaymanagermsgs/data/GResponseMsgData.h"
#include "control/src/displaymanagermsgs/factory/GMsgCmdDataFactory.h"
#include "config/GDisplayManagerConfig.h"
#include "utils/personality/src/GPersonality.h"
#include "utils/logger/src/GLogger.h"

#include <memory>

GSetDefaultPersonalityMsgCommand::GSetDefaultPersonalityMsgCommand(GAbsData * pData)
	: GAbsMsgCommand(pData)
{
	LOG_LEVEL4("GSetDefaultPersonalityMsgCommand()");
}

GSetDefaultPersonalityMsgCommand::~GSetDefaultPersonalityMsgCommand()
{
	LOG_LEVEL4("~GSetDefaultPersonalityMsgCommand()");
}

void GSetDefaultPersonalityMsgCommand::Execute() throw(GException)
{
	LOG_LEVEL4("Execute()");
	
	std::auto_ptr<GResponseMsgData> poResponse(CreateInitializedResponse(GSetDefaultPersonalityMsgDataXML::CXMLTag, GMsgCmdDataFactory::CInterfaceVersion));
	
	try
	{
		GSetDefaultPersonalityMsgData *pRequestData(static_cast<GSetDefaultPersonalityMsgData *>(m_pData));
		if( pRequestData == NULL )
		{
			throw GEXCEPTION("Null pointer for GSetDefaultPersonalityMsgData *pRequestData");
		}
		pRequestData->Validate();

		if( GPersonality::IsAValidPersonality(pRequestData->GetPersonality()) && SETCONF_TO_FILESYSTEM(eDisplayManagerPersonalityName, pRequestData->GetPersonality()) )
		{
			poResponse->SetResponseCode(GResponseMsgData::eOk);
		}
		else
		{
			poResponse->SetResponseCode(GResponseMsgData::eInvalidPersonality);
		}
	}
	catch( GException& e )
	{
		LOG_LEVEL1(QString("Excepcion: ")+e.GetMessage());

		poResponse->SetResponseCode(GResponseMsgData::eInvalidPersonality);
	}

	SetResponseNamespace(poResponse);
	m_pResponse=poResponse.release();
}
