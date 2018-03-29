#include "GGdiFillRectMsgCommand.h"

#include "model/src/gdimodel/GGDIModelSingleton.h"
#include "control/src/displaymanagermsgs/xml/GGdiFillRectMsgDataXML.h"
#include "control/src/displaymanagermsgs/data/GResponseMsgData.h"
#include "control/src/displaymanagermsgs/factory/GMsgCmdDataFactory.h"
#include "model/src/gdimodel/GGDIRect.h"

#include "utils/logger/src/GLogger.h"
#include <QMutexLocker>

#include <memory>

GGdiFillRectMsgCommand::GGdiFillRectMsgCommand(GAbsData * pData)
	: GAbsMsgCommand(pData)
{
	LOG_LEVEL4("GGdiFillRectMsgCommand()");
}

GGdiFillRectMsgCommand::~GGdiFillRectMsgCommand()
{
	LOG_LEVEL4("~GGdiFillRectMsgCommand()");
}

void GGdiFillRectMsgCommand::Execute() throw(GException)
{
	LOG_LEVEL4("Execute()");
	
	std::auto_ptr<GResponseMsgData> poResponse(CreateInitializedResponse(GGdiFillRectMsgDataXML::CXMLTag, GMsgCmdDataFactory::CInterfaceVersion));

	try
	{
		GGdiFillRectMsgData * pRequestData(static_cast<GGdiFillRectMsgData *>(m_pData));
		if( pRequestData == NULL )
		{
			throw GEXCEPTION("Null pointer for GGdiFillRectMsgData *pRequestData");
		}
		pRequestData->Validate();

		GGDIRect oRect;
		oRect.SetColor(pRequestData->GetColor());
		oRect.SetXLoc(pRequestData->GetXLoc());
		oRect.SetYLoc(pRequestData->GetYLoc());
		oRect.SetHeight(pRequestData->GetHeight());
		oRect.SetWidth(pRequestData->GetWidth());

		QSharedPointer<GGDIModel> pModel(GGDIModelSingleton::GetInstance()->GetModel());
        {
            QMutexLocker mtxScope(pModel->GetMutex());
            pModel->DrawRect(oRect, pRequestData->GetSynchronousRequest());
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
