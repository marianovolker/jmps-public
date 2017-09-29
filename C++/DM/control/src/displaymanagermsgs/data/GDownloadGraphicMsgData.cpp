#include "GDownloadGraphicMsgData.h"

#include "utils/logger/src/GLogger.h"

const QString GDownloadGraphicMsgData::st_CRequestMsgImageType						("image");

GDownloadGraphicMsgData::GDownloadGraphicMsgData()
{
	LOG_LEVEL4("GDownloadGraphicMsgData()");
}

GDownloadGraphicMsgData::~GDownloadGraphicMsgData()
{
	LOG_LEVEL4("~GDownloadGraphicMsgData()");
}

QString GDownloadGraphicMsgData::GetScreen()
{
	LOG_LEVEL4("GetScreen()");

	return m_strScreen;
}

void GDownloadGraphicMsgData::SetScreen(QString strScreen)
{
	LOG_LEVEL4("SetScreen()");

	m_strScreen = strScreen;
}

QString GDownloadGraphicMsgData::GetType()
{
	LOG_LEVEL4("GetType()");

	return m_strType;
}

void GDownloadGraphicMsgData::SetType(QString strType)
{
	LOG_LEVEL4("SetType()");

	m_strType = strType;
}

QString GDownloadGraphicMsgData::GetCRC()
{
	LOG_LEVEL4("GetCRC()");

	return  m_strCRC;
}

void GDownloadGraphicMsgData::SetCRC(QString strCRC)
{
	LOG_LEVEL4("SetCRC()");

	m_strCRC = strCRC;
}

QString  GDownloadGraphicMsgData::GetDisplay()
{
	LOG_LEVEL4("GetDisplay()");

	return m_strDisplay;
}

void GDownloadGraphicMsgData::SetDisplay(QString strDisplay)
{
	LOG_LEVEL4("SetDisplay()");

	m_strDisplay = strDisplay;
}

QString GDownloadGraphicMsgData::GetFilename()
{
	LOG_LEVEL4("GetFilename()");

	return m_strFilename;
}

void GDownloadGraphicMsgData::SetFilename(QString strFilename)
{
	LOG_LEVEL4("SetFilename()");

	m_strFilename = strFilename;
}

QString GDownloadGraphicMsgData::GetSegment()
{
	LOG_LEVEL4("GetSegment()");

	return m_strSegment;
}

void GDownloadGraphicMsgData::SetSegment(QString strSegment)
{
	LOG_LEVEL4("SetSegment()");

	m_strSegment = strSegment;
}
