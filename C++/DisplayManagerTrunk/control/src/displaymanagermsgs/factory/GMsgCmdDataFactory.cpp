#include <QString>

#include "GMsgCmdDataFactory.h"

#include "control/src/displaymanagermsgs/base/GAbsMsgCommand.h"
#include "utils/base/src/GException.h"

#include "control/src/displaymanagermsgs/xml/GRequestClassifier.h"
#include "control/src/displaymanagermsgs/xml/GClearScreenMsgDataXML.h"
#include "control/src/displaymanagermsgs/xml/GDownloadGraphicMsgDataXML.h"
#include "control/src/displaymanagermsgs/xml/GFlyTextMsgDataXML.h"
#include "control/src/displaymanagermsgs/xml/GGdiClearScreenMsgDataXML.h"
#include "control/src/displaymanagermsgs/xml/GGdiDrawLineMsgDataXML.h"
#include "control/src/displaymanagermsgs/xml/GGdiDrawTextMsgDataXML.h"
#include "control/src/displaymanagermsgs/xml/GGdiFillRectMsgDataXML.h"
#include "control/src/displaymanagermsgs/xml/GGdiGetScreenSizeMsgDataXML.h"
#include "control/src/displaymanagermsgs/xml/GGdiRefreshScreenMsgDataXML.h"
#include "control/src/displaymanagermsgs/xml/GInfoScreenMsgDataXML.h"
#include "control/src/displaymanagermsgs/xml/GKeyEventMsgDataXML.h"
#include "control/src/displaymanagermsgs/xml/GKioskIdMsgDataXML.h"
#include "control/src/displaymanagermsgs/xml/GResponseMsgDataXML.h"
#include "control/src/displaymanagermsgs/xml/GGetPersonalitiesMsgDataXML.h"
#include "control/src/displaymanagermsgs/xml/GSetPersonalityMsgDataXML.h"
#include "control/src/displaymanagermsgs/xml/GSetDefaultPersonalityMsgDataXML.h"
#include "control/src/displaymanagermsgs/xml/GURLMsgDataXML.h"
#include "control/src/displaymanagermsgs/xml/GClockMsgDataXML.h"
#include "control/src/displaymanagermsgs/xml/GSetMediaManagerMsgDataXML.h"
#include "control/src/displaymanagermsgs/xml/GGetMediaManagersMsgDataXML.h"
#include "control/src/displaymanagermsgs/xml/GSetForkFailureOptionMsgDataXML.h"
#include "control/src/displaymanagermsgs/xml/GGetForkFailureOptionsMsgDataXML.h"

#include "control/src/displaymanagermsgs/command/GClearScreenMsgCommand.h"
#include "control/src/displaymanagermsgs/command/GDownloadGraphicMsgCommand.h"
#include "control/src/displaymanagermsgs/command/GFlyTextMsgCommand.h"
#include "control/src/displaymanagermsgs/command/GGdiClearScreenMsgCommand.h"
#include "control/src/displaymanagermsgs/command/GGdiDrawLineMsgCommand.h"
#include "control/src/displaymanagermsgs/command/GGdiDrawTextMsgCommand.h"
#include "control/src/displaymanagermsgs/command/GGdiFillRectMsgCommand.h"
#include "control/src/displaymanagermsgs/command/GGdiGetScreenSizeMsgCommand.h"
#include "control/src/displaymanagermsgs/command/GGdiRefreshScreenMsgCommand.h"
#include "control/src/displaymanagermsgs/command/GInfoScreenMsgCommand.h"
#include "control/src/displaymanagermsgs/command/GKeyEventMsgCommand.h"
#include "control/src/displaymanagermsgs/command/GKioskIdMsgCommand.h"
#include "control/src/displaymanagermsgs/command/GResponseMsgCommand.h"
#include "control/src/displaymanagermsgs/command/GGetPersonalitiesMsgCommand.h"
#include "control/src/displaymanagermsgs/command/GSetPersonalityMsgCommand.h"
#include "control/src/displaymanagermsgs/command/GSetDefaultPersonalityMsgCommand.h"
#include "control/src/displaymanagermsgs/command/GURLMsgCommand.h"
#include "control/src/displaymanagermsgs/command/GClockMsgCommand.h"
#include "control/src/displaymanagermsgs/command/GSetMediaManagerMsgCommand.h"
#include "control/src/displaymanagermsgs/command/GGetMediaManagersMsgCommand.h"
#include "control/src/displaymanagermsgs/command/GSetForkFailureOptionMsgCommand.h"
#include "control/src/displaymanagermsgs/command/GGetForkFailureOptionsMsgCommand.h"

#include "control/src/displaymanagermsgs/data/GClockMsgData.h"

const QString GMsgCmdDataFactory::CInterfaceVersion("01.0.01");
QMutex GMsgCmdDataFactory::m_oMutex;
GMsgCmdDataFactory* GMsgCmdDataFactory::m_poCmdDataFactory;


GMsgCmdDataFactory::GMsgCmdDataFactory()
{
}

GMsgCmdDataFactory::~GMsgCmdDataFactory()
{
}

GMsgCmdDataFactory* GMsgCmdDataFactory::GetInstance()
{
	m_oMutex.lock();
	
	if(m_poCmdDataFactory == NULL)
	{
		m_poCmdDataFactory = new GMsgCmdDataFactory();
	}
	
	m_oMutex.unlock();
	
	return m_poCmdDataFactory;
	
}

