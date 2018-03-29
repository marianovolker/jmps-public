#include "GGDIText.h"

#include "utils/logger/src/GLogger.h"

GGDIText::GGDIText():
	m_strBGColor(""),
	m_strFGColor(""),
	m_strFont(""),
	m_strAttr(""),
	m_strText(""),
	m_iTextNumber(0),
	m_iXLoc(0),
	m_iYLoc(0)
{
	LOG_LEVEL4("GGDIText()");
}

GGDIText::~GGDIText()
{
	LOG_LEVEL4("~GGDIText()");
}

QString GGDIText::GetBGColor()
{
	LOG_LEVEL4("GetBGColor()");

	return m_strBGColor;
}

void GGDIText::SetBGColor(QString strBGColor)
{
	LOG_LEVEL4("SetBGColor()");

	m_strBGColor = strBGColor;
}

QString GGDIText::GetFGColor()
{
	LOG_LEVEL4("GetFGColor()");

	return m_strFGColor;
}

void GGDIText::SetFGColor(QString strFGColor)
{
	LOG_LEVEL4("SetFGColor()");

	m_strFGColor = strFGColor;
}

QString GGDIText::GetText()
{
	LOG_LEVEL4("GetText()");

	return m_strText;
}

void GGDIText::SetText(QString strText)
{
	LOG_LEVEL4("SetText()");

	m_strText = strText;
}

int GGDIText::GetXLoc()
{
	LOG_LEVEL4("GetXLoc()");

	return m_iXLoc;
}

void GGDIText::SetXLoc(int iXLoc)
{
	LOG_LEVEL4("SetXLoc()");

	m_iXLoc = iXLoc;
}

int GGDIText::GetYLoc()
{
	LOG_LEVEL4("GetYLoc()");

	return m_iYLoc;
}

void GGDIText::SetYLoc(int iYLoc)
{
	LOG_LEVEL4("SetYLoc()");

	m_iYLoc = iYLoc;
}

QString GGDIText::GetFont()
{
	LOG_LEVEL4("GetFont()");

	return  m_strFont;
}

void GGDIText::SetFont(QString strFont)
{
	LOG_LEVEL4("SetFont()");

	m_strFont = strFont;
}

QString GGDIText::GetAttr()
{
	LOG_LEVEL4("GetAttr()");

	return m_strAttr;
}

void GGDIText::SetAttr(QString strAttr)
{
	LOG_LEVEL4("SetAttr()");

	m_strAttr = strAttr;
}

int GGDIText::GetTextNumber()
{
	LOG_LEVEL4("GetTextNumber()");

	return m_iTextNumber;
}

void GGDIText::SetTextNumber(int iTextNumber)
{
	LOG_LEVEL4("SetTextNumber()");

	m_iTextNumber = iTextNumber;
}
