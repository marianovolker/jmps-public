#include "GISModel.h"

#include "utils/logger/src/GLogger.h"
#include "view/src/infoscreenview/GISView.h"


const QString GISModel::CXMLTag("Model");

GISModel::GISModel()
{
	LOG_LEVEL4("GISModel()");

	m_oDisplays = new QMap<int, GDisplay *>;
}

GISModel::~GISModel()
{
	LOG_LEVEL4("~GISModel()");

	QMapIterator<int, GDisplay *> oIterator(*m_oDisplays);
	while (oIterator.hasNext())
	{ 
		delete oIterator.next().value();		
	}	
	
	delete m_oDisplays;	
	m_oDisplays = NULL;
}

void GISModel::AddDisplay(GDisplay* oDisplay)
{

	LOG_LEVEL4("AddDisplay()");

	m_oDisplays->insert(oDisplay->GetNumber(), oDisplay);
}

GDisplay* GISModel::GetDisplay(int iNumber) const
{
	LOG_LEVEL4("GetDisplay()");

	QMap<int, GDisplay *>::const_iterator i = m_oDisplays->find(iNumber);
	return (i != m_oDisplays->end())? i.value() : NULL;		
}

QList<GDisplay *> GISModel::GetAllDisplays() const
{
	LOG_LEVEL4("GetAllDisplays()");

	return m_oDisplays->values();
}

void GISModel::RemoveDisplay(int iNumber)
{
	LOG_LEVEL4("RemoveDisplay()");

	GDisplay* oDisplay = GetDisplay(iNumber);
	
	if(oDisplay != NULL) 
	{			
		m_oDisplays->remove(iNumber);
		delete oDisplay;
	}
}

void GISModel::RemoveAllDisplays()
{
	LOG_LEVEL4("RemoveAllDisplays()");

	QMapIterator<int, GDisplay *> oIterator(*m_oDisplays);
	while (oIterator.hasNext())
	{
		delete oIterator.next().value();
	}

	m_oDisplays->clear();
}

void GISModel::ClearView(bool bSynchronize)
{
	LOG_LEVEL4("ClearView()");

	QSharedPointer<GISView> poISView = GISView::GetInstance();
	poISView->Clear(bSynchronize);
}
void GISModel::UpdateView(bool bSynchronize)
{
	LOG_LEVEL4("UpdateView()");

	QSharedPointer<GISView> poISView = GISView::GetInstance();
	poISView->Update(bSynchronize);
}

void GISModel::UpdateViewSection(GTextField * pTextField, bool bSynchronize)
{
	LOG_LEVEL4("UpdateViewSection()");

	QSharedPointer<GISView> poISView = GISView::GetInstance();
	poISView->UpdateSection(pTextField,bSynchronize);
}

void GISModel::UpdateViewSection(GSegment * pSegment, bool bSynchronize)
{
	LOG_LEVEL4("UpdateViewSection()");

	QSharedPointer<GISView> poISView = GISView::GetInstance();
	poISView->UpdateSection(pSegment,bSynchronize);
}


QMutex *GISModel::GetMutex()
{
    return &m_mutex;
}

