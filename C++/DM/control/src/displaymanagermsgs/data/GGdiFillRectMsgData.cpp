#include "GGdiFillRectMsgData.h"

#include "utils/logger/src/GLogger.h"


GGdiFillRectMsgData::GGdiFillRectMsgData()
{
	LOG_LEVEL4("GGdiFillRectMsgData()");
}

GGdiFillRectMsgData::~GGdiFillRectMsgData()
{
	LOG_LEVEL4("~GGdiFillRectMsgData()");
}

int GGdiFillRectMsgData::GetXLoc()
{
	LOG_LEVEL4("GetXLoc()");

	return m_iXLoc;
}

void GGdiFillRectMsgData::SetXLoc(int iXLoc)
{
	LOG_LEVEL4("SetXLoc()");

	m_iXLoc = iXLoc;
}

int GGdiFillRectMsgData::GetYLoc()
{
	LOG_LEVEL4("GetYLoc()");

	return m_iYLoc;
}

void GGdiFillRectMsgData::SetYLoc(int iYLoc)
{
	LOG_LEVEL4("SetYLoc()");

	m_iYLoc = iYLoc;
}

QString GGdiFillRectMsgData::GetColor()
{
	LOG_LEVEL4("GetColor()");

	return m_strColor;
}

void GGdiFillRectMsgData::SetColor(QString strColor)
{
	LOG_LEVEL4("SetColor()");

	m_strColor = strColor;
}

int GGdiFillRectMsgData::GetHeight()
{
	LOG_LEVEL4("GetHeight()");

	return m_iHeight;
}

void GGdiFillRectMsgData::SetHeight(int iHeight)
{
	LOG_LEVEL4("SetHeight()");

	m_iHeight = iHeight;
}

int GGdiFillRectMsgData::GetWidth()
{
	LOG_LEVEL4("GetWidth()");

	return m_iWidth;
}

void GGdiFillRectMsgData::SetWidth(int iWidth)
{
	LOG_LEVEL4("SetWidth()");

	m_iWidth = iWidth;
}
