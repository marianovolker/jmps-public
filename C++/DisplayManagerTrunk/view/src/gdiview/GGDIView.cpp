#include "GGDIView.h"

#include "utils/logger/src/GLogger.h"
#include "config/GDisplayManagerConfig.h"
#include "view/src/utils/GPersonalityView.h"
#include "model/src/gdimodel/GGDILine.h"
#include "model/src/gdimodel/GGDIRect.h"
#include "model/src/gdimodel/GGDIText.h"

QSharedPointer<GGDIView> GGDIView::m_pGGDIView;
QMutex GGDIView::m_mutex;

GGDIView::GGDIView()
{
	LOG_LEVEL4("GGDIView()");

	m_sName = "GGDIView";
}

GGDIView::~GGDIView()
{
	LOG_LEVEL4("~GGDIView()");
}

void GGDIView::Init()
{
	LOG_LEVEL4("Init()");

	GAbsView::Init();
}

void GGDIView::ClearView(bool bSynchronize)
{
	LOG_LEVEL4("ClearView()");

	m_rStrListJSCalls.clear();

    static QString const qksJSCall1("if(SectionLoadOrderControler){SectionLoadOrderControler.Clear();}");
    static QString const qksJSCall2("$('<div id=\"screen\" style=\"position: absolute; top: 0px; left 0px; width: 0px; height: 0px; overflow: hidden;\"/>').appendTo('body');");

    m_rStrListJSCalls << QString(qksJSCall1);
    m_rStrListJSCalls << QString(qksJSCall2);

	SetViewContent(bSynchronize, m_rStrListJSCalls);
}

void GGDIView::DrawLine(GGDILine& oLine, bool bSynchronize)
{
	LOG_LEVEL4("DrawLine()");

	m_rStrListJSCalls.clear();

    static QString const qksExpressionWrapper("'%1'");
    static QString const qksScreenName("#screen");
    static QString const qksNodeAttacher("$(%1).append(%2);");
    static QString const qksClass("gdi_drawline");
	static QString const qksStyle("style=\"position:absolute; left: %1 px; top: %2 px; width: %3 px; height: %4 px; background-color: #%5;\"");
	static QString const qksNewDiv("<div id=\"line_%1\" class=\"%2\" %3></div>");

	int iXPos = oLine.GetXLoc();
	int iYPos = oLine.GetYLoc();
//	In GDI the CRIND already scales the sizes and coordinates.
//	GetScaledXY(iXPos, iYPos);

	int iWidth  = oLine.GetLength();
	int iHeight = oLine.GetThickness().toInt();
//	In GDI the CRIND already scales the sizes and coordinates.
//	GetScaledWidthAndHeight(iWidth, iHeight);

	m_rStrListJSCalls << QString(qksNodeAttacher)
							.arg(QString(qksExpressionWrapper).arg(qksScreenName))
							.arg(QString(qksExpressionWrapper).arg(QString(qksNewDiv).arg(oLine.GetLineNumber()).arg(qksClass).arg(QString(qksStyle).arg(iXPos).arg(iYPos).arg(iWidth).arg(iHeight).arg(oLine.GetColor()))));
	UpdateViewContent(bSynchronize, m_rStrListJSCalls);
}

void GGDIView::DrawRect(GGDIRect& oRect, bool bSynchronize)
{
	LOG_LEVEL4("DrawRect()");

	m_rStrListJSCalls.clear();

    static QString const qksExpressionWrapper("'%1'");
    static QString const qksScreenName("#screen");
    static QString const qksNodeAttacher("$(%1).append(%2);");
    static QString const qksClass("gdi_drawrect");
	static QString const qksStyle("style=\"position:absolute; left: %1 px; top: %2 px; width: %3 px; height: %4 px; background-color: #%5;\"");
	static QString const qksNewDiv("<div id=\"rect_%1\" class=\"%2\" %3></div>");

	int iXPos = oRect.GetXLoc();
	int iYPos = oRect.GetYLoc();
//	In GDI the CRIND already scales the sizes and coordinates.
//	GetScaledXY(iXPos, iYPos);

	int iWidth  = oRect.GetWidth();
	int iHeight = oRect.GetHeight();
//	In GDI the CRIND already scales the sizes and coordinates.
//	GetScaledWidthAndHeight(iWidth, iHeight);

	m_rStrListJSCalls << QString(qksNodeAttacher)
							.arg(QString(qksExpressionWrapper).arg(qksScreenName))
							.arg(QString(qksExpressionWrapper).arg(QString(qksNewDiv).arg(oRect.GetRectNumber()).arg(qksClass).arg(QString(qksStyle).arg(iXPos).arg(iYPos).arg(iWidth).arg(iHeight).arg(oRect.GetColor()))));
	UpdateViewContent(bSynchronize, m_rStrListJSCalls);
}