GAbsMsgCommand* GMsgCmdDataFactory::Create(QByteArray & oInBuffer) throw(GException)
{
	GAbsMsgCommand* poCommand = NULL;

	switch(	GRequestClassifier::GetInstance()->GetRequestType( oInBuffer ) )
	{	
		case GRequestClassifier::eClearScreenRequest:
		{
			poCommand = new GClearScreenMsgCommand( GClearScreenMsgDataXML::UnMarshall( oInBuffer ) );
			break;
		}
		case GRequestClassifier::eDownloadGraphicRequest:
		{
			poCommand = new GDownloadGraphicMsgCommand( GDownloadGraphicMsgDataXML::UnMarshall( oInBuffer ) );
			break;
		}
		case GRequestClassifier::eFlyTextRequest:
		{
			poCommand = new GFlyTextMsgCommand( GFlyTextMsgDataXML::UnMarshall( oInBuffer ) );
			break;
		}
		case GRequestClassifier::eGdiClearScreenRequest:
		{
			poCommand = new GGdiClearScreenMsgCommand( GGdiClearScreenMsgDataXML::UnMarshall( oInBuffer ) );
			break;
		}
		case GRequestClassifier::eGdiDrawLineRequest:
		{
			poCommand = new GGdiDrawLineMsgCommand( GGdiDrawLineMsgDataXML::UnMarshall( oInBuffer ) );
			break;
		}
		case GRequestClassifier::eGdiDrawTextRequest:
		{
			poCommand = new GGdiDrawTextMsgCommand( GGdiDrawTextMsgDataXML::UnMarshall( oInBuffer ) );
			break;
		}
		case GRequestClassifier::eGdiFillRectRequest:
		{
			poCommand = new GGdiFillRectMsgCommand( GGdiFillRectMsgDataXML::UnMarshall( oInBuffer ) );
			break;
		}
		case GRequestClassifier::eGdiGetScreenSizeRequest:
		{
			poCommand = new GGdiGetScreenSizeMsgCommand( GGdiGetScreenSizeMsgDataXML::UnMarshall( oInBuffer ) );
			break;
		}
		case GRequestClassifier::eGdiRefreshScreenRequest:
		{
			poCommand = new GGdiRefreshScreenMsgCommand( GGdiRefreshScreenMsgDataXML::UnMarshall( oInBuffer ) );
			break;
		}
		case GRequestClassifier::eInfoScreenRequest:
		{
			poCommand = new GInfoScreenMsgCommand( GInfoScreenMsgDataXML::UnMarshall( oInBuffer ) );
			break;
		}		
		case GRequestClassifier::eKeyEventRequest:
		{
			poCommand = new GKeyEventMsgCommand( GKeyEventMsgDataXML::UnMarshall( oInBuffer ) );
			break;
		}
		case GRequestClassifier::eKioskIdRequest:
		{
			poCommand = new GKioskIdMsgCommand( GKioskIdMsgDataXML::UnMarshall( oInBuffer ) );
			break;
		}
		case GRequestClassifier::eResponse:
		{
			poCommand = new GResponseMsgCommand( GResponseMsgDataXML::UnMarshall( oInBuffer ) );
			break;
		}
		case GRequestClassifier::eGetPersonalitiesRequest:
		{
			poCommand = new GGetPersonalitiesMsgCommand( GGetPersonalitiesMsgDataXML::UnMarshall( oInBuffer ) );
			break;
		}
		case GRequestClassifier::eSetPersonalityRequest:
		{
			poCommand = new GSetPersonalityMsgCommand( GSetPersonalityMsgDataXML::UnMarshall( oInBuffer ) );
			break;
		}
		case GRequestClassifier::eSetDefaultPersonalityRequest:
		{
			poCommand = new GSetDefaultPersonalityMsgCommand( GSetDefaultPersonalityMsgDataXML::UnMarshall( oInBuffer ) );
			break;
		}
		case GRequestClassifier::eURLRequest:
		{
			poCommand = new GURLMsgCommand( GURLMsgDataXML::UnMarshall( oInBuffer ) );
			break;
		}
		case GRequestClassifier::eClockRequest:
		{
			GClockMsgData *pClockMsgData = GClockMsgDataXML::UnMarshall(oInBuffer);
			poCommand = new GClockMsgCommand(pClockMsgData);
			break;
		}
		case GRequestClassifier::eSetMediaManagerRequest:
		{
			poCommand = new GSetMediaManagerMsgCommand( GSetMediaManagerMsgDataXML::UnMarshall( oInBuffer ) );
			break;
		}
		case GRequestClassifier::eGetMediaManagersRequest:
		{
			poCommand = new GGetMediaManagersMsgCommand( GGetMediaManagersMsgDataXML::UnMarshall( oInBuffer ) );
			break;
		}
		case GRequestClassifier::eSetForkFailureOptionRequest:
		{
			poCommand = new GSetForkFailureOptionMsgCommand( GSetForkFailureOptionMsgDataXML::UnMarshall( oInBuffer ) );
			break;
		}
		case GRequestClassifier::eGetForkFailureOptionsRequest:
		{
			poCommand = new GGetForkFailureOptionsMsgCommand( GGetForkFailureOptionsMsgDataXML::UnMarshall( oInBuffer ) );
			break;
		}
		case GRequestClassifier::eNotFound:
		{
			throw GEXCEPTION("Message Parsing error");
			break;
		}			
	}
	
	return poCommand;	
}
