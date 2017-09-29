#include "GISMsgCmdDataFactory.h"

#include <QString>

#include "control/src/infoscreenmsg/utils/GISCommandStatusEnum.h"
#include "control/src/infoscreenmsg/utils/GISException.h"

#include "control/src/infoscreenmsg/data/GISSaveApplicationMsgData.h"
#include "control/src/infoscreenmsg/data/GISAddFieldMsgData.h"
#include "control/src/infoscreenmsg/data/GISAddSegmentMsgData.h"
#include "control/src/infoscreenmsg/data/GISAddDisplayMsgData.h"
#include "control/src/infoscreenmsg/data/GISAddScreenMsgData.h"
#include "control/src/infoscreenmsg/data/GISAdjustColorPaletteMsgData.h"
#include "control/src/infoscreenmsg/data/GISDeleteDisplayMsgData.h"
#include "control/src/infoscreenmsg/data/GISClearScreenMsgData.h"
#include "control/src/infoscreenmsg/data/GISDeleteScreenMsgData.h"
#include "control/src/infoscreenmsg/data/GISDeleteFieldMsgData.h"
#include "control/src/infoscreenmsg/data/GISDeleteSegmentMsgData.h"
#include "control/src/infoscreenmsg/data/GISDisplayScreenMsgData.h"
#include "control/src/infoscreenmsg/data/GISDisplaySegmentMsgData.h"
#include "control/src/infoscreenmsg/data/GISSetScreenColorMsgData.h"
#include "control/src/infoscreenmsg/data/GISDisplayFlyTextMsgData.h"
#include "control/src/infoscreenmsg/data/GISModifyFieldMsgData.h"
#include "control/src/infoscreenmsg/data/GISModifySegmentMsgData.h"
#include "control/src/infoscreenmsg/data/GISSetFieldAttributeMsgData.h"
#include "control/src/infoscreenmsg/data/GISAdjustBacklightIntensityMsgData.h"
#include "control/src/infoscreenmsg/data/GISAdjustSoundVolumeMsgData.h"
#include "control/src/infoscreenmsg/data/GISEraseApplicationMsgData.h"
#include "control/src/infoscreenmsg/data/GISObjectAddressMsgData.h"
#include "control/src/infoscreenmsg/data/GISSegmentDisplayCountMsgData.h"
#include "control/src/infoscreenmsg/data/GISSetChromakeyColorMsgData.h"
#include "control/src/infoscreenmsg/data/GISSetGraphicsTypeMsgData.h"
#include "control/src/infoscreenmsg/data/GISStatusMsgData.h"
#include "control/src/infoscreenmsg/data/GISVideoFrameSeekMsgData.h"
#include "control/src/infoscreenmsg/data/GISVideoPlayMsgData.h"
#include "control/src/infoscreenmsg/data/GISVideoRangePlayMsgData.h"
#include "control/src/infoscreenmsg/data/GISVideoStopMsgData.h"
#include "control/src/infoscreenmsg/data/GISVideoViewMsgData.h"

