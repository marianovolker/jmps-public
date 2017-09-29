#include "GGetPersonalitiesMsgCommand.h"

#include "control/src/displaymanagermsgs/xml/GGetPersonalitiesMsgDataXML.h"
#include "control/src/displaymanagermsgs/data/GResponseMsgData.h"
#include "control/src/displaymanagermsgs/factory/GMsgCmdDataFactory.h"
#include "config/GDisplayManagerConfig.h"
#include "utils/personality/src/GPersonality.h"
#include "utils/logger/src/GLogger.h"

#include <QStringList>
#include <memory>

GGetPersonalitiesMsgCommand::GGetPersonalitiesMsgCommand(GAbsData * pData)
	: GAbsMsgCommand(pData)
{
	LOG_LEVEL4("GGetPersonalitiesMsgCommand()");
}

GGetPersonalitiesMsgCommand::~GGetPersonalitiesMsgCommand()
{
	LOG_LEVEL4("~GGetPersonalitiesMsgCommand()");
}

void GGetPersonalitiesMsgCommand::Execute() throw(GException)
{
	LOG_LEVEL4("Execute()");
	
	std::auto_ptr<GResponseMsgData> poResponse(CreateInitializedResponse(GGetPersonalitiesMsgDataXML::CXMLTag, GMsgCmdDataFactory::CInterfaceVersion));
	
	try
	{
		GGetPersonalitiesMsgData *pRequestData(static_cast<GGetPersonalitiesMsgData *>(m_pData));
		if( pRequestData == NULL )
		{
			throw GEXCEPTION("Null pointer for GGetPersonalitiesMsgData *pRequestData");
		}
		pRequestData->Validate();

		QByteArray strResult;
		QChar const aUnicodeAsNullChar(0x0000);
		bool bFirst(true);

		strResult += GETCONF_STR_FROM_FILESYSTEM(eDisplayManagerPersonalityName);
		strResult.append(aUnicodeAsNullChar);
		strResult += GETCONF_STR(eDisplayManagerPersonalityName);
		strResult.append(aUnicodeAsNullChar);

		foreach( QString str, GPersonality::GetPersonalityList() )
		{
				if( bFirst )
				{
					strResult += str;
					bFirst = false;
				}
				else
				{
					strResult += "," + str;
				}
		}
		strResult.append(aUnicodeAsNullChar);

		poResponse->SetData(strResult.toHex());
		poResponse->SetResponseCode(GResponseMsgData::eOk);
	}
	catch( GException& e )
	{
		LOG_LEVEL1(QString("Excepcion: ")+e.GetMessage());

		poResponse->SetResponseCode(GResponseMsgData::eCantGetPersonalityList);
	}

	SetResponseNamespace(poResponse);
	m_pResponse=poResponse.release();
}
