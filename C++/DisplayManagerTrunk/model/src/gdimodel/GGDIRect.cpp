#include "GGDIRect.h"

#include "utils/logger/src/GLogger.h"

GGDIRect::GGDIRect():
	m_strColor(""),
	m_iXLoc(0),
	m_iYLoc(0),
	m_iHeight(0),
	m_iWidth(0),
	m_iRectNumber(0)
{
	LOG_LEVEL4("GGDIRect()");
}

GGDIRect::~GGDIRect()
{
	LOG_LEVEL4("~GGDIRect()");
}

int GGDIRect::GetXLoc()
{
	LOG_LEVEL4("GetXLoc()");

	return m_iXLoc;
}

void GGDIRect::SetXLoc(int iXLoc)
{
	LOG_LEVEL4("SetXLoc()");

	m_iXLoc = iXLoc;
}

int GGDIRect::GetYLoc()
{
	LOG_LEVEL4("GetYLoc()");

	return m_iYLoc;
}

void GGDIRect::SetYLoc(int iYLoc)
{
	LOG_LEVEL4("SetYLoc()");

	m_iYLoc = iYLoc;
}

QString GGDIRect::GetColor()
{
	LOG_LEVEL4("GetColor()");

	return m_strColor;
}

void GGDIRect::SetColor(QString strColor)
{
	LOG_LEVEL4("SetColor()");

	m_strColor = strColor;
}

int GGDIRect::GetHeight()
{
	LOG_LEVEL4("GetHeight()");

	return m_iHeight;
}

void GGDIRect::SetHeight(int iHeight)
{
	LOG_LEVEL4("SetHeight()");

	m_iHeight = iHeight;
}

int GGDIRect::GetWidth()
{
	LOG_LEVEL4("GetWidth()");

	return m_iWidth;
}

void GGDIRect::SetWidth(int iWidth)
{
	LOG_LEVEL4("SetWidth()");

	m_iWidth = iWidth;
}

int GGDIRect::GetRectNumber()
{
	LOG_LEVEL4("GetRectNumber()");

	return m_iRectNumber;
}

void GGDIRect::SetRectNumber(int iRectNumber)
{
	LOG_LEVEL4("SetRectNumber()");

	m_iRectNumber = iRectNumber;
}
