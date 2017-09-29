#include "GPersonalityView.h"

#include <QStringList>
#include <QMutexLocker>

#include "view/src/utils/requestsmanager/GPageLoadRequestManager.h"
#include "view/src/utils/requests/GSetPreloadedPagesRequest.h"
#include "config/GDisplayManagerConfig.h"
#include "utils/logger/src/GLogger.h"

QSharedPointer<GPersonalityView> GPersonalityView::m_pGPersonalityView;
QMutex GPersonalityView::m_mutex;

QSharedPointer<GPersonalityView> GPersonalityView::GetInstance (void) throw(GException)
{
	LOG_LEVEL4("GetInstance()");

	QMutexLocker oLocker(&GPersonalityView::m_mutex);

	if( m_pGPersonalityView.isNull() )
	{
		m_pGPersonalityView = QSharedPointer<GPersonalityView>(new GPersonalityView());

		if (!m_pGPersonalityView.isNull())
		{
			m_pGPersonalityView->Init();
		}
	}

	if( m_pGPersonalityView.isNull() )
	{
		 throw(GException("GPersonalityView::GetInstance() made a NULL pointer."));
	}

	return m_pGPersonalityView;
}

GPersonalityView::GPersonalityView (void)
	:	m_mutex_instance(QMutex::Recursive) // Have to be in this way to let some methods access other methods without locking.
{
	LOG_LEVEL4("GPersonalityView()");
}

GPersonalityView::~GPersonalityView (void)
{
	LOG_LEVEL4("~GPersonalityView()");
}

void GPersonalityView::Init (void)
{
	LOG_LEVEL4("Init()");

	QSharedPointer<GPageLoadRequestManager> pGPageLoadRequestManager(GPageLoadRequestManager::GetInstance());

	if( !connect( this, SIGNAL(SignalSendRequest(GSetPreloadedPagesRequest*)),
			pGPageLoadRequestManager.data(), SLOT(ReceiveRequest(GSetPreloadedPagesRequest*)), Qt::QueuedConnection ) )
	{
		LOG_LEVEL1("Can't connect to GPageLoadRequestManager object.");
	}

	bool bIsView(false);
	m_uiSessionId = pGPageLoadRequestManager->Subscribe("PersonalityView", bIsView);
}

void GPersonalityView::PresetView (bool bSynchronize)
{
	LOG_LEVEL4("PresetView()");

	QStringList oStrListJSCalls;
	QStringList oStrListJSLibraries;
	QString strPersonalityURL("");

	ObtainDefaultScreenURL( strPersonalityURL );

	oStrListJSLibraries << GETCONF_STR(eDisplayManagerJQueryLibraryFileName);

	oStrListJSLibraries << GETCONF_STR(eDisplayManagerJSLibraryFileName);

	GSetPreloadedPagesRequest* pRequest(new GSetPreloadedPagesRequest(m_uiSessionId, bSynchronize, oStrListJSCalls, oStrListJSLibraries, strPersonalityURL));

	emit SignalSendRequest(pRequest);
}

void GPersonalityView::RefreshPersonality (bool bSynchronize)
{
	LOG_LEVEL4("RefreshPersonality()");

	QMutexLocker oLocker(&m_mutex_instance);

	QSharedPointer<GPageLoadRequestManager> pGPageLoadRequestManager(GPageLoadRequestManager::GetInstance());

	m_oPersonality.Init(pGPageLoadRequestManager->GetViewResolution(), GETCONF_STR(eDisplayManagerPersonalityName));
	m_qvInfoscreenLanguageMapping = m_oPersonality.GetPersonalityLanguageMapping();
	for( unsigned int uiI = eClockPhaseMin; (uiI >= eClockPhaseMin) && (uiI <= eClockPhaseMax); ++uiI )
	{
		m_qzClockDimension[uiI] = QSize(m_oPersonality.GetClockGraphicPropertyDimension(GPersonality::eWidth,uiI),m_oPersonality.GetClockGraphicPropertyDimension(GPersonality::eHeight,uiI));
	}
	for( unsigned int uiI = eLeftArrowDirection; (uiI > eLeftArrowDirection) && (uiI <= eRightArrowDirection); ++uiI )
	{
		m_qzArrowDimension[uiI] = QSize(m_oPersonality.GetArrowGraphicPropertyDimension(GPersonality::eWidth,static_cast<GPersonality::eArrowDirection>(uiI)),m_oPersonality.GetArrowGraphicPropertyDimension(GPersonality::eHeight,static_cast<GPersonality::eArrowDirection>(uiI)));
	}

	PresetView(bSynchronize);
}