#include "control/src/infoscreenmsg/command/GISSaveApplicationMsgCommand.h"
#include "control/src/infoscreenmsg/command/GISAddFieldMsgCommand.h"
#include "control/src/infoscreenmsg/command/GISAddSegmentMsgCommand.h"
#include "control/src/infoscreenmsg/command/GISAddDisplayMsgCommand.h"
#include "control/src/infoscreenmsg/command/GISAddScreenMsgCommand.h"
#include "control/src/infoscreenmsg/command/GISAdjustColorPaletteMsgCommand.h"
#include "control/src/infoscreenmsg/command/GISDeleteDisplayMsgCommand.h"
#include "control/src/infoscreenmsg/command/GISClearScreenMsgCommand.h"
#include "control/src/infoscreenmsg/command/GISDeleteScreenMsgCommand.h"
#include "control/src/infoscreenmsg/command/GISDeleteFieldMsgCommand.h"
#include "control/src/infoscreenmsg/command/GISDeleteSegmentMsgCommand.h"
#include "control/src/infoscreenmsg/command/GISDisplayScreenMsgCommand.h"
#include "control/src/infoscreenmsg/command/GISDisplaySegmentMsgCommand.h"
#include "control/src/infoscreenmsg/command/GISSetScreenColorMsgCommand.h"
#include "control/src/infoscreenmsg/command/GISDisplayFlyTextMsgCommand.h"
#include "control/src/infoscreenmsg/command/GISModifyFieldMsgCommand.h"
#include "control/src/infoscreenmsg/command/GISModifySegmentMsgCommand.h"
#include "control/src/infoscreenmsg/command/GISSetFieldAttributeMsgCommand.h"
#include "control/src/infoscreenmsg/command/GISAdjustBacklightIntensityMsgCommand.h"
#include "control/src/infoscreenmsg/command/GISAdjustSoundVolumeMsgCommand.h"
#include "control/src/infoscreenmsg/command/GISEraseApplicationMsgCommand.h"
#include "control/src/infoscreenmsg/command/GISObjectAddressMsgCommand.h"
#include "control/src/infoscreenmsg/command/GISSegmentDisplayCountMsgCommand.h"
#include "control/src/infoscreenmsg/command/GISSetChromakeyColorMsgCommand.h"
#include "control/src/infoscreenmsg/command/GISSetGraphicsTypeMsgCommand.h"
#include "control/src/infoscreenmsg/command/GISStatusMsgCommand.h"
#include "control/src/infoscreenmsg/command/GISVideoFrameSeekMsgCommand.h"
#include "control/src/infoscreenmsg/command/GISVideoPlayMsgCommand.h"
#include "control/src/infoscreenmsg/command/GISVideoRangePlayMsgCommand.h"
#include "control/src/infoscreenmsg/command/GISVideoStopMsgCommand.h"
#include "control/src/infoscreenmsg/command/GISVideoViewMsgCommand.h"

#include "utils/logger/src/GLogger.h"
#include "model/src/infoscreenmodel/GAttributeEnum.h"

GISMsgCmdDataFactory*  GISMsgCmdDataFactory::m_pGISMsgCmdDataFactory = NULL;
QMutex GISMsgCmdDataFactory::m_mutex;

GISMsgCmdDataFactory::GISMsgCmdDataFactory()
{
	LOG_LEVEL4("GISMsgCmdDataFactory()");
}

GISMsgCmdDataFactory::~GISMsgCmdDataFactory()
{
	LOG_LEVEL4("~GISMsgCmdDataFactory()");
}

GISMsgCmdDataFactory * GISMsgCmdDataFactory::GetInstance() throw(GException)
{
	LOG_LEVEL4("GetInstance()");

	m_mutex.lock();

	if( m_pGISMsgCmdDataFactory == NULL )
	{
		m_pGISMsgCmdDataFactory = new GISMsgCmdDataFactory();
	}

	m_mutex.unlock();

	if( m_pGISMsgCmdDataFactory == NULL )
	{
		 throw( GEXCEPTION("GISMsgCmdDataFactory::GetInstance() made a NULL pointer.") );
	}

	return m_pGISMsgCmdDataFactory;
}

