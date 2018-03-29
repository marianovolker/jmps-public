#include "GGdiDrawLineMsgCommand.h"

#include "model/src/gdimodel/GGDIModelSingleton.h"
#include "control/src/displaymanagermsgs/xml/GGdiDrawLineMsgDataXML.h"
#include "control/src/displaymanagermsgs/data/GResponseMsgData.h"
#include "control/src/displaymanagermsgs/factory/GMsgCmdDataFactory.h"
#include "model/src/gdimodel/GGDILine.h"

#include "utils/logger/src/GLogger.h"
#include <QMutexLocker>

#include <memory>

GGdiDrawLineMsgCommand::GGdiDrawLineMsgCommand(GAbsData * pData)
	: GAbsMsgCommand(pData)
{
	LOG_LEVEL4("GGdiDrawLineMsgCommand()");
}

GGdiDrawLineMsgCommand::~GGdiDrawLineMsgCommand()
{
	LOG_LEVEL4("~GGdiDrawLineMsgCommand()");
}

void GGdiDrawLineMsgCommand::Execute() throw(GException)
{
	LOG_LEVEL4("Execute()");
	
	std::auto_ptr<GResponseMsgData> poResponse(CreateInitializedResponse(GGdiDrawLineMsgDataXML::CXMLTag, GMsgCmdDataFactory::CInterfaceVersion));

	try
	{
		GGdiDrawLineMsgData *pRequestData(static_cast<GGdiDrawLineMsgData *>(m_pData));
		if( pRequestData == NULL )
		{
			throw GEXCEPTION("Null pointer for GGdiDrawLineMsgData *pRequestData");
		}
		pRequestData->Validate();

		GGDILine oLine;
		oLine.SetThickness(pRequestData->GetThickness());
		oLine.SetColor(pRequestData->GetColor());
		oLine.SetLength(pRequestData->GetLength());
		oLine.SetXLoc(pRequestData->GetXLoc());
		oLine.SetYLoc(pRequestData->GetYLoc());

		QSharedPointer<GGDIModel> pModel(GGDIModelSingleton::GetInstance()->GetModel());
        {
            QMutexLocker mtxScope(pModel->GetMutex());
            pModel->DrawLine(oLine, pRequestData->GetSynchronousRequest());
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
