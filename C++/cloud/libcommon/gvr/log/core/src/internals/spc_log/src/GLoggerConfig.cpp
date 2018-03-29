/**
 * © 2013 Gilbarco Inc.
 * Confidential and Proprietary
 *
 * @file GLoggerConfig.cpp
 * @author Guillermo Paris
 * @date April 08 2013
 * @copyright © 2013 2014 Gilbarco Inc. Confidential and Proprietary
 */

#include <string.h>
#include <GSystem.h>
#include "GIniFileHandler.h"
#include "GLoggerConfig.h"
#include "GLogger.h"

using namespace gvr::log;

const char*  GLoggerConfig::mst_pcLogPathName      = LOG_DEFAULT_PATHNAME;
const char*  GLoggerConfig::mst_pcLogFileName      = LOG_DEFAULT_FILENAME;
const char*  GLoggerConfig::mst_pcIniConfigName    = INI_LOGGER_DEFAULT_CONFIG_FILE;

const char*  GLoggerConfig::mst_pcGenSectionName   = GENERAL_SECTION_NAME;
const char*  GLoggerConfig::mst_pcModSectionName   = MODULE_SECTION_NAME;
const char*  GLoggerConfig::mst_pcConsLevelKeyName = CONSOLELEVEL_KEY_NAME;
const char*  GLoggerConfig::mst_pcFileLevelKeyName = FILELEVEL_KEY_NAME;
const char*  GLoggerConfig::mst_pcPathKeyName      = PATH_KEY_NAME;
const char*  GLoggerConfig::mst_pcFileKeyName      = FILE_KEY_NAME;


// log level labels
const char*  GLoggerConfig::mst_arypcLevelLabels[eAllLevels] =
{
	LVL_DEBUG, LVL_TRACE, LVL_NOTICE, LVL_INFO, LVL_WARNING, LVL_ERROR, LVL_FATAL
};


GLoggerConfig::GLoggerConfig(const char* pcIniFilename)
	: m_u8ConsoleLevel(eFatal), m_u8FileLevel(eError),
	  m_iRunningMode(0), m_logFileSize(0), m_pLogFile(NULL)
{
	// This forces the user to call Initialize()
	m_blIniError = true;
	m_blLogError = true;

	if(pcIniFilename == NULL)
	{
		pcIniFilename = mst_pcIniConfigName; // default .ini name if not supplied
	}

	m_pcIniFilename = pcIniFilename;

	memset(m_arycLogFileDate, 0, sizeof(m_arycLogFileDate));
	memset(m_aryModuleInfo, 0, sizeof(m_aryModuleInfo));

	// Hard-coded primary default settings
	m_aryModuleInfo[ePrinter].pcKeyName = KEY_PRINTER;
	m_aryModuleInfo[ePrinter].pcShortName = LOG_DEV_PRINTER;
	m_aryModuleInfo[ePrinter].u8Level = eError;
	m_aryModuleInfo[eCardReader].pcKeyName = KEY_CARDREADER;
	m_aryModuleInfo[eCardReader].pcShortName = LOG_DEV_CARDREADER;
	m_aryModuleInfo[eCardReader].u8Level = eError;
	m_aryModuleInfo[eKeypad].pcKeyName = KEY_KEYPAD;
	m_aryModuleInfo[eKeypad].pcShortName = LOG_DEV_KEYPAD;
	m_aryModuleInfo[eKeypad].u8Level = eError;
	m_aryModuleInfo[eDisplay].pcKeyName = KEY_DISPLAY;
	m_aryModuleInfo[eDisplay].pcShortName = LOG_DEV_DISPLAY;
	m_aryModuleInfo[eDisplay].u8Level = eError;
	m_aryModuleInfo[eSpot].pcKeyName = KEY_SPOT;
	m_aryModuleInfo[eSpot].pcShortName = LOG_DEV_SPOT;
	m_aryModuleInfo[eSpot].u8Level = eError;
	m_aryModuleInfo[eSpotProtocol].pcKeyName = KEY_SPOTPROTOCOL;
	m_aryModuleInfo[eSpotProtocol].pcShortName = LOG_DEV_SPOTPROTOCOL;
	m_aryModuleInfo[eSpotProtocol].u8Level = eError;
	m_aryModuleInfo[eCommon].pcKeyName = KEY_COMMON;
	m_aryModuleInfo[eCommon].pcShortName = LOG_DEV_COMMON;
	m_aryModuleInfo[eCommon].u8Level = eError;
	m_aryModuleInfo[eConfig].pcKeyName = KEY_CONFIG;
	m_aryModuleInfo[eConfig].pcShortName = LOG_DEV_CONFIG;
	m_aryModuleInfo[eConfig].u8Level = eError;
	m_aryModuleInfo[eUnCategorized].pcKeyName = KEY_UNCATEGORIZED;
	m_aryModuleInfo[eUnCategorized].pcShortName = LOG_DEV_UNCATEGORIZED;
	m_aryModuleInfo[eUnCategorized].u8Level = eError;
}

