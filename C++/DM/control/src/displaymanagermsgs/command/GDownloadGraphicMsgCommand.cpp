#include "GDownloadGraphicMsgCommand.h"

#include "control/src/displaymanagermsgs/xml/GDownloadGraphicMsgDataXML.h"
#include "control/src/displaymanagermsgs/data/GResponseMsgData.h"
#include "control/src/displaymanagermsgs/factory/GMsgCmdDataFactory.h"
#include "utils/image/src/GImageMappings.h"
#include "utils/image/src/GImageConverter.h"
#include "utils/image/src/GImageHelper.h"

#include <memory>
#include <QFileInfo>
#include <QSharedPointer>
#include <QMutexLocker>

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

	std::auto_ptr<GResponseMsgData> poResponse(CreateInitializedResponse(GDownloadGraphicMsgDataXML::CXMLTag, GMsgCmdDataFactory::CInterfaceVersion));
	
	try
	{
		GDownloadGraphicMsgData *pRequestData(static_cast<GDownloadGraphicMsgData *>(m_pData));
		if( pRequestData == NULL )
		{
			throw GEXCEPTION("GInfoScreenMsgData is NULL.");
		}
		pRequestData->Validate();

		if( pRequestData->GetType() == GDownloadGraphicMsgData::st_CRequestMsgImageType )
		{
			LOG_LEVEL3("DownloadGraphic command of type: " + GDownloadGraphicMsgData::st_CRequestMsgImageType + ". Preparing image conversion... ");

			const QString CstrCRCReq(pRequestData->GetCRC());
			const int CiSegmentReq = (pRequestData->GetSegment()).toInt();
			const QString CstrOutputFileName(CstrCRCReq + QString(".") + GImageConverter::st_CPNGImageFormat);
			const QString CstrFullOutputFileName(GImageHelper::CurrentResolutionMonochromeImagesPath() + CstrOutputFileName);
			
			std::auto_ptr<GImageMap> poImageMap(new GImageMap(CstrCRCReq, CstrOutputFileName, CiSegmentReq, GImageConverter::st_CPNGImageFormat));

			QSharedPointer<GImageMappings> poImageMappings(GImageMappings::GetInstance());
            {
                QMutexLocker mtxImageMappingsScope(poImageMappings->GetMutex());
                poImageMappings->AddImageMap(poImageMap);
            }

            std::auto_ptr<GImageJob> poImageJob(new GImageJob(CstrCRCReq, pRequestData->GetFilename(), CiSegmentReq, GImageConverter::st_CBMPImageFormat));
            poImageJob->SetOutputFile(CstrFullOutputFileName);
            poImageJob->SetBackupFileName(GImageHelper::RawImagesPath() + CstrCRCReq + QString(".") + GImageConverter::st_CBMPImageFormat);

            //Adding a task to convert an image to the Image Converter
            QSharedPointer<GImageConverter> oImageConverter = GImageConverter::GetInstance();
			oImageConverter->AddImageJob(poImageJob.release());
		}
		else
		{
			LOG_LEVEL3("DownloadGraphic command is not of type: " + GDownloadGraphicMsgData::st_CRequestMsgImageType + ". No image processing.");
		}

		poResponse->SetResponseCode(GResponseMsgData::eOk);
	}
	catch( GException& e )
	{
		LOG_LEVEL1(QString("Excepcion: ")+e.GetMessage());

		poResponse->SetResponseCode(GResponseMsgData::eError);
	}
	
	SetResponseNamespace(poResponse);
	m_pResponse=poResponse.release();
}
