#include "GLogConfig.h"

#include <QFileInfo>

GLogConfig::GLogConfig(GLogConfig::eWriteTo eWriteTo, GLogConfig::eLevel eLvl, QString pszFileName)
	: 	m_eLevel(eLvl)

{
	m_strDirectory = QFileInfo(pszFileName).path();
	m_strFileName  = QFileInfo(pszFileName).fileName();
	
	switch(eWriteTo)
	{
		case GLogConfig::eConsole:
		{
			m_bWriteToConsole = true;			
			m_bWriteToFile = false; 
			break;
		}
		case GLogConfig::eFile:
		{
			m_bWriteToFile = true; 
			m_bWriteToConsole = false;
			break;
		}
		case GLogConfig::eBoth:
		{
			m_bWriteToConsole = m_bWriteToFile = true; 	
			break;
		}	
	}		
}

GLogConfig::~GLogConfig()
{
}

bool GLogConfig::GetWriteToConsole()
{
	return m_bWriteToConsole;
}

bool  GLogConfig::GetWriteToFile()
{
	return m_bWriteToFile;
}

GLogConfig::eLevel GLogConfig::GetLevel()
{
	return m_eLevel;
}

QString GLogConfig::GetDirectory()
{
	return m_strDirectory;
}

QString GLogConfig::GetFileName()
{
	return m_strFileName;
}

QString GLogConfig::GetFullFileName()
{
	return m_strDirectory + "/" + m_strFileName;	
}