bool GPersonalityView::MayRemoveOverlaps (void)
{
	LOG_LEVEL4("MayRemoveOverlaps()");

	QMutexLocker oLocker(&m_mutex_instance);

	return m_oPersonality.IsRemoveOverlaps();
}

unsigned int GPersonalityView::ObtainOverlapsErrorMarginPixels (void)
{
	LOG_LEVEL4("ObtainOverlapsErrorMarginPixels()");

	QMutexLocker oLocker(&m_mutex_instance);

	int iResult = m_oPersonality.GetOverlapsErrorMarginPixels();
	return ((iResult>0)?(static_cast<unsigned int>(iResult)):(0));
}

bool GPersonalityView::ObtainImageFromPersonality ( QString &strImagePath, int iImageId )
{
    LOG_LEVEL4("ObtainImageFromPersonality()");

    QMutexLocker oLocker(&m_mutex_instance);

    if( ( strImagePath = ( m_oPersonality.GetCustomGraphicProperty( iImageId ) ) ) == "" )
    {
        LOG_LEVEL4(QString("Could not obtain image path for segment %1.").arg(iImageId));
    	//No Image for this segment in Personality
        return false;
    }

    if( !m_oPersonality.GetAValidPath(strImagePath) )
    {
		LOG_LEVEL4(QString("Image path '%1' does not exist for segment %2.").arg(strImagePath).arg(iImageId));
		// Image doesn't exist
		strImagePath = "";
		return false;
    }

    return true;
}

bool GPersonalityView::ObtainClockCharacterImageFromPersonality ( QString &strImagePath, int iClockPhase )
{
    LOG_LEVEL4("ObtainClockCharacterImageFromPersonality()");

    QMutexLocker oLocker(&m_mutex_instance);

    if( ( strImagePath = ( m_oPersonality.GetClockGraphicProperty( iClockPhase ) ) ) == "" )
    {
        LOG_LEVEL4(QString("Could not obtain clock image path for phase %1.").arg(iClockPhase));
    	//No Image for this clock phase in Personality
        return false;
    }

    if( !m_oPersonality.GetAValidPath(strImagePath) )
    {
		LOG_LEVEL4(QString("Clock image path '%1' does not exist for phase %2.").arg(strImagePath).arg(iClockPhase));
		// Image doesn't exist
		strImagePath = "";
		return false;
    }

    return true;
}

bool GPersonalityView::ObtainArrowCharacterImageFromPersonality ( QString &strImagePath, int iArrowDirection )
{
    LOG_LEVEL4("ObtainArrowCharacterImageFromPersonality()");

    QMutexLocker oLocker(&m_mutex_instance);

	enum {	eInfoScreenLeftArrowDirection = 0,
			eInfoScreenRightArrowDirection = 1 };

    if ( (strImagePath = (m_oPersonality.GetArrowGraphicProperty((iArrowDirection==eInfoScreenLeftArrowDirection)?(GPersonality::eLeftArrow):(GPersonality::eRightArrow)))) == "" )
    {
        LOG_LEVEL4(QString("Could not obtain arrow image path for direction %1.").arg(iArrowDirection));
    	//No Image for this clock phase in Personality
        return false;
    }

    if( !m_oPersonality.GetAValidPath(strImagePath) )
    {
		LOG_LEVEL4(QString("Arrow image path '%1' does not exist for direction %2.").arg(strImagePath).arg(iArrowDirection));
		// Image doesn't exist
		strImagePath = "";
		return false;
    }

    return true;
}

