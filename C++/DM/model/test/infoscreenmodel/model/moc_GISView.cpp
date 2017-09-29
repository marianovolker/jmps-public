#include "moc_GISView.h"

#include "utils/logger/src/GLogger.h"

GISView*  GISView::m_pGISView = NULL;

GISView* GISView::GetInstance() throw(GException)
{
	LOG_LEVEL4("GetInstance()");

	if( m_pGISView == NULL )
	{
		m_pGISView = new GISView();

		if( m_pGISView != NULL )
		{
			m_pGISView->Init();
		}
	}

	if( m_pGISView == NULL )
	{
		 throw(GException("GISView::GetInstance() made a NULL pointer."));
	}

	return m_pGISView;
}

GISView::GISView()
{
	LOG_LEVEL4("GISView()");
}

GISView::~GISView()
{
	LOG_LEVEL4("~GISView()");
}

void GISView::Init()
{
	LOG_LEVEL4("Init()");
}

void GISView::Update(bool bSynchronize)
{
	LOG_LEVEL4(QString("Update() [")+((bSynchronize)?("true"):("false"))+"]");
}

void GISView::UpdateSection(GTextField* p, bool bSynchronize)
{
	LOG_LEVEL4(QString("UpdateSection() GTextField.number[") + QString::number(p->GetNumber()) + "][" + ((bSynchronize)?("true"):("false")) + "]" );
}

void GISView::UpdateSection(GSegment* p, bool bSynchronize)
{
	LOG_LEVEL4(QString("UpdateSection() GSegment.number[") + QString::number(p->GetNumber()) + "][" + ((bSynchronize)?("true"):("false")) + "]" );
}

void GISView::Clear(bool bSynchronize)
{
	LOG_LEVEL4(QString("Clear() [")+((bSynchronize)?("true"):("false"))+"]");
}