GLoggerConfig::~GLoggerConfig()
{
	if(m_pLogFile)
	{
		delete m_pLogFile;
		m_pLogFile = NULL;
	}
}

bool GLoggerConfig::Initialize()
{
	GIniFileHandler oIni(m_pcIniFilename);
	char arycBuffer[192];

	if( ! oIni.IsReadable() )
	{
		// If it can't read the .ini then applies defaults.
		GSystem::Perror( "WARNING: Initialization file [%s] not found. Using defaults.\n", m_pcIniFilename );

		m_strLogFolder   = mst_pcLogPathName;
		m_strLogFilename = mst_pcLogFileName;
		OpenLogFile();
		return !m_blLogError;
	}

	m_blIniError = false;

	oIni.GetPrivateProfileString( mst_pcGenSectionName, mst_pcPathKeyName, mst_pcLogPathName, arycBuffer, sizeof(arycBuffer) );
	m_strLogFolder = arycBuffer;

	oIni.GetPrivateProfileString( mst_pcGenSectionName, mst_pcFileKeyName, mst_pcLogFileName, arycBuffer, sizeof(arycBuffer) );
	m_strLogFilename = arycBuffer;

	int i = oIni.GetPrivateProfileInt(mst_pcGenSectionName, mst_pcConsLevelKeyName, eFatal);
	if( i >= eLowestLogLevel && i <= eHighestLogLevel )
		m_u8ConsoleLevel = UINT8(i);

	i = oIni.GetPrivateProfileInt(mst_pcGenSectionName, mst_pcFileLevelKeyName, eError);
	if( i >= eLowestLogLevel && i <= eHighestLogLevel)
		m_u8FileLevel = UINT8(i);

	for (int idx = eFirstModule; idx <= eLastModule; idx++)
	{
		const char* pcKey = m_aryModuleInfo[idx].pcKeyName;

		i = oIni.GetPrivateProfileInt(mst_pcModSectionName, pcKey, m_u8FileLevel); //default is file level value
		if (i < eLowestLogLevel || i > eHighestLogLevel)
		{
			i = m_u8FileLevel; //default is file level value
		}

		if (i != eError) // default error level is already set.
		{
			m_aryModuleInfo[idx].u8Level = UINT8(i);
		}
	}

	return OpenLogFile();
}

