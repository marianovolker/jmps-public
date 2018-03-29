#include "GISAbsData.h"

#include "utils/logger/src/GLogger.h"

inline QSet<unsigned> InitializeValidFonts (void)
{
	LOG_LEVEL4("InitializeValidFonts()");
	QSet<unsigned> qsetValidFont;
	qsetValidFont.insert(0);
	qsetValidFont.insert(1);
	return qsetValidFont;
}

QSet<unsigned> const GISAbsData::_qksetValidFont(InitializeValidFonts());

GISAbsData::GISAbsData()
	:	m_iAttr(0),
		m_iColor(0),
		m_iDisplay(0),
		m_iField(0),
		m_iFont(0),
		m_iScreen(0),
		m_iSegment(0),
		m_iXpos(0),
		m_iYpos(0),
		m_strVLD(0),
		m_bSynchronousRequest(false)
{
	LOG_LEVEL4("GISAbsData()");
}

GISAbsData::~GISAbsData()
{
	LOG_LEVEL4("~GISAbsData()");
}

int GISAbsData::GetAttr()
{
	LOG_LEVEL4("GetAttr()");

	return  m_iAttr;
}

void GISAbsData::SetAttr(int iAttr)
{
	LOG_LEVEL4("SetAttr()");

	m_iAttr = iAttr;
}

int GISAbsData::GetColor()
{
	LOG_LEVEL4("GetColor()");

	return m_iColor;
}

void GISAbsData::SetColor(int iColor)
{
	LOG_LEVEL4("SetColor()");

	m_iColor = iColor;	
}

int GISAbsData::GetDisplay()
{
	LOG_LEVEL4("GetDisplay()");

	return m_iDisplay;
}

void GISAbsData::SetDisplay(int iDisplay)
{
	LOG_LEVEL4("SetDisplay()");

	m_iDisplay = iDisplay;
}

int GISAbsData::GetField()
{
	LOG_LEVEL4("GetField()");

	return m_iField;
}

void GISAbsData::SetField(int iField)
{
	LOG_LEVEL4("SetField()");

	m_iField = iField;
}

int GISAbsData::GetFont()
{
	LOG_LEVEL4("GetFont()");

	return m_iFont;
}

void GISAbsData::SetFont(int iFont)
{
	LOG_LEVEL4("SetFont()");

	m_iFont = iFont;
}

int GISAbsData::GetScreen()
{
	LOG_LEVEL4("GetScreen()");

	return m_iScreen;
}

void GISAbsData::SetScreen(int iScreen)
{
	LOG_LEVEL4("SetScreen()");

	m_iScreen = iScreen;
}

int GISAbsData::GetSegment()
{
	LOG_LEVEL4("GetSegment()");

	return m_iSegment;
}

void GISAbsData::SetSegment(int iSegment)
{
	LOG_LEVEL4("SetSegment()");

	m_iSegment = iSegment;
}

int GISAbsData::GetXpos()
{
	LOG_LEVEL4("GetXpos()");

	return m_iXpos;
}

void GISAbsData::SetXpos(int iXpos)
{
	LOG_LEVEL4("SetXpos()");

	m_iXpos = iXpos;
}

int GISAbsData::GetYpos()
{
	LOG_LEVEL4("GetYpos()");

	return m_iYpos;
}

void GISAbsData::SetYpos(int iYpos)
{
	LOG_LEVEL4("SetYpos()");

	m_iYpos = iYpos;
}

QByteArray GISAbsData::GetVLD()
{
	LOG_LEVEL4("GetVLD()");

	return m_strVLD;
}

void GISAbsData::SetVLD( QByteArray strVLD )
{
	LOG_LEVEL4("SetVLD()");

	m_strVLD = strVLD;
}

bool GISAbsData::IsValidFont(int iFont)
{
	LOG_LEVEL4("IsValidFont()");

	return 	_qksetValidFont.contains(iFont);
}

bool GISAbsData::GetSynchronousRequest()
{
	LOG_LEVEL4("GetSynchronousRequest()");

	return m_bSynchronousRequest;
}

void GISAbsData::SetSynchronousRequest(bool bSynchronousRequest)
{
	LOG_LEVEL4("SetSynchronousRequest()");

	m_bSynchronousRequest = bSynchronousRequest;

	if( m_bSynchronousRequest )
	{
		LOG_LEVEL4("Setting the response as Synchronous on InfoScreen.");
	}
}