bool GPersonalityView::ObtainDefaultScreenURL(QString & strScreenURL)
{
	LOG_LEVEL4("ObtainDefaultScreenURL()");

	QMutexLocker oLocker(&m_mutex_instance);

	strScreenURL = m_oPersonality.GetStrClearScreen();

    if( !m_oPersonality.GetAValidPath(strScreenURL) )
    {
		LOG_LEVEL4(QString("Screen personality default URL path '%1' does not exist.").arg(strScreenURL));
		// Image doesn't exist
		strScreenURL = "";
		return false;
    }

	return !strScreenURL.isEmpty();
}

bool GPersonalityView::ObtainCustomScreenURL(QString & strScreenURL, int iScreenNumber)
{
	LOG_LEVEL4("ObtainCustomScreenURL()");

	QMutexLocker oLocker(&m_mutex_instance);

	strScreenURL.clear();
	if( iScreenNumber > 0 )
	{
		strScreenURL = m_oPersonality.GetCustomScreenProperty(iScreenNumber);

	    if( !m_oPersonality.GetAValidPath(strScreenURL) )
	    {
			LOG_LEVEL4(QString("Screen personality URL path '%1' does not exist for screen %2.").arg(strScreenURL).arg(iScreenNumber));
			// Image doesn't exist
			strScreenURL = "";
			return false;
	    }
	}

	return !strScreenURL.isEmpty();
}

bool GPersonalityView::ObtainScreenURL(QString & strScreenURL, int iScreenNumber)
{
	LOG_LEVEL4("ObtainScreenURL()");

	QMutexLocker oLocker(&m_mutex_instance);

	bool bResult(false);

	if( iScreenNumber > 0 )
	{
		bResult = ObtainCustomScreenURL(strScreenURL,iScreenNumber);
	}

	if (!bResult)
	{
		bResult = ObtainDefaultScreenURL(strScreenURL);
	}
	return bResult;
}

