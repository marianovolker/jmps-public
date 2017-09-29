#include "GRequestClassifier.h"

#include <QStringList>
#include <QMutexLocker>

#include "GClearScreenMsgDataXML.h"
#include "GDownloadGraphicMsgDataXML.h"
#include "GFlyTextMsgDataXML.h"
#include "GGdiClearScreenMsgDataXML.h"
#include "GGdiDrawLineMsgDataXML.h"
#include "GGdiDrawTextMsgDataXML.h"
#include "GGdiFillRectMsgDataXML.h"
#include "GGdiGetScreenSizeMsgDataXML.h"
#include "GGdiRefreshScreenMsgDataXML.h"
#include "GInfoScreenMsgDataXML.h"
#include "GKeyEventMsgDataXML.h"
#include "GKioskIdMsgDataXML.h"
#include "GResponseMsgDataXML.h"
#include "GGetPersonalitiesMsgDataXML.h"
#include "GSetPersonalityMsgDataXML.h"
#include "GSetDefaultPersonalityMsgDataXML.h"
#include "GURLMsgDataXML.h"
#include "GClockMsgDataXML.h"
#include "GSetMediaManagerMsgDataXML.h"
#include "GGetMediaManagersMsgDataXML.h"
#include "GSetForkFailureOptionMsgDataXML.h"
#include "GGetForkFailureOptionsMsgDataXML.h"

#include "utils/logger/src/GLogger.h"

QMutex GRequestClassifier::m_oMutex;
GRequestClassifier* GRequestClassifier::m_poClassifier(NULL);


GRequestClassifier* GRequestClassifier::GetInstance()
{
	LOG_LEVEL4("GetInstance()");

	{
		QMutexLocker oLock(&m_oMutex);

		if(m_poClassifier == NULL)
		{
			m_poClassifier = new GRequestClassifier();
		}
	}
	return m_poClassifier;
}

GRequestClassifier::eRequestType GRequestClassifier::GetRequestType(QByteArray & oXML)
{	
	LOG_LEVEL4("GetRequestType()");

	eRequestType eReturn(eNotFound);
	QString strXML(oXML);
	
	//! Note
	/*! At this point we just handle tag name matching.
	 *  Proper XML validation will be held in upper layers.
	 *  */
	
	if( m_oReg.indexIn(strXML) != -1)
	{
		enum { eCmdRootTag = 1 };
		QString strTag = m_oReg.cap(static_cast<int>(eCmdRootTag));

		GRequestClassifierMap::const_iterator i = m_oKeys.find(strTag);

		if(i != m_oKeys.end() )
		{
			eReturn = i.value();
		}
	}
	return eReturn;
}

GRequestClassifier::GRequestClassifier()
  : m_oKeys(),
    m_oReg()
{
	LOG_LEVEL4("GRequestClassifier()");

	m_oKeys.insert(GClearScreenMsgDataXML::CXMLTag				, 	eClearScreenRequest);
	m_oKeys.insert(GDownloadGraphicMsgDataXML::CXMLTag			, 	eDownloadGraphicRequest);
	m_oKeys.insert(GFlyTextMsgDataXML::CXMLTag					, 	eFlyTextRequest);
	m_oKeys.insert(GGdiClearScreenMsgDataXML::CXMLTag			, 	eGdiClearScreenRequest);
	m_oKeys.insert(GGdiDrawLineMsgDataXML::CXMLTag				, 	eGdiDrawLineRequest);
	m_oKeys.insert(GGdiDrawTextMsgDataXML::CXMLTag				, 	eGdiDrawTextRequest);
	m_oKeys.insert(GGdiFillRectMsgDataXML::CXMLTag				, 	eGdiFillRectRequest);
	m_oKeys.insert(GGdiGetScreenSizeMsgDataXML::CXMLTag			, 	eGdiGetScreenSizeRequest);
	m_oKeys.insert(GGdiRefreshScreenMsgDataXML::CXMLTag			, 	eGdiRefreshScreenRequest);
	m_oKeys.insert(GInfoScreenMsgDataXML::CXMLTag				, 	eInfoScreenRequest);
	m_oKeys.insert(GKeyEventMsgDataXML::CXMLTag					, 	eKeyEventRequest);
	m_oKeys.insert(GKioskIdMsgDataXML::CXMLTag					, 	eKioskIdRequest);
	m_oKeys.insert(GResponseMsgDataXML::CXMLTag					,	eResponse);
	m_oKeys.insert(GGetPersonalitiesMsgDataXML::CXMLTag			, 	eGetPersonalitiesRequest);
	m_oKeys.insert(GSetPersonalityMsgDataXML::CXMLTag			, 	eSetPersonalityRequest);
	m_oKeys.insert(GSetDefaultPersonalityMsgDataXML::CXMLTag	, 	eSetDefaultPersonalityRequest);
	m_oKeys.insert(GURLMsgDataXML::CXMLTag						, 	eURLRequest);
	m_oKeys.insert(GClockMsgDataXML::CXMLTag					,	eClockRequest);
	m_oKeys.insert(GSetMediaManagerMsgDataXML::CXMLTag			,	eSetMediaManagerRequest);
	m_oKeys.insert(GGetMediaManagersMsgDataXML::CXMLTag			,	eGetMediaManagersRequest);
	m_oKeys.insert(GSetForkFailureOptionMsgDataXML::CXMLTag		,	eSetForkFailureOptionRequest);
	m_oKeys.insert(GGetForkFailureOptionsMsgDataXML::CXMLTag	,	eGetForkFailureOptionsRequest);
	
	m_oReg = QRegExp("<(?:[a-zA-Z_][\\w\\-]*:)?(" \
                                                "clear_screen_request|" \
                                                "gdi_clearscreen_request|" \
                                                "gdi_drawline_request|" \
                                                "gdi_drawtext_request|" \
                                                "gdi_fillrect_request|" \
                                                "gdi_getscreensize_request|" \
                                                "gdi_refreshscreen_request|" \
                                                "download_graphic_request|" \
                                                "flytext_request|" \
                                                "infoscreen_request|" \
                                                "key_event|" \
                                                "kiosk_id_request|" \
                                                "response|" \
                                                "get_personalities_request|" \
                                                "set_personality_request|" \
                                                "set_default_personality_request|" \
                                                "url_request|" \
												"clock_request|" \
												"set_media_manager_type_request|" \
												"get_media_managers_request|" \
												"set_fork_failure_option_request|" \
												"get_fork_failure_options_request" \
                     ")(?:[^\\w\\-].*)?>",
					 Qt::CaseSensitive,QRegExp::RegExp2);
	m_oReg.setMinimal(true);
}

GRequestClassifier::~GRequestClassifier()
{
	LOG_LEVEL4("~GRequestClassifier()");
}