void GGDIView::DrawText(GGDIText& oText, bool bSynchronize)
{
	LOG_LEVEL4("DrawText()");

	QSharedPointer<GPersonalityView> pGPersonalityView = GPersonalityView::GetInstance();
	if( pGPersonalityView.isNull() )
	{
		throw(GException("GGDIView::UpdateDom() obtained a GPersonalityView NULL pointer."));
	}

	m_rStrListJSCalls.clear();

	int iXPos = oText.GetXLoc();
	int iYPos = oText.GetYLoc();
//	In GDI the CRIND already scales the sizes and coordinates.
//	GetScaledXY(iXPos, iYPos);

    static QString const qksScreenNameWrapper("'%1'");
    static QString const qksScreenName("#screen");
    QString const sParentName(QString(qksScreenNameWrapper).arg(qksScreenName));
    static QString const qksFieldName("field_%1");
    static QString const qksFontName("field_%1");
    static QString const qksFontNameOpaque("field_%1_opaque");
    QString const qksBackgroundColor(oText.GetBGColor());
    QString const qksForegroundColor(oText.GetFGColor());
    static QString const qksTextWrapper("'%1'");
    static bool const bFreeText = true;
    bool bRemoveOverlappings = pGPersonalityView->MayRemoveOverlaps();
    unsigned int uiOverlappingsMargin = pGPersonalityView->ObtainOverlapsErrorMarginPixels();
    unsigned int uiTextAttr = oText.GetAttr().toUInt();
    QString qsFontName(oText.GetFont());
	unsigned int uiFontName(qsFontName.toUInt());
	if(qsFontName != "SM")  
	{
		qsFontName.setNum(uiFontName,10);
	}
	
    m_rStrListJSCalls <<
			pGPersonalityView->GenerateTextFieldScript
				(
					QString(qksTextWrapper).arg(pGPersonalityView->ConvertExtendedASCIIToUTF8(oText.GetText())),
					QString(qksFieldName).arg(oText.GetTextNumber()),
					sParentName,
					QPoint(iXPos,iYPos),
					pGPersonalityView->ObtainTextFieldAttrFlag(uiTextAttr),
					QString( (((!bFreeText)||(bRemoveOverlappings))?(qksFontName):(qksFontNameOpaque)) ).arg(qsFontName),
					qksBackgroundColor,
					qksForegroundColor,
					bFreeText,
					bRemoveOverlappings,
					uiOverlappingsMargin
				);
    UpdateViewContent(bSynchronize, m_rStrListJSCalls);
}

void GGDIView::Update(bool bSynchronize)
{
	LOG_LEVEL4("Update()");

	m_rStrListJSCalls.clear();

    static QString const qksJSCall("$('#screen').css({'width':'100%', 'height':'100%'});");

    m_rStrListJSCalls << QString(qksJSCall);
    UpdateViewContent(bSynchronize, m_rStrListJSCalls);
}

QSharedPointer<GGDIView> GGDIView::GetInstance() throw(GException)
{
	LOG_LEVEL4("GetInstance()");

	QMutexLocker oLocker(&GGDIView::m_mutex);

	if( m_pGGDIView.isNull() )
	{
		m_pGGDIView = QSharedPointer<GGDIView>(new GGDIView());

		if( !m_pGGDIView.isNull() )
		{
			m_pGGDIView->Init();
		}
	}

	if( m_pGGDIView.isNull() )
	{
		 throw(GException("GGDIView::GetInstance() made a NULL pointer."));
	}

	return m_pGGDIView;
}
