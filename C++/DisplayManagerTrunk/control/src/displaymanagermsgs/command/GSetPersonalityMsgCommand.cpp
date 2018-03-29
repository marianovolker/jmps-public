#include "GSetPersonalityMsgCommand.h"

#include "control/src/displaymanagermsgs/xml/GSetPersonalityMsgDataXML.h"
#include "control/src/displaymanagermsgs/data/GResponseMsgData.h"
#include "control/src/displaymanagermsgs/factory/GMsgCmdDataFactory.h"
#include "config/GDisplayManagerConfig.h"
#include "utils/logger/src/GLogger.h"
#include "view/src/utils/GPersonalityView.h"

#include <memory>

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
	
	std::auto_ptr<GResponseMsgData> poResponse(CreateInitializedResponse(GSetPersonalityMsgDataXML::CXMLTag, GMsgCmdDataFactory::CInterfaceVersion));
	
	try
	{
		GSetPersonalityMsgData *pRequestData(static_cast<GSetPersonalityMsgData *>(m_pData));
		if( pRequestData == NULL )
		{
			throw GEXCEPTION("Null pointer for GSetPersonalityMsgData *pRequestData");
		}
		pRequestData->Validate();

		QString strPersonality("");
		static QString const qsDefaultPersonality("default");

		if( pRequestData->GetPersonality() == qsDefaultPersonality )
		{
			strPersonality = GETCONF_STR_FROM_FILESYSTEM(eDisplayManagerPersonalityName);
		}
		else if( GPersonality::IsAValidPersonality(pRequestData->GetPersonality()) )
		{
			strPersonality = pRequestData->GetPersonality();
		}

		if( (!strPersonality.isEmpty()) && (SETCONF(eDisplayManagerPersonalityName, strPersonality)) )
		{
			poResponse->SetResponseCode(GResponseMsgData::eOk);

			QSharedPointer<GPersonalityView> pGPersonalityView = GPersonalityView::GetInstance();

			pGPersonalityView->RefreshPersonality();
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
