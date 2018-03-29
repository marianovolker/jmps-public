#include "GScreen.h"

#include "GDisplay.h"

const QString GScreen::CXMLTag				("Screen");
const QString GScreen::CXMLNumber			("number");
const QString GScreen::CXMLDisplayNumber	("displayNumber");
const QString GScreen::CXMLBGColor			("bgColor");


GScreen::GScreen(int number, GDisplay *oDisplay)
	: m_iNumber	(number),
	  m_oDisplay(oDisplay),
	  m_iBGColor(0)
{
	m_oSegments = new QMap<int, GSegment *>;
}

GScreen::~GScreen()
{	
	QMapIterator<int, GSegment *> oIterator(*m_oSegments);
	
	while(oIterator.hasNext()) 
	{
		delete oIterator.next().value();
	}		 
	
	delete m_oSegments;	
	m_oSegments = NULL;
}

int GScreen::GetNumber() 
{
	return m_iNumber;
}

GDisplay* GScreen::GetDisplay()
{
	return m_oDisplay;
}

int GScreen::GetBGColor() 
{
	return m_iBGColor;
}

void GScreen::SetBGColor(int iColor) 
{
	m_iBGColor = iColor;
}


void GScreen::AddSegment(GSegment* oSegment)
{
	m_oSegments->insert(oSegment->GetNumber(), oSegment);
}

GSegment* GScreen::GetSegment(int iNumber)
{	
	QMap<int, GSegment *>::const_iterator i = m_oSegments->find(iNumber);
	return (i != m_oSegments->end())? i.value() : NULL;	
}

QList<GSegment *> GScreen::GetAllSegments()
{
	return m_oSegments->values();
}

void GScreen::RemoveSegment(int iNumber)
{
	GSegment* oSegment = GetSegment(iNumber);
	
	if(oSegment != NULL) 
	{			
		m_oSegments->remove(iNumber);
		delete oSegment;
	}		
}

bool GScreen::IsPersistent()
{
	return (GetNumber() > 0);
}
