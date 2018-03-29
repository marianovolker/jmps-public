#include "GGdiRefreshScreenMsgCommand.h"

#include "model/src/gdimodel/GGDIModelSingleton.h"
#include "control/src/displaymanagermsgs/xml/GGdiRefreshScreenMsgDataXML.h"
#include "control/src/displaymanagermsgs/data/GResponseMsgData.h"
#include "control/src/displaymanagermsgs/factory/GMsgCmdDataFactory.h"
#include "utils/logger/src/GLogger.h"

#include <QMutexLocker>
#include <memory>

GGdiRefreshScreenMsgCommand::GGdiRefreshScreenMsgCommand(GAbsData * pData)
	: GAbsMsgCommand(pData)
{
	LOG_LEVEL4("GGdiRefreshScreenMsgCommand()");
}

GGdiRefreshScreenMsgCommand::~GGdiRefreshScreenMsgCommand()
{	
	LOG_LEVEL4("~GGdiRefreshScreenMsgCommand()");
}

void GGdiRefreshScreenMsgCommand::Execute() throw(GException)
{
	LOG_LEVEL4("Execute()");
	
	std::auto_ptr<GResponseMsgData> poResponse(CreateInitializedResponse(GGdiRefreshScreenMsgDataXML::CXMLTag, GMsgCmdDataFactory::CInterfaceVersion));

	try
	{
		GGdiRefreshScreenMsgData *pRequestData(static_cast<GGdiRefreshScreenMsgData *>(m_pData));
		if( pRequestData == NULL )
		{
			throw GEXCEPTION("Null pointer for GGdiRefreshScreenMsgData *pRequestData");
		}
		pRequestData->Validate();

		QSharedPointer<GGDIModel> pModel(GGDIModelSingleton::GetInstance()->GetModel());
        {
            QMutexLocker mtxScope(pModel->GetMutex());
            pModel->UpdateView(pRequestData->GetSynchronousRequest());
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
