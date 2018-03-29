#include "GFlyTextMsgData.h"

#include "utils/logger/src/GLogger.h"


GFlyTextMsgData::GFlyTextMsgData()
{
	LOG_LEVEL4("GFlyTextMsgData()");
}

GFlyTextMsgData::~GFlyTextMsgData()
{
	LOG_LEVEL4("~GFlyTextMsgData()");
}

QString GFlyTextMsgData::GetFont()
{
	LOG_LEVEL4("GetFont()");

	return  m_strFont;
}


void GFlyTextMsgData::SetFont(QString strFont)
{
	LOG_LEVEL4("SetFont()");

	m_strFont = strFont;
}

QString GFlyTextMsgData::GetText()
{
	LOG_LEVEL4("GetText()");

	return  m_strText;
}

void GFlyTextMsgData::SetText(QString strText)
{
	LOG_LEVEL4("SetText()");

	m_strText = strText;
}

int GFlyTextMsgData::GetXLoc()
{
	LOG_LEVEL4("GetXLoc()");

	return m_iXLoc;
}

void GFlyTextMsgData::SetXLoc(int iXLoc)
{
	LOG_LEVEL4("SetXLoc()");

	m_iXLoc = iXLoc;
}

int GFlyTextMsgData::GetYLoc()
{
	LOG_LEVEL4("GetYLoc()");

	return m_iYLoc;
}

void GFlyTextMsgData::SetYLoc(int iYLoc)
{
	LOG_LEVEL4("SetYLoc()");

	m_iYLoc = iYLoc;
}