GISAbsCommand * GISMsgCmdDataFactory::Create(QByteArray & oInBuffer) throw(GException)
{
	LOG_LEVEL3("Create()");

	GISAbsCommand* poISAbsCommand(NULL);
	
	QByteArray oInText = QByteArray::fromHex(oInBuffer.constData());
	
	QString strCmd = GetField(oInText, eCMDFieldNumber);	

	LOG_LEVEL3(QString("Command: ") + strCmd.toUtf8().data());
	
	if( GISSaveApplicationMsgData::m_CCommandId == strCmd )
	{
		poISAbsCommand = new GISSaveApplicationMsgCommand();
		poISAbsCommand->SetData( new GISSaveApplicationMsgData() );
	}
	else if( GISAddFieldMsgData::m_CCommandId == strCmd )
	{
		poISAbsCommand = new GISAddFieldMsgCommand();
		poISAbsCommand->SetData( new GISAddFieldMsgData() );
	}
	else if( GISAddSegmentMsgData::m_CCommandId == strCmd ) 
	{
		poISAbsCommand = new GISAddSegmentMsgCommand();
		poISAbsCommand->SetData( new GISAddSegmentMsgData() );		
	}
	else if( GISAddDisplayMsgData::m_CCommandId == strCmd ) 
	{
		poISAbsCommand = new GISAddDisplayMsgCommand();
		poISAbsCommand->SetData( new GISAddDisplayMsgData() );		
	}
	else if( GISAddScreenMsgData::m_CCommandId == strCmd )
	{
		poISAbsCommand = new GISAddScreenMsgCommand();
		poISAbsCommand->SetData( new GISAddScreenMsgData() );		
	}
	else if( GISAdjustColorPaletteMsgData::m_CCommandId == strCmd ) 
	{
		poISAbsCommand = new GISAdjustColorPaletteMsgCommand();
		poISAbsCommand->SetData( new GISAdjustColorPaletteMsgData() );		
	}
	else if( GISDeleteDisplayMsgData::m_CCommandId == strCmd ) 
	{
		poISAbsCommand = new GISDeleteDisplayMsgCommand();
		poISAbsCommand->SetData( new GISDeleteDisplayMsgData() );		
	}
	else if( GISClearScreenMsgData::m_CCommandId == strCmd ) 
	{
		poISAbsCommand = new GISClearScreenMsgCommand();
		poISAbsCommand->SetData( new GISClearScreenMsgData() );		
	}
	else if( GISDeleteScreenMsgData::m_CCommandId == strCmd ) 
	{
		poISAbsCommand = new GISDeleteScreenMsgCommand();
		poISAbsCommand->SetData( new GISDeleteScreenMsgData() );		
	}
	else if( GISDeleteFieldMsgData::m_CCommandId == strCmd ) 
	{
		poISAbsCommand = new GISDeleteFieldMsgCommand();
		poISAbsCommand->SetData( new GISDeleteFieldMsgData() );		
	}
	else if( GISDeleteSegmentMsgData::m_CCommandId == strCmd ) 
	{
		poISAbsCommand = new GISDeleteSegmentMsgCommand();
		poISAbsCommand->SetData( new GISDeleteSegmentMsgData() );		
	}
	else if( GISDisplayScreenMsgData::m_CCommandId == strCmd )
	{
		poISAbsCommand = new GISDisplayScreenMsgCommand();
		poISAbsCommand->SetData( new GISDisplayScreenMsgData() );		
	}
	else if( GISDisplaySegmentMsgData::m_CCommandId == strCmd ) 
	{
		poISAbsCommand = new GISDisplaySegmentMsgCommand();
		poISAbsCommand->SetData( new GISDisplaySegmentMsgData() );		
	}
	else if( GISSetScreenColorMsgData::m_CCommandId == strCmd )
	{
		poISAbsCommand = new GISSetScreenColorMsgCommand();
		poISAbsCommand->SetData( new GISSetScreenColorMsgData() );		
	}
	else if( GISDisplayFlyTextMsgData::m_CCommandId == strCmd ) 
	{
		poISAbsCommand = new GISDisplayFlyTextMsgCommand();
		poISAbsCommand->SetData( new GISDisplayFlyTextMsgData() );		
	}
	else if( GISModifyFieldMsgData::m_CCommandId == strCmd ) 
	{
		poISAbsCommand = new GISModifyFieldMsgCommand();
		poISAbsCommand->SetData( new GISModifyFieldMsgData() );		
	}
	else if( GISModifySegmentMsgData::m_CCommandId == strCmd )
	{
		poISAbsCommand = new GISModifySegmentMsgCommand();
		poISAbsCommand->SetData( new GISModifySegmentMsgData() );		
	}
	else if( GISSetFieldAttributeMsgData::m_CCommandId == strCmd ) 
	{
		poISAbsCommand = new GISSetFieldAttributeMsgCommand();
		poISAbsCommand->SetData( new GISSetFieldAttributeMsgData() );		
	}
	else if( GISAdjustBacklightIntensityMsgData::m_CCommandId == strCmd )
	{
		poISAbsCommand = new GISAdjustBacklightIntensityMsgCommand();
		poISAbsCommand->SetData( new GISAdjustBacklightIntensityMsgData() );		
	}		
	else if( GISAdjustSoundVolumeMsgData::m_CCommandId == strCmd )
	{
		poISAbsCommand = new GISAdjustSoundVolumeMsgCommand();
		poISAbsCommand->SetData( new GISAdjustSoundVolumeMsgData() );		
	}		
	else if( GISEraseApplicationMsgData::m_CCommandId == strCmd )
	{
		poISAbsCommand = new GISEraseApplicationMsgCommand();
		poISAbsCommand->SetData( new GISEraseApplicationMsgData() );		
	}		
	else if( GISObjectAddressMsgData::m_CCommandId == strCmd )
	{
		poISAbsCommand = new GISObjectAddressMsgCommand();
		poISAbsCommand->SetData( new GISObjectAddressMsgData() );		
	}		
	else if( GISSegmentDisplayCountMsgData::m_CCommandId == strCmd )
	{
		poISAbsCommand = new GISSegmentDisplayCountMsgCommand();
		poISAbsCommand->SetData( new GISSegmentDisplayCountMsgData() );		
	}		
	else if( GISSetChromakeyColorMsgData::m_CCommandId == strCmd )
	{
		poISAbsCommand = new GISSetChromakeyColorMsgCommand();
		poISAbsCommand->SetData( new GISSetChromakeyColorMsgData() );		
	}		
	else if( GISSetGraphicsTypeMsgData::m_CCommandId == strCmd )
	{
		poISAbsCommand = new GISSetGraphicsTypeMsgCommand();
		poISAbsCommand->SetData( new GISSetGraphicsTypeMsgData() );		
	}		
	else if( GISStatusMsgData::m_CCommandId == strCmd )
	{
		poISAbsCommand = new GISStatusMsgCommand();
		poISAbsCommand->SetData( new GISStatusMsgData() );		
	}		
	else if( GISVideoFrameSeekMsgData::m_CCommandId == strCmd )
	{
		poISAbsCommand = new GISVideoFrameSeekMsgCommand();
		poISAbsCommand->SetData( new GISVideoFrameSeekMsgData() );		
	}		
	else if( GISVideoPlayMsgData::m_CCommandId == strCmd )
	{
		poISAbsCommand = new GISVideoPlayMsgCommand();
		poISAbsCommand->SetData( new GISVideoPlayMsgData() );		
	}		
	else if( GISVideoRangePlayMsgData::m_CCommandId == strCmd )
	{
		poISAbsCommand = new GISVideoRangePlayMsgCommand();
		poISAbsCommand->SetData( new GISVideoRangePlayMsgData() );		
	}		
	else if( GISVideoStopMsgData::m_CCommandId == strCmd )
	{
		poISAbsCommand = new GISVideoStopMsgCommand();
		poISAbsCommand->SetData( new GISVideoStopMsgData() );		
	}		
	else if( GISVideoViewMsgData::m_CCommandId == strCmd )
	{
		poISAbsCommand = new GISVideoViewMsgCommand();
		poISAbsCommand->SetData( new GISVideoViewMsgData() );		
	}		
	else
	{
		throw GIS_EXCEPTION(GISCommandStatusEnum(GISCommandStatusEnum::eCommandError));
	}

	Populate(poISAbsCommand, oInText);
	
	return poISAbsCommand;
}

