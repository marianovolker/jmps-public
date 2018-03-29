#include "GISView.h"

#include <memory>
#include <QFile>

#include "utils/logger/src/GLogger.h"
#include "model/src/infoscreenmodel/GISModelSingleton.h"
#include "config/GDisplayManagerConfig.h"
#include "utils/image/src/GImageConverter.h"
#include "utils/image/src/GImageMappings.h"
#include "utils/image/src/GImageHelper.h"
#include "utils/image/src/GImageMap.h"
#include "view/src/utils/GPersonalityView.h"

QSharedPointer<GISView> GISView::m_pGISView;
QMutex GISView::m_mutex;

GISView::GISView()
{
	LOG_LEVEL4("GISView()");

	m_sName = "GISView";
}

GISView::~GISView()
{
	LOG_LEVEL4("~GISView()");
}

void GISView::Init()
{
	LOG_LEVEL4("Init()");

	GAbsView::Init();
}

void GISView::Clear(bool bSynchronize)
{
	LOG_LEVEL4("Clear()");

	LOG_LEVEL3("Updating current DOM.");

	QStringList oStrListJSCalls;

    static QString const qksJSCall1("if(SectionLoadOrderControler){SectionLoadOrderControler.Clear();}");
    oStrListJSCalls << QString(qksJSCall1);

	SetViewContent(bSynchronize, oStrListJSCalls);
}

void GISView::Update(bool bSynchronize)
{
	LOG_LEVEL4("Update()");

	LOG_LEVEL3("Updating current DOM.");

	QString strPersonalityURL;
	QStringList oStrListJSCalls;

    static QString const qksJSCall1("if(SectionLoadOrderControler){SectionLoadOrderControler.Clear();}");
    oStrListJSCalls << QString(qksJSCall1);

	UpdateDom(oStrListJSCalls, strPersonalityURL);

	if( strPersonalityURL.isEmpty() )
	{
		SetViewContent(bSynchronize, oStrListJSCalls);
	}
	else
	{
		QStringList oStrListJSLibraries;
		oStrListJSLibraries << GETCONF_STR(eDisplayManagerJQueryLibraryFileName);
		SetView(bSynchronize, oStrListJSCalls, oStrListJSLibraries, strPersonalityURL);
	}
}

void GISView::UpdateSection(GTextField* pTextField, bool bSynchronize)
{
	LOG_LEVEL4("UpdateSection()");

	LOG_LEVEL3("Updating current DOM.");

	QStringList oStrListJSCalls;

    static QString const qksScreenNameWrapper("'%1'");
    static QString const qksScreemName("body");

	UpdateTextField(pTextField,QString(qksScreenNameWrapper).arg(qksScreemName), oStrListJSCalls, true);

	delete pTextField;

	UpdateViewContent(bSynchronize, oStrListJSCalls);
}

void GISView::UpdateSection(GSegment* pSegment, bool bSynchronize)
{
	LOG_LEVEL4("UpdateSection()");

	LOG_LEVEL3("Updating current DOM.");

	QStringList oStrListJSCalls;

    static QString const qksScreenNameWrapper("'%1'");
    static QString const qksScreemName("body");
    static QString const qksSegmentNameWrapper("'#%1'");
    static QString const qksSegmentName("segment_%1");
	int iSegmentNumber = pSegment->GetNumber();

    {
		static QString const qksPreviousSegmentEraseScript("$(%1).remove();");
		oStrListJSCalls << QString(qksPreviousSegmentEraseScript).arg(QString(qksSegmentNameWrapper).arg(QString(qksSegmentName).arg(iSegmentNumber)));
    }

	UpdateSegment(pSegment, QString(qksScreenNameWrapper).arg(qksScreemName), oStrListJSCalls);

	delete pSegment;

	UpdateViewContent(bSynchronize, oStrListJSCalls);
}

