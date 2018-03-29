#include "GGdiDrawTextMsgCommand.h"

#include "model/src/gdimodel/GGDIModelSingleton.h"
#include "control/src/displaymanagermsgs/xml/GGdiDrawTextMsgDataXML.h"
#include "control/src/displaymanagermsgs/data/GResponseMsgData.h"
#include "control/src/displaymanagermsgs/factory/GMsgCmdDataFactory.h"
#include "model/src/gdimodel/GGDIText.h"

#include "utils/logger/src/GLogger.h"
#include <QMutexLocker>

#include <memory>

GGdiDrawTextMsgCommand::GGdiDrawTextMsgCommand(GAbsData * pData)
	: GAbsMsgCommand(pData)
{
	LOG_LEVEL4("GGdiDrawTextMsgCommand()");
}

GGdiDrawTextMsgCommand::~GGdiDrawTextMsgCommand()
{
	LOG_LEVEL4("~GGdiDrawTextMsgCommand()");
}

void GGdiDrawTextMsgCommand::Execute() throw(GException)
{
	LOG_LEVEL3("Execute()");
	
	std::auto_ptr<GResponseMsgData> poResponse(CreateInitializedResponse(GGdiDrawTextMsgDataXML::CXMLTag, GMsgCmdDataFactory::CInterfaceVersion));

	try
	{
		GGdiDrawTextMsgData *pRequestData(static_cast<GGdiDrawTextMsgData *>(m_pData));
		if( pRequestData == NULL )
		{
			throw GEXCEPTION("Null pointer for GGdiDrawTextMsgData *pRequestData");
		}
		pRequestData->Validate();

		GGDIText oText;
		oText.SetBGColor(pRequestData->GetBGColor());
		oText.SetFGColor(pRequestData->GetFGColor());
		oText.SetText(pRequestData->GetText());
		oText.SetXLoc(pRequestData->GetXLoc());
		oText.SetYLoc(pRequestData->GetYLoc());
		oText.SetFont(pRequestData->GetFont());
		oText.SetAttr(pRequestData->GetAttr());

		QSharedPointer<GGDIModel> pModel(GGDIModelSingleton::GetInstance()->GetModel());
        {
            QMutexLocker mtxScope(pModel->GetMutex());
            pModel->DrawText(oText, pRequestData->GetSynchronousRequest());
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