void GISMsgCmdDataFactory::Populate( GISAbsCommand* poISAbsCommand, QByteArray & oInBuffer ) throw(GException)
{
	LOG_LEVEL4 ("Populate");

	bool bOk(true);

	GetField(oInBuffer, eCMDFieldNumber).toInt(&bOk);
	if( !bOk )
	{
		throw GIS_EXCEPTION(GISCommandStatusEnum(GISCommandStatusEnum::eCommandError));
	}

	int iDisplay = GetField(oInBuffer, eDISPFieldNumber).toInt(&bOk);
	if( !bOk )
	{
		throw GIS_EXCEPTION(GISCommandStatusEnum(GISCommandStatusEnum::eInvalidDisplayNumberOrDisplayNumberDoesNotExist));
	}
	
	int iScreen = GetField(oInBuffer, eSCRNFieldNumber).toInt(&bOk);
	if( !bOk )
	{
		throw GIS_EXCEPTION(GISCommandStatusEnum(GISCommandStatusEnum::eInvalidScreenNumberOrScreenNumberDoesNotExist));
	}
	
	int iSegment = GetField(oInBuffer, eSEGFieldNumber).toInt(&bOk);
	if( !bOk )
	{
		throw GIS_EXCEPTION(GISCommandStatusEnum(GISCommandStatusEnum::eInvalidSegmentNumberOrSegmentNumberDoesNotExist));
	}
	
	int iField = GetField(oInBuffer, eFLDFieldNumber).toInt(&bOk);
	if( !bOk )
	{
		throw GIS_EXCEPTION(GISCommandStatusEnum(GISCommandStatusEnum::eInvalidFieldNumberOrFieldNumberDoesNotExist));
	}

        int iAttrib = GetField(oInBuffer, eATTRFieldNumber).toInt(&bOk);
	if( !bOk )
	{
		throw GIS_EXCEPTION(GISCommandStatusEnum(GISCommandStatusEnum::eInvalidAttribute));
	}
	
	int iXPos = GetField(oInBuffer, eXPOSFieldNumber).toInt(&bOk);
	if( !bOk )
	{
		if (iAttrib==GAttributeEnum::eCenterText || iAttrib==GAttributeEnum::eLeftText || iAttrib==GAttributeEnum::eRightText
				|| iAttrib==GAttributeEnum::eCenterWithUnderscore || iAttrib==GAttributeEnum::eLeftWithUnderscore || iAttrib==GAttributeEnum::eRightWithUnderscore)
		{
			// Comment from Old Java DM: Some applications send bogus data ("***") for X pos of a centered Flytext command; convert to 0.
			iXPos = 0;
		}
		else
		{
			throw GIS_EXCEPTION(GISCommandStatusEnum(GISCommandStatusEnum::eInvalidXPositionOrXRangeExceeded));
		}
	}
	
	int iYPos = GetField(oInBuffer, eYPOSFieldNumber).toInt(&bOk);
	if( !bOk )
	{
		throw GIS_EXCEPTION(GISCommandStatusEnum(GISCommandStatusEnum::eInvalidYPositionOrYRangeExceeded));
	}	
	
	int iFont = GetField(oInBuffer, eFONTFieldNumber).toInt(&bOk);
	if( !bOk )
	{
		throw GIS_EXCEPTION(GISCommandStatusEnum(GISCommandStatusEnum::eFontNumberUndefined));
	}
	
	int iColor = GetField(oInBuffer, eCOLORFieldNumber).toInt(&bOk);
	if( !bOk )
	{
		throw GIS_EXCEPTION(GISCommandStatusEnum(GISCommandStatusEnum::eInvalidColorOrColorRegister));
	}

	// This field is optional for some commands.
	QByteArray strVLD("");
	try
	{
		strVLD = GetField(oInBuffer, eVLDFieldNumber);
	}
	catch(GISException& e)
	{
		// Ignoring not found exception for this field because this field is optional for some commands.
	}

	GISAbsData* pGISAbsData = poISAbsCommand->GetData();

	pGISAbsData->SetDisplay(iDisplay);
	pGISAbsData->SetAttr(iAttrib);
	pGISAbsData->SetColor(iColor);
	pGISAbsData->SetField(iField);
	pGISAbsData->SetFont(iFont);
	pGISAbsData->SetScreen(iScreen);
	pGISAbsData->SetSegment(iSegment);
	pGISAbsData->SetXpos(iXPos);
	pGISAbsData->SetYpos(iYPos);
	pGISAbsData->SetVLD(strVLD);
}

QByteArray GISMsgCmdDataFactory::GetField( QByteArray & oInBuffer, eFieldNumber eFieldNumberRequired ) throw(GException)
{
	LOG_LEVEL4("GetField");

	int iIndex = 0;
	int iCount = 1;
	int iRequiredField = static_cast<int>(eFieldNumberRequired);
	int iVariableField = static_cast<int>(eVLDFieldNumber);
	int iLastField 	   = static_cast<int>(eLastFieldNumber);
	QByteArray strFieldRequired;
	
	while( (iCount < iRequiredField) && (iCount < iLastField) )
	{
		iIndex = oInBuffer.indexOf('\0', iIndex) + 1;
		iCount++;
	}
	
	if( iCount == iRequiredField )
	{
		int iLen = 0;

		if( iCount != iVariableField )
		{
			iLen = (oInBuffer.indexOf('\0', iIndex) - iIndex);
		}
		else
		{
			iLen = (oInBuffer.length()-iIndex);
		}

		strFieldRequired = oInBuffer.mid(iIndex, iLen);
	}
	else
	{
		throw GIS_EXCEPTION_DETAILS(GISCommandStatusEnum(GISCommandStatusEnum::eCommandError), "Field Not Found");
	}

	return strFieldRequired;
}
