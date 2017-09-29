#include "GSegment.h"

#include "GScreen.h"

const QString GSegment::CXMLTag				("Segment");
const QString GSegment::CXMLNumber			("number");
const QString GSegment::CXMLScreenNumber	("screenNumber");
const QString GSegment::CXMLBGColor			("bGColor");
const QString GSegment::CXMLVisible			("visible");
const QString GSegment::CXMLWidth			("width");
const QString GSegment::CXMLHeight			("height");
const QString GSegment::CXMLXPos			("xPos");
const QString GSegment::CXMLYPos			("yPos");

GSegment::GSegment(int iNumber, GScreen* oScreen)
	: m_iNumber	(iNumber),
	  m_oScreen	(oScreen),
	  m_iBGColor(0),
	  m_iWidth	(0),
	  m_iHeight	(0),
	  m_iXPos	(0),
	  m_iYPos	(0)
{
	m_oFields = new QMap<int, GTextField *>;
}


GSegment::GSegment(const GSegment& oSegment)
		: m_iNumber	(oSegment.GetNumber()),
		m_oScreen	(oSegment.GetScreen()),
		m_iBGColor(oSegment.GetBGColor()),
		m_bVisible (oSegment.IsVisible()),
		m_iWidth	(oSegment.GetWidth()),
		m_iHeight	(oSegment.GetHeight()),
		m_iXPos	(oSegment.GetXPos()),
		m_iYPos	(oSegment.GetYPos())
{

	m_oFields = new QMap<int, GTextField *>;

	QMapIterator<int, GTextField *> oIterator(*(oSegment.GetFieldsMap()));

	while( oIterator.hasNext() )
	{
		oIterator.next();
		GTextField *oTextField = new GTextField(*(oIterator.value()));
		
		AddField(oTextField);
	}

}




GSegment::~GSegment()
{
	QMapIterator<int, GTextField *> oIterator(*m_oFields);

	while( oIterator.hasNext() )
	{ 
		oIterator.next();
		delete oIterator.value();
	}	
	
	delete m_oFields;
	m_oFields = NULL;
}



GSegment *GSegment::Clone(GSegment *poSegment)
{

	GSegment *oNewSegment = new GSegment(*poSegment);

	return oNewSegment;
}


void GSegment::AddField(GTextField* oField)
{
	m_oFields->insert(oField->GetNumber(), oField);
}

GTextField* GSegment::GetField(int iNumber) const
{
	QMap<int, GTextField *>::const_iterator i = m_oFields->find(iNumber);

	return (i != m_oFields->end())? i.value() : NULL;
}

QList<GTextField *> GSegment::GetAllFields() const
{
	return m_oFields->values();
}

void GSegment::RemoveField(int iNumber)
{	
	GTextField* oField = GetField(iNumber);
	
	if( oField != NULL )
	{			
		m_oFields->remove(iNumber);
		delete oField;
	}
}

int GSegment::GetNumber() const
{
	return m_iNumber;
}

GScreen* GSegment::GetScreen() const
{
	return m_oScreen;
}

int GSegment::GetBGColor() const
{
	return m_iBGColor;
}

void GSegment::SetBGColor(int iColor)
{
	m_iBGColor = iColor;
}

bool GSegment::IsVisible() const
{
	return m_bVisible;
}

void GSegment::SetVisible(bool bVisible)
{
	m_bVisible = bVisible;
}

int GSegment::GetWidth() const
{
	return m_iWidth;
}

void GSegment::SetWidth(int iWidth)
{
	m_iWidth = iWidth;
}

int GSegment::GetHeight() const
{
	return m_iHeight;
}

void GSegment::SetHeight(int iHeight)
{
	m_iHeight = iHeight;
}

int GSegment::GetXPos() const
{
	return m_iXPos;
}

void GSegment::SetXPos(int iXPos)
{
	m_iXPos = iXPos;
}

int GSegment::GetYPos() const
{
	return m_iYPos;
}

void GSegment::SetYPos(int iYPos)
{
	m_iYPos = iYPos;
}


QMap<int, GTextField *> *GSegment::GetFieldsMap() const
{
	return m_oFields;
}
