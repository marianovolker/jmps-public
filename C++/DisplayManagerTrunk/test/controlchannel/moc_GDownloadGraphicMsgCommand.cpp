#include "control/src/displaymanagermsgs/command/GDownloadGraphicMsgCommand.h"

#include <QFileInfo>
#include <memory>

#include "control/src/displaymanagermsgs/xml/GDownloadGraphicMsgDataXML.h"
#include "control/src/displaymanagermsgs/data/GResponseMsgData.h"
#include "control/src/displaymanagermsgs/factory/GMsgCmdDataFactory.h"
#include "utils/image/src/GImageMappings.h"
#include "utils/image/src/GImageConverter.h"

GDownloadGraphicMsgCommand::GDownloadGraphicMsgCommand(GAbsData * pData)
	: GAbsMsgCommand(pData)
{
	LOG_LEVEL4("GDownloadGraphicMsgCommand()");
}

GDownloadGraphicMsgCommand::~GDownloadGraphicMsgCommand()
{
	LOG_LEVEL4("~GDownloadGraphicMsgCommand()");
}

void GDownloadGraphicMsgCommand::Execute() throw(GException)
{
	LOG_LEVEL4("Execute()");

	std::auto_ptr<GResponseMsgData> poResponse(new GResponseMsgData());
	if (poResponse.get() == NULL)
	{
		QString strExMsg("Error creating response message for ");
		strExMsg += GDownloadGraphicMsgDataXML::CXMLTag;
		strExMsg += " message.";
		throw GEXCEPTION(strExMsg);
	}

	poResponse->SetInterfaceVersion(GMsgCmdDataFactory::CInterfaceVersion);
	poResponse->SetCommand(GDownloadGraphicMsgDataXML::CXMLTag);
	poResponse->SetResponseCode(GResponseMsgData::eOk);

	{
		GAbsMsgData * pRequestData(static_cast<GAbsMsgData *>(m_pData));
		if (pRequestData != NULL)
		{
			poResponse->SetNamespacePrefix(pRequestData->GetNamespacePrefix());
			poResponse->SetNamespaceURI(pRequestData->GetNamespaceURI());
		}
	}

	m_pResponse = poResponse.release();
}
