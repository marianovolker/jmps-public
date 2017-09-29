#include "GInfoScreenMsgCommand.h"

#include "control/src/displaymanagermsgs/xml/GInfoScreenMsgDataXML.h"
#include "control/src/displaymanagermsgs/data/GResponseMsgData.h"
#include "control/src/displaymanagermsgs/factory/GMsgCmdDataFactory.h"
#include "control/src/infoscreenmsg/utils/GISException.h"
#include "control/src/infoscreenmsg/base/GISAbsCommand.h"
#include "control/src/infoscreenmsg/utils/GISCommandStatusEnum.h"
#include "control/src/infoscreenmsg/data/GISCommandResponse.h"
#include "utils/logger/src/GLogger.h"

#include <QSharedPointer>
#include <memory>

GInfoScreenMsgCommand::GInfoScreenMsgCommand(GAbsData * pData)
	: GAbsMsgCommand(pData),
	  m_poISMsgCmdDataFactory( GISMsgCmdDataFactory::GetInstance() )
{
	LOG_LEVEL4("GInfoScreenMsgCommand()");

	if (m_poISMsgCmdDataFactory == NULL)
	{
		QString strExMsg("Error instantiating ISMsgCmdDataFactory for ");
		strExMsg += GInfoScreenMsgDataXML::CXMLTag;
		strExMsg += " message.";
		throw GEXCEPTION(strExMsg);
	}
}

GInfoScreenMsgCommand::~GInfoScreenMsgCommand()
{
	LOG_LEVEL4("~GInfoScreenMsgCommand()");
}

void GInfoScreenMsgCommand::Execute() throw(GException)
{
	LOG_LEVEL4("Execute()");

	std::auto_ptr<GResponseMsgData> poResponse(CreateInitializedResponse(GInfoScreenMsgDataXML::CXMLTag, GMsgCmdDataFactory::CInterfaceVersion));

	try
	{
		GInfoScreenMsgData * pRequestData(static_cast<GInfoScreenMsgData *>(m_pData));
		if( pRequestData == NULL )
		{
			throw GEXCEPTION("Null pointer for GInfoScreenMsgData *pRequestData");
		}
		pRequestData->Validate();

		QByteArray oData = pRequestData->GetData();
		QSharedPointer<GISAbsCommand> poCommand(m_poISMsgCmdDataFactory->Create(oData));
		if( poCommand.isNull() )
		{
			throw GEXCEPTION("Null pointer for GISAbsCommand *poCommand");
		}

		// Pass Request-Synchronous flag to the next level (infoscreen message type).
		poCommand->GetData()->SetSynchronousRequest(pRequestData->GetSynchronousRequest());
		poCommand->Execute();

		GISCommandResponse * pISResponse(static_cast<GISCommandResponse *>(poCommand->GetResponse()));
		if( pISResponse == NULL )
		{
			throw GEXCEPTION("Null pointer for GISCommandResponse * pISResponse");
		}

		poResponse->SetData(pISResponse->ToString().toUtf8());

		// Fill level 1 response...
		if( (pISResponse->GetCommandStatus().GetError() == GISCommandStatusEnum::eNoErrors) ||
		    (pISResponse->GetCommandStatus().GetError() == GISCommandStatusEnum::eApplicationSaveComplete))
		{
			poResponse->SetResponseCode(GResponseMsgData::eOk);
		}
		else
		{
			poResponse->SetResponseCode(GResponseMsgData::eInfoScreenError);
		}
	}
	catch( GException& e )
	{
		LOG_LEVEL1(QString("Excepcion: ")+e.GetMessage());

		poResponse->SetResponseCode(GResponseMsgData::eInfoScreenError);
	}

	SetResponseNamespace(poResponse);
	m_pResponse = poResponse.release();
}