void GISView::UpdateDom(QStringList& rStrListJSCalls, QString &strPersonalityURL)
{
	LOG_LEVEL4("UpdateDom()");

	QSharedPointer<GISModel> pISModel = GISModelSingleton::GetInstance()->GetModel();

	if( pISModel.isNull() )
	{
		 throw(GException("GISView::UpdateDom() obtained a GISModel NULL pointer."));
	}

	QList<GDisplay *> oDisplayList(pISModel->GetAllDisplays());
	if( oDisplayList.size() == 0 )
	{
		// We assume there is nothing to show
		return;
	}
	else if( oDisplayList.size() > 1 )
	{
		throw(GException("We should only have one Display"));
	}
	else
	{
		UpdateScreen(oDisplayList[0]->GetCurrentScreen(), rStrListJSCalls, strPersonalityURL);
	}
}

void GISView::UpdateScreen(GScreen* pScreen, QStringList& rStrListJSCalls, QString &strPersonalityURL)
{
	LOG_LEVEL4("UpdateScreen()");

    static QString const qksScreenNameWrapper("'%1'");
    static QString const qksScreemName("body");

	if( pScreen == NULL )
	{
		return;
	}

	QSharedPointer<GPersonalityView> pGPersonalityView = GPersonalityView::GetInstance();
	if( pGPersonalityView.isNull() )
	{
		throw(GException("GISView::UpdateTextField() obtained a GPersonalityView NULL pointer."));
	}

	pGPersonalityView->ObtainCustomScreenURL( strPersonalityURL, pScreen->GetNumber() );

	// Iterate through the Segments
	QList<GSegment *> oSegmentList(pScreen->GetAllSegments());
	QList<GSegment *>::iterator oSegmentIterator = oSegmentList.begin();
	GSegment* pSegment;

	while( oSegmentIterator != oSegmentList.end() )
	{
		pSegment = (GSegment *) *oSegmentIterator;
		UpdateSegment(pSegment,QString(qksScreenNameWrapper).arg(qksScreemName),rStrListJSCalls);
		++oSegmentIterator;
	}
}

void GISView::UpdateSegment(GSegment* pSegment, QString const & sParentName, QStringList& rStrListJSCalls)
{
	LOG_LEVEL4("UpdateSegment()");

	if( pSegment == NULL )
	{
		 throw(GException("GISView::UpdateSegment() received a GSegment NULL pointer."));
	}

	if( pSegment->IsVisible() )
	{
		QSharedPointer<GPersonalityView> pGPersonalityView = GPersonalityView::GetInstance();
		if( pGPersonalityView.isNull() )
		{
			throw(GException("GISView::UpdateTextField() obtained a GPersonalityView NULL pointer."));
		}

		int iXPos = pSegment->GetXPos();
		int iYPos = pSegment->GetYPos();

		GetScaledXY(iXPos, iYPos);

		int iWidth = pSegment->GetWidth();
		int iHeight = pSegment->GetHeight();

		GetScaledWidthAndHeight(iWidth, iHeight);


		int iSegmentNumber = pSegment->GetNumber();

		QString strImgTag("");
		{
			static QString const qksImgTag("'<img _src=\"%1\"/>'");
			QString strImagePath("");
			if ( ObtainImagePath(strImagePath, iSegmentNumber) )
			{
				strImgTag = QString(qksImgTag).arg(strImagePath);
			}
		}

		static QString const qksSegmentName("segment_%1");

		rStrListJSCalls << pGPersonalityView->GenerateSegmentScript(strImgTag,QString(qksSegmentName).arg(iSegmentNumber),sParentName,QRect(iXPos,iYPos,iWidth,iHeight),pSegment->IsVisible());

		QList<GTextField *> oFieldList(pSegment->GetAllFields());
		QList<GTextField *>::iterator oFieldIterator(oFieldList.begin());
		GTextField* pField;

		while (oFieldIterator != oFieldList.end())
		{
			static QString const qksSegmentNameWrapper("'#%1'");
			pField = (GTextField *) *oFieldIterator;
			UpdateTextField(pField,QString(qksSegmentNameWrapper).arg(QString(qksSegmentName).arg(iSegmentNumber)),rStrListJSCalls,false);
			++oFieldIterator;
		}
	}
}

