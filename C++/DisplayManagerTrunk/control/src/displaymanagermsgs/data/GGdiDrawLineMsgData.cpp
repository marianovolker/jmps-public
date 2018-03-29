#include "GGdiDrawLineMsgData.h"

#include "utils/logger/src/GLogger.h"


GGdiDrawLineMsgData::GGdiDrawLineMsgData()
{
	LOG_LEVEL4("GGdiDrawLineMsgData()");
}

GGdiDrawLineMsgData::~GGdiDrawLineMsgData()
{
	LOG_LEVEL4("~GGdiDrawLineMsgData()");
}

QString GGdiDrawLineMsgData::GetThickness()
{
	LOG_LEVEL4("GetThickness()");

	return m_strThickness;
}

void GGdiDrawLineMsgData::SetThickness(QString strThickness)
{
	LOG_LEVEL4("SetThickness()");

	m_strThickness = strThickness;
}

QString GGdiDrawLineMsgData::GetColor()
{
	LOG_LEVEL4("GetColor()");

	return m_strColor;
}

void GGdiDrawLineMsgData::SetColor(QString strColor)
{
	LOG_LEVEL4("SetColor()");

	m_strColor = strColor;
}

int GGdiDrawLineMsgData::GetLength()
{
	LOG_LEVEL4("GetLength()");

	return m_iLength;
}

void GGdiDrawLineMsgData::SetLength(int iLength)
{
	LOG_LEVEL4("SetLength()");

	m_iLength = iLength;
}

int GGdiDrawLineMsgData::GetXLoc()
{
	LOG_LEVEL4("GetXLoc()");

	return m_iXLoc;
}

void GGdiDrawLineMsgData::SetXLoc(int iXLoc)
{
	LOG_LEVEL4("SetXLoc()");

	m_iXLoc = iXLoc;
}

int GGdiDrawLineMsgData::GetYLoc()
{
	LOG_LEVEL4("GetYLoc()");

	return m_iYLoc;
}

void GGdiDrawLineMsgData::SetYLoc(int iYLoc)
{
	LOG_LEVEL4("SetYLoc()");

	m_iYLoc = iYLoc;
}