QString GPersonalityView::ConvertExtendedASCIIToUTF8 (QString const & sInfoScreenText)
{
	LOG_LEVEL4("ConvertExtendedASCIIToUTF8()");

	QMutexLocker oLocker(&m_mutex_instance);

	// Input characters are assumed to be in the character encoding
	// defined in the InfoScreen Message Level Protocol spec (GMSCGP-FS-S10.0-P)
	// - replace extended characters (i.e. > 127) with Unicode escape sequences

	enum {	eInfoScreenLowerBoundClockPhaseCharacters = 24,
			eInfoScreenHigherBoundClockPhaseCharacters = 31,
			eInfoScreenRightArrowCharacter = 91,
			eInfoScreenLeftArrowCharacter = 93 };

	enum {	eInfoScreenLeftArrowDirection = 0,
			eInfoScreenRightArrowDirection = 1 };

	QString sUTF8Text; // QString stores internal data as Unicode, more specifically as UTF8;
	{
		static QString const sHTMLEntityEncoderHeader("'+ $('<div style=\"white-space:nowrap;\"/>').text('");
		static QString const sHTMLEntityEncoderFooter("').html() + '");
		static QString const sCharacterImage("<img _src=\"%1\"%2/>");
		static QString const sCharacterImageStyle(" style=\"%1\"");
		static QString const sCharacterImageStyleSeparator("; ");
		static QString const sCharacterImageStyleWidth("width:%1");
		static QString const sCharacterImageStyleHeight("height:%1");
		static QChar const qcNull = QChar(QChar::Null);

		bool bInHTMLEntityEncoderScope = true;
		sUTF8Text += sHTMLEntityEncoderHeader;
		QString::const_iterator kiter = sInfoScreenText.constBegin();
		for (; kiter != sInfoScreenText.constEnd(); ++kiter)
		{
			unsigned int uiCharacterTableIndex = static_cast<unsigned char>((*kiter).toAscii()); // The cast is needed in order to preserve the relationship with the array index;
			unsigned int uiInfoScreenChar = static_cast<unsigned int>(m_qvInfoscreenLanguageMapping.value(uiCharacterTableIndex,qcNull).unicode());
			if ((uiInfoScreenChar >= eInfoScreenLowerBoundClockPhaseCharacters) && (uiInfoScreenChar <= eInfoScreenHigherBoundClockPhaseCharacters))
			{
				QString strImagePath("");
				unsigned int uiClockPhase(static_cast<unsigned int>((uiInfoScreenChar-eInfoScreenLowerBoundClockPhaseCharacters)+eClockPhaseMin));
				if ( ObtainClockCharacterImageFromPersonality(strImagePath, uiClockPhase) )
				{
					if (bInHTMLEntityEncoderScope == true)
					{
						sUTF8Text += sHTMLEntityEncoderFooter;
						bInHTMLEntityEncoderScope = false;
					}
					{
						QString sImageStyle("");
						int iWidth(m_qzClockDimension[uiClockPhase].width());
						int iHeight(m_qzClockDimension[uiClockPhase].height());
						if ( (iWidth > 0) && (iHeight > 0) )
						{
							sImageStyle = QString(sCharacterImageStyle).arg(QString(sCharacterImageStyleWidth).arg(iWidth).append("px").append(sCharacterImageStyleSeparator)+QString(sCharacterImageStyleHeight).arg(iHeight).append("px"));
						}
						else if ( iWidth > 0 )
						{
							sImageStyle = QString(sCharacterImageStyle).arg(QString(sCharacterImageStyleWidth).arg(iWidth).append("px"));
						}
						else if ( iHeight > 0 )
						{
							sImageStyle = QString(sCharacterImageStyle).arg(QString(sCharacterImageStyleHeight).arg(iHeight).append("px"));
						}
						sUTF8Text += QString(sCharacterImage).arg(strImagePath).arg(sImageStyle);
					}
				}
			}
			else if ((uiInfoScreenChar == eInfoScreenLeftArrowCharacter)||(uiInfoScreenChar == eInfoScreenRightArrowCharacter))
			{
				QString strImagePath("");
				unsigned int uiArrowDirection((uiInfoScreenChar==eInfoScreenLeftArrowCharacter)?(eLeftArrowDirection):(eRightArrowDirection));
				if ( ObtainArrowCharacterImageFromPersonality(strImagePath,uiArrowDirection) )
				{
					if (bInHTMLEntityEncoderScope == true)
					{
						sUTF8Text += sHTMLEntityEncoderFooter;
						bInHTMLEntityEncoderScope = false;
					}
					{
						QString sImageStyle("");
						int iWidth(m_qzArrowDimension[uiArrowDirection].width());
						int iHeight(m_qzArrowDimension[uiArrowDirection].height());
						if ( (iWidth > 0) && (iHeight > 0) )
						{
							sImageStyle = QString(sCharacterImageStyle).arg(QString(sCharacterImageStyleWidth).arg(iWidth).append("px").append(sCharacterImageStyleSeparator)+QString(sCharacterImageStyleHeight).arg(iHeight).append("px"));
						}
						else if ( iWidth > 0 )
						{
							sImageStyle = QString(sCharacterImageStyle).arg(QString(sCharacterImageStyleWidth).arg(iWidth).append("px"));
						}
						else if ( iHeight > 0 )
						{
							sImageStyle = QString(sCharacterImageStyle).arg(QString(sCharacterImageStyleHeight).arg(iHeight).append("px"));
						}
						sUTF8Text += QString(sCharacterImage).arg(strImagePath).arg(sImageStyle);
					}
				}
			}
			else if (uiInfoScreenChar > 0)
			{
				if (bInHTMLEntityEncoderScope == false)
				{
					sUTF8Text += sHTMLEntityEncoderHeader;
					bInHTMLEntityEncoderScope = true;
				}
				{
					static QChar const qcCharacterSingleQuote('\'');
					static QString const sEscapedCharacterSingleQuote("\\\'");
					static QChar const qcCharacterDoubleQuote('\"');
					static QString const sEscapedCharacterDoubleQuote("\\\"");
					static QChar const qcCharacterSlash('\\');
					static QString const sEscapedCharacterSlash("\\\\");

					QChar qcUTF8(uiInfoScreenChar);
					if (qcUTF8 == qcCharacterSingleQuote)
					{
						sUTF8Text += sEscapedCharacterSingleQuote;
					}
					else if (qcUTF8 == qcCharacterDoubleQuote)
					{
						sUTF8Text += sEscapedCharacterDoubleQuote;
					}
					else if (qcUTF8 == qcCharacterSlash)
					{
						sUTF8Text += sEscapedCharacterSlash;
					}
					else
					{
						sUTF8Text += qcUTF8;
					}
				}
			}
			// ignore all other characters;
		}
		if (bInHTMLEntityEncoderScope == true)
		{
			sUTF8Text += sHTMLEntityEncoderFooter;
			bInHTMLEntityEncoderScope = false;
		}
	}

	return sUTF8Text;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
enum
{
	eInfScrnTxtAttr_Visible 				= 0,
	eInfScrnTxtAttr_Invisible 				= 1,
	eInfScrnTxtAttr_EraseLineBeforeDisplay 	= 2,
	eInfScrnTxtAttr_StartFlash 				= 6,
	eInfScrnTxtAttr_StopFlash 				= 7,
	eInfScrnTxtAttr_ScrollMedium 			= 8,
	eInfScrnTxtAttr_StopScroll 				= 9,
	eInfScrnTxtAttr_CenterText 				= 10,
	eInfScrnTxtAttr_LeftText 				= 11,
	eInfScrnTxtAttr_RightText 				= 12,
	eInfScrnTxtAttr_UnderscoreOn 			= 13,
	eInfScrnTxtAttr_CenterWithUnderscore 	= 14,
	eInfScrnTxtAttr_LeftWithUnderscore 		= 15,
	eInfScrnTxtAttr_RightWithUnderscore 	= 16,
	eInfScrnTxtAttr_UnderscoreOff 			= 17,
	eInfScrnTxtAttr_HorizontalScrollSlow 	= 18,
	eInfScrnTxtAttr_HorizontalScrollMedium 	= 19,
	eInfScrnTxtAttr_HorizontalScrollFast 	= 20,
	eInfScrnTxtAttr_PopTextSlow 			= 21,
	eInfScrnTxtAttr_PopTextMedium 			= 22,
	eInfScrnTxtAttr_PopTextFast 			= 23,
	eInfScrnTxtAttr_VerticalScrollSlow 		= 24,
	eInfScrnTxtAttr_VerticalScrollMedium 	= 25,
	eInfScrnTxtAttr_VerticalScrollFast 		= 26
};

//	---------------------------
//	V E F U PSMF HSMF VSMF CRLJ
//	0 0 0 0   00   00   00   00
//	---------------------------
enum
{
//	---------------------------------------------------
	eInfScrnTxtAttrFlag_Mask 					= 0xFFF,
	eInfScrnTxtAttrFlag_Visible 				= 0x800,
	eInfScrnTxtAttrFlag_EraseLineBeforeDisplay 	= 0x400,
	eInfScrnTxtAttrFlag_Flash 					= 0x200,
	eInfScrnTxtAttrFlag_Underscore 				= 0x100,
//	---------------------------------------------------
	eInfScrnTxtAttrFlag_PopMask 				= 0x0C0,
	eInfScrnTxtAttrFlag_PopTextFast 			= 0x0C0,
	eInfScrnTxtAttrFlag_PopTextMedium 			= 0x080,
	eInfScrnTxtAttrFlag_PopTextSlow 			= 0x040,
//	---------------------------------------------------
	eInfScrnTxtAttrFlag_HorizontalScrollMask 	= 0x030,
	eInfScrnTxtAttrFlag_HorizontalScrollFast 	= 0x030,
	eInfScrnTxtAttrFlag_HorizontalScrollMedium 	= 0x020,
	eInfScrnTxtAttrFlag_HorizontalScrollSlow 	= 0x010,
//	---------------------------------------------------
	eInfScrnTxtAttrFlag_VerticalScrollMask 		= 0x00C,
	eInfScrnTxtAttrFlag_VerticalScrollFast 		= 0x00C,
	eInfScrnTxtAttrFlag_VerticalScrollMedium 	= 0x008,
	eInfScrnTxtAttrFlag_VerticalScrollSlow 		= 0x004,
//	---------------------------------------------------
	eInfScrnTxtAttrFlag_AligmentMask 			= 0x003,
	eInfScrnTxtAttrFlag_AligmentCenterText 		= 0x003,
	eInfScrnTxtAttrFlag_AligmentRightText 		= 0x002,
	eInfScrnTxtAttrFlag_AligmentLeftText 		= 0x001,
	eInfScrnTxtAttrFlag_AligmentNoneText 		= 0x000
//	---------------------------------------------------
};

unsigned int GPersonalityView::ObtainTextFieldAttrFlag (unsigned int uiTxtAttr)
{
	LOG_LEVEL4("ObtainTextFieldAttrFlag()");

	QMutexLocker oLocker(&m_mutex_instance);

	unsigned int uiTxtAttrFlag = static_cast<unsigned int>(eInfScrnTxtAttrFlag_Visible);
	switch (uiTxtAttr)
	{
		case eInfScrnTxtAttr_EraseLineBeforeDisplay:
		{
			uiTxtAttrFlag = static_cast<unsigned int>(eInfScrnTxtAttrFlag_Visible | eInfScrnTxtAttrFlag_AligmentNoneText | eInfScrnTxtAttrFlag_EraseLineBeforeDisplay);
			break;
		}
		case eInfScrnTxtAttr_CenterText:
		{
			uiTxtAttrFlag = static_cast<unsigned int>(eInfScrnTxtAttrFlag_Visible | eInfScrnTxtAttrFlag_AligmentCenterText);
			break;
		}
		case eInfScrnTxtAttr_RightText:
		{
			uiTxtAttrFlag = static_cast<unsigned int>(eInfScrnTxtAttrFlag_Visible | eInfScrnTxtAttrFlag_AligmentRightText);
			break;
		}
		case eInfScrnTxtAttr_UnderscoreOn:
		{
			uiTxtAttrFlag = static_cast<unsigned int>(eInfScrnTxtAttrFlag_Visible | eInfScrnTxtAttrFlag_AligmentNoneText | eInfScrnTxtAttrFlag_Underscore);
			break;
		}
		case eInfScrnTxtAttr_LeftWithUnderscore:
		{
			uiTxtAttrFlag = static_cast<unsigned int>(eInfScrnTxtAttrFlag_Visible | eInfScrnTxtAttrFlag_AligmentLeftText | eInfScrnTxtAttrFlag_Underscore);
			break;
		}
		case eInfScrnTxtAttr_CenterWithUnderscore:
		{
			uiTxtAttrFlag = static_cast<unsigned int>(eInfScrnTxtAttrFlag_Visible | eInfScrnTxtAttrFlag_AligmentCenterText | eInfScrnTxtAttrFlag_Underscore);
			break;
		}
		case eInfScrnTxtAttr_RightWithUnderscore:
		{
			uiTxtAttrFlag = static_cast<unsigned int>(eInfScrnTxtAttrFlag_Visible | eInfScrnTxtAttrFlag_AligmentRightText | eInfScrnTxtAttrFlag_Underscore);
			break;
		}
		case eInfScrnTxtAttr_PopTextSlow:
		case eInfScrnTxtAttr_PopTextMedium:
		case eInfScrnTxtAttr_PopTextFast:
		case eInfScrnTxtAttr_ScrollMedium:
		case eInfScrnTxtAttr_HorizontalScrollSlow:
		case eInfScrnTxtAttr_HorizontalScrollMedium:
		case eInfScrnTxtAttr_HorizontalScrollFast:
		case eInfScrnTxtAttr_VerticalScrollSlow:
		case eInfScrnTxtAttr_VerticalScrollMedium:
		case eInfScrnTxtAttr_VerticalScrollFast:
		{
			uiTxtAttrFlag = static_cast<unsigned int>(eInfScrnTxtAttrFlag_Visible | eInfScrnTxtAttrFlag_AligmentNoneText | eInfScrnTxtAttrFlag_HorizontalScrollMedium);
			break;
		}
		case eInfScrnTxtAttr_StartFlash:
		case eInfScrnTxtAttr_Visible:
		case eInfScrnTxtAttr_Invisible:
		case eInfScrnTxtAttr_StopFlash:
		case eInfScrnTxtAttr_StopScroll:
		case eInfScrnTxtAttr_LeftText:
		case eInfScrnTxtAttr_UnderscoreOff:
		default:
		{
			uiTxtAttrFlag = static_cast<unsigned int>(eInfScrnTxtAttrFlag_Visible | eInfScrnTxtAttrFlag_AligmentNoneText);
			break;
		}
	}
	return uiTxtAttrFlag;
}

QString GPersonalityView::GenerateSegmentScript(QString const & sContent, QString const & sSegmentName, QString const & sSegmentParent, QRect const & qPosition, bool bIsVisible, bool bFloatingSegment)
{
	LOG_LEVEL4("GenerateSegmentScript()");

	QMutexLocker oLocker(&m_mutex_instance);

	QString sSegmentScript("var Segment = new GraphicSegment('");
	{
		sSegmentScript += sSegmentName+QString("',");
		sSegmentScript += sSegmentParent+QString(",");
		if (!sContent.isEmpty())
		{
			sSegmentScript += sContent+QString(",");
		}
		else
		{
			sSegmentScript += QString("'',");
		}
		sSegmentScript += QString::number(qPosition.x())+QString(",");
		sSegmentScript += QString::number(qPosition.y())+QString(",");
		sSegmentScript += QString::number(qPosition.width())+QString(",");
		sSegmentScript += QString::number(qPosition.height())+QString(")");
		if (!bIsVisible)
		{
			sSegmentScript += QString(".SetVisibility(false)");
		}
		if (bFloatingSegment)
		{
			sSegmentScript += QString(".Build(true);");
		}
		else
		{
			sSegmentScript += QString(".Build(false);");
		}

		LOG_LEVEL3("********** Segment Script String ************");
		LOG_LEVEL3(sSegmentScript);
	}
	return sSegmentScript;
}

QString GPersonalityView::GenerateTextFieldScript(QString const & sText, QString const & sFieldName, QString const & sFieldParent, QPoint const & qPosition, unsigned int uiTxtAttrFlag, QString const & sFontName, QString const & sBackgroundColour, QString const & sForegroundColour, bool bFloatingText, bool bRemoveOverlaps, unsigned int uiOverlapMargin = 10)
{
	LOG_LEVEL4("GenerateTextFieldScript()");

	QMutexLocker oLocker(&m_mutex_instance);

	QString sFieldScript("var Field = new TextField('");
	{
		sFieldScript += sFieldName+QString("','");
		sFieldScript += sFontName+QString("',");
		if (!sFieldParent.isEmpty())
		{
			sFieldScript += sFieldParent+QString(",");
		}
		else
		{
			sFieldScript += QString("'',");
		}
		if (!sText.isEmpty())
		{
			sFieldScript += sText+QString(",");
		}
		else
		{
			sFieldScript += QString("'',");
		}
		if (bRemoveOverlaps)
		{
			sFieldScript += QString("true,");
		}
		else
		{
			sFieldScript += QString("false,");
		}
		sFieldScript += QString::number(uiOverlapMargin)+QString(")");

		sFieldScript += QString(".SetX(")+QString::number(qPosition.x())+QString(")");
		sFieldScript += QString(".SetY(")+QString::number(qPosition.y())+QString(")");

		{
			uiTxtAttrFlag &= eInfScrnTxtAttrFlag_Mask;
			if( uiTxtAttrFlag & eInfScrnTxtAttrFlag_EraseLineBeforeDisplay )
			{
				sFieldScript += QString(".SetEraseLine(true)");
			}
			switch (uiTxtAttrFlag & eInfScrnTxtAttrFlag_AligmentMask)
			{
				case eInfScrnTxtAttrFlag_AligmentCenterText:
				{
					sFieldScript += QString(".SetAligmentCenter()");
					break;
				}
				case eInfScrnTxtAttrFlag_AligmentRightText:
				{
					sFieldScript += QString(".SetAligmentRigth()");
					break;
				}
				case eInfScrnTxtAttrFlag_AligmentLeftText:
				{
					sFieldScript += QString(".SetAligmentLeft()");
					break;
				}
				case eInfScrnTxtAttrFlag_AligmentNoneText:
				default:
				{
					sFieldScript += QString(".SetAligmentNone()");
					break;
				}
			}
			if ((uiTxtAttrFlag & eInfScrnTxtAttrFlag_Visible) == 0)
			{
				sFieldScript += QString(".SetVisibility(false)");
			}
			if (uiTxtAttrFlag & eInfScrnTxtAttrFlag_Underscore)
			{
				sFieldScript += QString(".SetUnderLine(true)");
			}
			switch (uiTxtAttrFlag & eInfScrnTxtAttrFlag_PopMask)
			{
				case eInfScrnTxtAttrFlag_PopTextFast:
				case eInfScrnTxtAttrFlag_PopTextMedium:
				case eInfScrnTxtAttrFlag_PopTextSlow:
				default:
					break;
			}
			switch (uiTxtAttrFlag & eInfScrnTxtAttrFlag_HorizontalScrollMask)
			{
				case eInfScrnTxtAttrFlag_HorizontalScrollFast:
				case eInfScrnTxtAttrFlag_HorizontalScrollMedium:
				{
					sFieldScript += QString(".SetScroll(true)");
					break;
				}
				case eInfScrnTxtAttrFlag_HorizontalScrollSlow:
				default:
					break;
			}
			switch (uiTxtAttrFlag & eInfScrnTxtAttrFlag_VerticalScrollMask)
			{
				case eInfScrnTxtAttrFlag_VerticalScrollFast:
				case eInfScrnTxtAttrFlag_VerticalScrollMedium:
				case eInfScrnTxtAttrFlag_VerticalScrollSlow:
				default:
					break;
			}
			if (uiTxtAttrFlag & eInfScrnTxtAttrFlag_Flash)
			{
			}
			if( !sBackgroundColour.isEmpty() )
			{
				sFieldScript += QString(".SetBackgroundColor('")+sBackgroundColour+QString("')");
			}
			if( !sForegroundColour.isEmpty() )
			{
				sFieldScript += QString(".SetColor('")+sForegroundColour+QString("')");
			}
		}
		if (bFloatingText)
		{
			sFieldScript += QString(".Build(true);");
		}
		else
		{
			sFieldScript += QString(".Build(false);");
		}

		LOG_LEVEL3("********** Field Script String ************");
		LOG_LEVEL3(sFieldScript);
	}
	return sFieldScript;
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