void GISView::UpdateTextField(GTextField* pField, QString const & sParentName, QStringList& rStrListJSCalls, bool bFreeText)
{
	LOG_LEVEL4("UpdateTextField()");

	if( pField == NULL )
	{
		 throw(GException("GISView::UpdateTextField() received a GTextField NULL pointer."));
	}

	QSharedPointer<GPersonalityView> pGPersonalityView = GPersonalityView::GetInstance();
	if( pGPersonalityView.isNull() )
	{
		throw(GException("GISView::UpdateTextField() obtained a GPersonalityView NULL pointer."));
	}

	int iXPos = pField->GetXpos();
	int iYPos = pField->GetYpos();

	GetScaledXY(iXPos, iYPos);

    static QString const qksFieldName("field_%1");
    static QString const qksFontName("field_%1");
    static QString const qksFontNameOpaque("field_%1_opaque");
    static QString const qksBackgroundColor("");
    static QString const qksForegroundColor("");
    static QString const qksTextWrapper("'%1'");
    bool bRemoveOverlappings = pGPersonalityView->MayRemoveOverlaps();
    unsigned int uiOverlappingsMargin = pGPersonalityView->ObtainOverlapsErrorMarginPixels();

    rStrListJSCalls << pGPersonalityView->GenerateTextFieldScript(QString(qksTextWrapper).arg(pGPersonalityView->ConvertExtendedASCIIToUTF8(pField->GetText())),QString(qksFieldName).arg(pField->GetNumber()),sParentName,QPoint(iXPos,iYPos),pGPersonalityView->ObtainTextFieldAttrFlag(pField->GetAttribute()),QString((((!bFreeText)||(bRemoveOverlappings))?(qksFontName):(qksFontNameOpaque))).arg(pField->GetFont()),qksBackgroundColor,qksForegroundColor,bFreeText,bRemoveOverlappings,uiOverlappingsMargin);
}

bool GISView::ObtainImagePath ( QString &strImagePath, int iSegmentNumber ) const
{
    LOG_LEVEL4("ObtainImagePath()");

	QSharedPointer<GPersonalityView> pGPersonalityView = GPersonalityView::GetInstance();
	if( pGPersonalityView.isNull() )
	{
		throw(GException("GISView::UpdateTextField() obtained a GPersonalityView NULL pointer."));
	}

    QSharedPointer<GImageMappings> poImageMaps(GImageMappings::GetInstance());
    {
        QMutexLocker mtxImageMappingsScope(poImageMaps->GetMutex());

        GImageMap *poSegmentImageMap( NULL );

        if( ( poSegmentImageMap = ( poImageMaps->GetImageMap( iSegmentNumber ) ) ) != NULL )
        {
            // GImageMappings has an image for current segment
            if ( !pGPersonalityView->ObtainImageFromPersonality( strImagePath, iSegmentNumber) )
            {
                // No Image for this segment in personality or image file not found
                if ( !QFile::exists( strImagePath = ( GImageHelper::CurrentResolutionColorImagesAbsolutePath() + poSegmentImageMap->GetFileName() ) ) )
                {
                    //No Color Image for current resolution
                    if ( !QFile::exists( strImagePath = ( GImageHelper::CurrentResolutionMonochromeImagesAbsolutePath() + poSegmentImageMap->GetFileName() ) ) )
                    {
                        //No Monochrome Image for current resolution
                        QString strLogMsg(QString ("No image could be found for segment %1.").arg(iSegmentNumber));
                        LOG_LEVEL2(strLogMsg);

                        return false;
                    }
                }
            }
        }
        else
        {
            return false;
        }
    }


    return true;
}

QSharedPointer<GISView> GISView::GetInstance() throw(GException)
{
	QMutexLocker oLocker(&GISView::m_mutex);

	LOG_LEVEL4("GetInstance()");

	if( m_pGISView.isNull() )
	{
		m_pGISView = QSharedPointer<GISView>(new GISView());

		if( !m_pGISView.isNull() )
		{
			m_pGISView->Init();
		}
	}

	if( m_pGISView.isNull() )
	{
		 throw(GException("GISView::GetInstance() made a NULL pointer."));
	}

	return m_pGISView;
}
