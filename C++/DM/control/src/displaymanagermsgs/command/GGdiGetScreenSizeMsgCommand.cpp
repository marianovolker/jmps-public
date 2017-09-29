#include "GGdiGetScreenSizeMsgCommand.h"

#include "model/src/gdimodel/GGDIModelSingleton.h"
#include "control/src/displaymanagermsgs/xml/GGdiGetScreenSizeMsgDataXML.h"
#include "control/src/displaymanagermsgs/data/GResponseMsgData.h"
#include "control/src/displaymanagermsgs/factory/GMsgCmdDataFactory.h"
#include "config/GDisplayManagerConfig.h"
#include "utils/logger/src/GLogger.h"

#include <memory>
#include <QByteArray>
#include <QChar>

GGdiGetScreenSizeMsgCommand::GGdiGetScreenSizeMsgCommand(GAbsData * pData)
	: GAbsMsgCommand(pData)
{
	LOG_LEVEL4("GGdiGetScreenSizeMsgCommand()");
}

GGdiGetScreenSizeMsgCommand::~GGdiGetScreenSizeMsgCommand()
{
	LOG_LEVEL4("~GGdiGetScreenSizeMsgCommand()");
}

void GGdiGetScreenSizeMsgCommand::Execute() throw(GException)
{
	LOG_LEVEL4("Execute()");
	
	std::auto_ptr<GResponseMsgData> poResponse(CreateInitializedResponse(GGdiGetScreenSizeMsgDataXML::CXMLTag, GMsgCmdDataFactory::CInterfaceVersion));

	try
	{
		GGdiGetScreenSizeMsgData * pRequestData(static_cast<GGdiGetScreenSizeMsgData *>(m_pData));
		if( pRequestData == NULL )
		{
			throw GEXCEPTION("Null pointer for GGdiGetScreenSizeMsgData *pRequestData");
		}
		pRequestData->Validate();

		QByteArray strData;
		QChar const aUnicodeAsNullChar(0x0000);

		strData.append(QString().sprintf("%04d", GETCONF_NUM(eDisplayManagerViewWidth)));
		strData.append(aUnicodeAsNullChar);
		strData.append(QString().sprintf("%04d", GETCONF_NUM(eDisplayManagerViewHeight)));
		strData.append(aUnicodeAsNullChar);

		poResponse->SetData(strData.toHex());
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
