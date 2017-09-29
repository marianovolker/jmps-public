#include "GURLView.h"

#include "utils/logger/src/GLogger.h"
#include "model/src/urlmodel/GURLModelSingleton.h"

QSharedPointer<GURLView> GURLView::m_pGURLView;
QMutex GURLView::m_mutex;

GURLView::GURLView()
{
	LOG_LEVEL4("GURLView()");

	m_sName = "GURLView";
}

GURLView::~GURLView()
{
	LOG_LEVEL4("~GURLView()");
}

void GURLView::Init()
{
	LOG_LEVEL4("Init()");

	GAbsView::Init();
}

void GURLView::Update(bool bSynchronize)
{
	LOG_LEVEL4("Update()");

	QSharedPointer<GURLModel> pModel = GURLModelSingleton::GetInstance()->GetModel();
	QString strURL(pModel->GetUrl());

	QStringList oStrListJSCalls;
	static QString const jsUrl("javascript:");

	if( strURL.contains(jsUrl) )
	{
		oStrListJSCalls.append(strURL.remove(0, jsUrl.length()));
		UpdateURLViewContent(bSynchronize, oStrListJSCalls);
	}
	else
	{
		static bool const bApplyZoomFactor(true);
		QStringList oStrListJSLibraries;
		SetView(bSynchronize, oStrListJSCalls, oStrListJSLibraries, strURL, bApplyZoomFactor);
	}
}

QSharedPointer<GURLView> GURLView::GetInstance() throw(GException)
{
	LOG_LEVEL4("GetInstance()");

	QMutexLocker oLocker(&GURLView::m_mutex);

	if( m_pGURLView.isNull() )
	{
		m_pGURLView = QSharedPointer<GURLView>(new GURLView());

		if( !m_pGURLView.isNull() )
		{
			m_pGURLView->Init();
		}
	}

	if( m_pGURLView.isNull() )
	{
		 throw(GException("GURLView::GetInstance() made a NULL pointer."));
	}

	return m_pGURLView;
}
