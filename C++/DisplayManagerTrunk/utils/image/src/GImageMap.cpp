#include "GImageMap.h"
#include "utils/logger/src/GLogger.h"


const QString GImageMap::CXMLTag		("ImageMap");
const QString GImageMap::CXMLName		("name");
const QString GImageMap::CXMLSegment	("segment");
const QString GImageMap::CXMLFileName	("fileName");
const QString GImageMap::CXMLFileFormat	("fileFormat");


GImageMap::GImageMap(const QString &strName,const QString &strFileName, int iSegment):
	m_iSegment(iSegment),
	m_strName(strName),
	m_strFileName(strFileName),
	m_strFileFormat("")
{
	LOG_LEVEL4("GImageMap()");
}

GImageMap::GImageMap(const QString &strName,const QString &strFileName, int iSegment, const QString &strFileFormat):
	m_iSegment(iSegment),
	m_strName(strName),
	m_strFileName(strFileName),
	m_strFileFormat(strFileFormat)
{
	LOG_LEVEL4("GImageMap()");
}

int GImageMap::GetSegment() const
{
	LOG_LEVEL4("GetSegment()");
	return m_iSegment;
}

const QString &GImageMap::GetName() const
{
	LOG_LEVEL4("GetName()");
	return m_strName;
}

const QString &GImageMap::GetFileName() const
{
	LOG_LEVEL4("GetFileName()");
	return m_strFileName;
}

void GImageMap::SetFileName(const QString &strName)
{
	LOG_LEVEL4("SetFileName()");
	m_strFileName = strName;
}	

const QString &GImageMap::GetFileFormat() const
{
	LOG_LEVEL4("GetFileFormat()");
	return m_strFileFormat;
}

void GImageMap::SetFileFormat(const QString &strFormat)
{
	LOG_LEVEL4("SetFileFormat()");
	m_strFileFormat = strFormat;
}