void GLoggerConfig::SetLogFilename()
{
	char arycFileDate[mst_CiLogfilenameDateMaxSize];
	arycFileDate[0] = 0;
	GLogger::String_timestamp(arycFileDate, mst_CiLogfilenameDateMaxSize , GLogger::eOnlyNumbersDateYYYYMMDD);

	// Store the filename date for later comparison
	strncpy(m_arycLogFileDate, arycFileDate, mst_CiLogfilenameDateMaxSize);
	if(HasDateChanged())
	{
		m_strLogFilename = mst_pcLogFileName;
		m_strLogFilename.replace(1 + mst_CiDiagnosticAcronymLength, mst_CiOnlyDateFmtMaxSize, arycFileDate, mst_CiOnlyDateFmtMaxSize);
	}
	else
	{
		unsigned long size=0;
		char fileIndex[3]={'0','0','0'};

		m_strLogFilename = mst_pcLogFileName;
		m_strLogFilename.replace(1 + mst_CiDiagnosticAcronymLength, mst_CiOnlyDateFmtMaxSize, arycFileDate, mst_CiOnlyDateFmtMaxSize);

		int iCounter=0;
		do
		{
			std::string sFilename = (m_strLogFolder + m_strLogFilename);
			m_pLogFile = new GFile(sFilename);
			m_blLogError =  m_pLogFile->Open(eOpenReadOnly);
			if(!m_blLogError)
			{
				break;
			}
			else
			{
				iCounter++;
				memset(fileIndex,0x00,sizeof(fileIndex));
				snprintf(fileIndex, sizeof(fileIndex), "%03d",iCounter);
				m_strLogFilename.replace(20,3,fileIndex);
			}

		} while(true);  // end of do-while.

		if (iCounter == 0)
		{
			return;
		}

		iCounter--;
		memset(fileIndex,0x00,sizeof(fileIndex));
		snprintf(fileIndex, sizeof(fileIndex), "%03d",iCounter);
		m_strLogFilename.replace(20,3,fileIndex);
		CalcLogFileSize();
		size=GetLogFileSize();

		if(size >= mst_CiLogfilenameDateMaxSize)
		{
			iCounter++;
			snprintf(fileIndex, sizeof(fileIndex), "%03d",iCounter);
			m_strLogFilename.replace(20,3,fileIndex);
		}
	}
}

bool GLoggerConfig::OpenLogFile()
{
	if( m_strLogFolder.at(m_strLogFolder.length() - 1) != mst_cPathSeparator &&
	    m_strLogFilename.at(0) != mst_cPathSeparator )
	{
		m_strLogFolder.append(1, mst_cPathSeparator);
	}

	std::string sFilename = (m_strLogFolder + m_strLogFilename);
	m_pLogFile = new GFile(sFilename);

	if(m_pLogFile == NULL)
	{
		GSystem::Perror( "ERROR: Couldn't open file [%s] . Log file WILL NOT BE AVAILABLE.\n", sFilename.c_str() );
		m_blLogError = true;
		return false;
	}

	m_blLogError = ! m_pLogFile->Open(eOpenAppend);
	if(m_blLogError)
	{
		GSystem::Perror( "ERROR: Couldn't open file [%s] . Log file WILL NOT BE AVAILABLE.\n", sFilename.c_str() );
	}

	return ! m_blLogError;
}

bool GLoggerConfig::SwitchLogFile()
{
	delete m_pLogFile;
	m_pLogFile = NULL;
	SetLogFilename();
	return OpenLogFile();
}

bool GLoggerConfig::HasDateChanged()
{
	int   iret = 0;
	char  arycCurrentDate[mst_CiLogfilenameDateMaxSize];

	GLogger::String_timestamp(arycCurrentDate, mst_CiLogfilenameDateMaxSize,
	                          GLogger::eOnlyNumbersDateYYYYMMDD);

	iret = strncmp(arycCurrentDate, m_arycLogFileDate, mst_CiOnlyDateFmtMaxSize);

	return iret == 0 ? false : true;
}


