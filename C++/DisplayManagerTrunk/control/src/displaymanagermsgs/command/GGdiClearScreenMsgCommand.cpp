#include "GGdiClearScreenMsgCommand.h"

#include "model/src/gdimodel/GGDIModelSingleton.h"
#include "control/src/displaymanagermsgs/xml/GGdiClearScreenMsgDataXML.h"
#include "control/src/displaymanagermsgs/data/GResponseMsgData.h"
#include "control/src/displaymanagermsgs/factory/GMsgCmdDataFactory.h"

#include "utils/logger/src/GLogger.h"
#include <QMutexLocker>

#include <memory>

GGdiClearScreenMsgCommand::GGdiClearScreenMsgCommand(GAbsData * pData)
	: GAbsMsgCommand(pData)
{
	LOG_LEVEL4("GGdiClearScreenMsgCommand()");
}

GGdiClearScreenMsgCommand::~GGdiClearScreenMsgCommand()
{
	LOG_LEVEL4("~GGdiClearScreenMsgCommand()");
}

void GGdiClearScreenMsgCommand::Execute() throw(GException)
{
	LOG_LEVEL4("Execute()");
	
	std::auto_ptr<GResponseMsgData> poResponse(CreateInitializedResponse(GGdiClearScreenMsgDataXML::CXMLTag, GMsgCmdDataFactory::CInterfaceVersion));

	try
	{
		GGdiClearScreenMsgData *pRequestData(static_cast<GGdiClearScreenMsgData *>(m_pData));
		if( pRequestData == NULL )
		{
			throw GEXCEPTION("GGdiClearScreenMsgData is NULL.");
		}
		pRequestData->Validate();

		QSharedPointer<GGDIModel> pModel(GGDIModelSingleton::GetInstance()->GetModel());
        {
            QMutexLocker mtxScope(pModel->GetMutex());
            pModel->ClearView(pRequestData->GetSynchronousRequest());
        }

		poResponse->SetResponseCode(GResponseMsgData::eOk);
	}
	catch( GException& e )
	{
		LOG_LEVEL1(QString("Excepcion: ")+e.GetMessage());

		poResponse->SetResponseCode(GResponseMsgData::eGDIError);
	}

	SetResponseNamespace(poResponse);
	m_pResponse=poResponse.release();
}

