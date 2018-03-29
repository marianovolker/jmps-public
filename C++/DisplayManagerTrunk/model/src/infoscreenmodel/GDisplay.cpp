#include "GDisplay.h"

#include "GScreen.h"

const QString GDisplay::CXMLTag				("Display");
const QString GDisplay::CXMLNumber			("number");
const QString GDisplay::CXMLCurrentScreen	("currentScreenNumber");
const QString GDisplay::CXMLIsColor			("isColor");


GDisplay::GDisplay(int iNumber) : m_iNumber(iNumber)
{	
	m_oScreens = new QMap<int, GScreen *>;
	m_oColorPalette = new GColorPalette();
	m_oCurrentScreen = NULL;

	AddScreen(new GScreen(0,this));
}

GDisplay::~GDisplay()
{	
	QMapIterator<int, GScreen *> oIterator(*m_oScreens);
	
	while(oIterator.hasNext()) 
	{
		delete oIterator.next().value();
	}		 
	
	delete m_oColorPalette;
	m_oColorPalette = NULL;
	
	delete m_oScreens;
	m_oScreens = NULL;
}

int GDisplay::GetNumber()
{
	return m_iNumber;
}

void GDisplay::SetColor(bool bColor)
{
	m_bIsColor = bColor;
}

bool GDisplay::IsColor()
{
	return m_bIsColor;
}

void GDisplay::AddScreen(GScreen* oScreen)
{
	m_oScreens->insert(oScreen->GetNumber(), oScreen);

	if(m_oCurrentScreen == NULL) m_oCurrentScreen = oScreen;
}

GScreen* GDisplay::GetScreen(int iNumber)
{	
	QMap<int, GScreen *>::const_iterator i = m_oScreens->find(iNumber);

	return (i != m_oScreens->end())? i.value() : NULL;		
}

QList<GScreen *> GDisplay::GetAllScreens()
{
	return m_oScreens->values();
}

void GDisplay::RemoveScreen(int iNumber)
{
	if( iNumber > 0 )
	{
		GScreen* oScreen = GetScreen(iNumber);

		if( oScreen != NULL )
		{
			m_oScreens->remove(iNumber);
			delete oScreen;
		}
	}
}

GScreen* GDisplay::GetCurrentScreen()
{
	return m_oCurrentScreen;
}

bool GDisplay::IsCurrentScreen(GScreen *oScreen) 
{	
	return (m_oCurrentScreen == oScreen);
}

void GDisplay::SetCurrentScreen(GScreen *oScreen) 
{


	//Comment this if
	/*
	if (m_oCurrentScreen != NULL)
	{
		// Restore All Segment Positions;
		{
			QList<GSegment *> oSegmentList(m_oCurrentScreen->GetAllSegments());
			QList<GSegment *>::iterator oSegmentIterator = oSegmentList.begin();
			while( oSegmentIterator != oSegmentList.end() )
			{
				GSegment * oSegment(static_cast<GSegment *>(*oSegmentIterator));
				if (oSegment != NULL)
				{
					oSegment->ResetVisPos();
				}
				++oSegmentIterator;
			}
		}
	}
	*/

	if( oScreen == NULL )
	{		
		QMapIterator<int, GScreen *> oIterator(*m_oScreens);
		if( oIterator.hasNext() )
		{
			m_oCurrentScreen = (GScreen *) oIterator.next().value();
		}
	}
	else if( !IsCurrentScreen(oScreen) )
	{
		m_oCurrentScreen = oScreen;
	}
}

GColorPalette* GDisplay::GetColorPalette()
{
	return m_oColorPalette;
}

void GDisplay::SetColorPalette(GColorPalette *oColorPalette) 
{
	m_oColorPalette = oColorPalette;
}