void GLoggerConfig::CalcLogFileSize()
{
	if(!m_pLogFile)
	{
		int fSize;
		std::string sFilename = (m_strLogFolder + m_strLogFilename);

		m_pLogFile = new GFile(sFilename);

		m_blLogError =  m_pLogFile->Open(eOpenReadOnly);
		if(!m_blLogError)
		{
			GSystem::Perror(" ERROR: Couldn't open file [%s] . Log file WILL NOT BE AVAILABLE.\n", sFilename.c_str() );
			m_blLogError = true;
			return;
		}
	}

	m_pLogFile->Size(m_logFileSize);

}

const char* GLoggerConfig::GetLevelLabel(eLogLevel eLevel)
{
	if( eLevel < eLowestLogLevel )
	{
		eLevel = eLowestLogLevel;
	}

	else if( eLevel > eHighestLogLevel )
	{
		eLevel = eHighestLogLevel;
	}

	return mst_arypcLevelLabels[eLevel];
}

eLogLevel GLoggerConfig::GetLevelIndex(const char* pcLevel)
{
	int i = eLowestLogLevel;
	for( ;i <= eHighestLogLevel; i++ )
	{
		if( ! strcasecmp(pcLevel, mst_arypcLevelLabels[i]) )
		{
			return static_cast<eLogLevel>( i );
		}
	}

	return eError;
}

int GLoggerConfig::GetModuleLogLevel(eSubSystem eModule)
{
	if(eModule < eFirstModule || eModule > eLastModule)
	{
		return m_u8FileLevel; // generic log level
	}
	else
	{
		if( m_u8FileLevel >= m_aryModuleInfo[eModule].u8Level )
		{
			return m_u8FileLevel; // the module level shall be at least the general file level
		}                         // in order to produce logging into the log file.
		else
		{
			return m_aryModuleInfo[eModule].u8Level;
		}
	}
}

const char* GLoggerConfig::GetModuleShortName(eSubSystem eModule)
{
	if(eModule < eFirstModule || eModule > eLastModule)
	{
		eModule = eUnCategorized;
	}

	return m_aryModuleInfo[eModule].pcShortName;
}

eSubSystem GLoggerConfig::GetModuleIndex(const char* pcModuleKey)
{
	if( pcModuleKey == NULL )
	{
		return eUnCategorized;
	}

	for( int i = eFirstModule; i <= eLastModule; i++ )
	{
		if( ! strcmp(pcModuleKey, m_aryModuleInfo[i].pcKeyName) )
		{
			return static_cast<eSubSystem>( i );
		}
	}

	return eUnCategorized;
}

bool GLoggerConfig::SetConsoleLevel(int iLevel)
{
	if( iLevel < eLowestLogLevel || iLevel > eHighestLogLevel )
	{
		return false; // wrong level
	}

	m_u8ConsoleLevel = UINT8(iLevel);
	return true;
}

bool GLoggerConfig::SetGeneralLogLevel(int iLevel)
{
	if( iLevel < eLowestLogLevel || iLevel > eHighestLogLevel )
	{
		return false; // wrong level
	}

	m_u8FileLevel = UINT8(iLevel);
	return true;
}

bool GLoggerConfig::SetModuleInfo(int iModule, int iLevel,
                                  const char* pcShortName /* = NULL */,
                                  const char* pcKeyName  /* = NULL */ )
{
	if( iLevel < eLowestLogLevel || iLevel > eHighestLogLevel )
	{
		return false; // wrong level number
	}

	if(iModule < eFirstModule || iModule > eLastModule)
	{
		return false; // wrong module number
	}

	m_aryModuleInfo[iModule].u8Level = UINT8(iLevel);

	if( pcShortName && *pcShortName != '\0' )
	{
		m_aryModuleInfo[iModule].pcShortName = pcShortName;
	}

	if( pcKeyName && *pcKeyName != '\0' )
	{
		m_aryModuleInfo[iModule].pcKeyName = pcKeyName;
	}

	return true;
}

const char* GLoggerConfig::GetLogFilePath()
{
	std::string sFilename = (m_strLogFolder + m_strLogFilename);
	return strdup(sFilename.c_str());
}

