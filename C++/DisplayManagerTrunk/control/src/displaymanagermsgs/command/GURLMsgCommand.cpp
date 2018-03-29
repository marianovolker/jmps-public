#include "GURLMsgCommand.h"

#include "model/src/urlmodel/GURLModelSingleton.h"
#include "control/src/displaymanagermsgs/xml/GURLMsgDataXML.h"
#include "control/src/displaymanagermsgs/data/GResponseMsgData.h"
#include "control/src/displaymanagermsgs/factory/GMsgCmdDataFactory.h"
#include "control/src/displaymanagermsgs/utils/GSiteServerURLCommandsHelper.h"
#include "utils/logger/src/GLogger.h"

#include <QMutexLocker>
#include <memory>

#include <QUrl>
#include <QString>
#include <QPair>
#include <QList>
#include <QStringList>

GURLMsgCommand::GURLMsgCommand(GAbsData * pData)
	: GAbsMsgCommand(pData)
{
	LOG_LEVEL4("GURLMsgCommand()");
}

GURLMsgCommand::~GURLMsgCommand()
{
	LOG_LEVEL4("~GURLMsgCommand()");
}

void GURLMsgCommand::Execute() throw(GException)
{
	LOG_LEVEL4("Execute()");

	std::auto_ptr<GResponseMsgData> poResponse(CreateInitializedResponse(GURLMsgDataXML::CXMLTag, GMsgCmdDataFactory::CInterfaceVersion));

	try
	{
		GURLMsgData *pRequestData(static_cast<GURLMsgData *>(m_pData));
		if( pRequestData == NULL )
		{
			throw GEXCEPTION("Null pointer for GURLMsgData *pRequestData");
		}
		pRequestData->Validate();

		LOG_LEVEL4(pRequestData->GetURL());


		GSiteServerURLCommandsHelper oSiteServerCmdHelper(pRequestData->GetURL());

		oSiteServerCmdHelper.FirstLevelParse();

		LOG_LEVEL4(oSiteServerCmdHelper.GetSecondURL());

		if (!oSiteServerCmdHelper.GetSecondURL().isEmpty())
		{
			QSharedPointer<GURLModel> pModel(GURLModelSingleton::GetInstance()->GetModel());
			{
				QMutexLocker mtxScope(pModel->GetMutex());

				pModel->SetUrl(oSiteServerCmdHelper.GetSecondURL());

				pModel->UpdateView(true); //Always Synchronized.

			}
		}
		
		oSiteServerCmdHelper.ProcessMediaCommands();

        poResponse->SetResponseCode(GResponseMsgData::eOk);
	}
	catch( GURLException& e )
	{
		LOG_LEVEL1(QString("Url Excepcion: ")+e.GetMessage());

		poResponse->SetResponseCode(GResponseMsgData::eURLError);
	}
	catch( GException& e )
	{
		LOG_LEVEL1(QString("Generic Excepcion: ")+e.GetMessage());

		poResponse->SetResponseCode(GResponseMsgData::eURLError);
	}

	SetResponseNamespace(poResponse);
	m_pResponse=poResponse.release();
}
