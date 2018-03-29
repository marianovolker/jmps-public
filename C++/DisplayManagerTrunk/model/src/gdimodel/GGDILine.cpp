#include "GGDILine.h"

#include "utils/logger/src/GLogger.h"

GGDILine::GGDILine():
	m_strThickness(""),
	m_strColor(""),
	m_iLength(0),
	m_iLineNumber(0),
	m_iXLoc(0),
	m_iYLoc(0)
{
	LOG_LEVEL4("GGDILine()");
}

GGDILine::~GGDILine()
{
	LOG_LEVEL4("~GGDILine()");
}

QString GGDILine::GetThickness()
{
	LOG_LEVEL4("GetThickness()");

	return m_strThickness;
}

void GGDILine::SetThickness(QString strThickness)
{
	LOG_LEVEL4("SetThickness()");

	m_strThickness = strThickness;
}

QString GGDILine::GetColor()
{
	LOG_LEVEL4("GetColor()");

	return m_strColor;
}

void GGDILine::SetColor(QString strColor)
{
	LOG_LEVEL4("SetColor()");

	m_strColor = strColor;
}

int GGDILine::GetLength()
{
	LOG_LEVEL4("GetLength()");

	return m_iLength;
}

void GGDILine::SetLength(int iLength)
{
	LOG_LEVEL4("SetLength()");

	m_iLength = iLength;
}

int GGDILine::GetXLoc()
{
	LOG_LEVEL4("GetXLoc()");

	return m_iXLoc;
}

void GGDILine::SetXLoc(int iXLoc)
{
	LOG_LEVEL4("SetXLoc()");

	m_iXLoc = iXLoc;
}

int GGDILine::GetYLoc()
{
	LOG_LEVEL4("GetYLoc()");

	return m_iYLoc;
}

void GGDILine::SetYLoc(int iYLoc)
{
	LOG_LEVEL4("SetYLoc()");

	m_iYLoc = iYLoc;
}

int GGDILine::GetLineNumber()
{
	LOG_LEVEL4("GetLineNumber()");

	return m_iLineNumber;
}

void GGDILine::SetLineNumber(int iLineNumber)
{
	LOG_LEVEL4("SetLineNumber()");

	m_iLineNumber = iLineNumber;
}
