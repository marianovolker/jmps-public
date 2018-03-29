/**
 * © 2013 Gilbarco Inc.
 * Confidential and Proprietary
 *
 * @file GLoggerConfig.cpp
 * @author Guillermo Paris
 * @date April 08 2013
 * @copyright © 2013 Gilbarco Inc. Confidential and Proprietary
 */

#include <string.h>
#include "Common/Util/GIniFileHandler.h"
#include "Common/Util/DEBUGOUT.h"

#include <GSystem.h>

const char   GLoggerConfig::mst_cPathSeparator     = PATH_SEPARATOR;
const char*  GLoggerConfig::mst_pcLogPathName      = LOG_DEFAULT_PATHNAME;
const char*  GLoggerConfig::mst_pcLogFileName      = LOG_DEFAULT_FILENAME;
const char*  GLoggerConfig::mst_pcIniConfigName    = INI_CONFIG_DEFAULT_FILE;

const char*  GLoggerConfig::mst_pcGenSectionName   = GENERAL_SECTION_NAME;
const char*  GLoggerConfig::mst_pcModSectionName   = MODULE_SECTION_NAME;
const char*  GLoggerConfig::mst_pcConsLevelKeyName = CONSOLELEVEL_KEY_NAME;
const char*  GLoggerConfig::mst_pcFileLevelKeyName = FILELEVEL_KEY_NAME;
const char*  GLoggerConfig::mst_pcPathKeyName      = PATH_KEY_NAME;
const char*  GLoggerConfig::mst_pcFileKeyName      = FILE_KEY_NAME;
const UINT8  GLoggerConfig::mst_u8DefaultConsLevel = eFatal;
const UINT8  GLoggerConfig::mst_u8DefaultFileLevel = eError;

// log level labels
const char*  GLoggerConfig::mst_arypcLevelLabels[eAllLevels] =
{
	LVL_DEBUG, LVL_TRACE, LVL_NOTICE, LVL_INFO, LVL_WARNING, LVL_ERROR, LVL_FATAL, LVL_SECURITY
};


GLoggerConfig::GLoggerConfig(const char* pcIniFilename)
	: m_u8ConsoleLevel(eFatal), m_u8FileLevel(eError), m_pLogFile(NULL)
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

	// Hard-coded primary default settings
	m_aryModuleInfo[ePrinter].pcKeyName = KEY_PRINTER;
	m_aryModuleInfo[ePrinter].pcShortName = LOG_DEV_PRINTER;
	m_aryModuleInfo[ePrinter].u8Level = eError;
	m_aryModuleInfo[eTrind].pcKeyName = KEY_TRIND;
	m_aryModuleInfo[eTrind].pcShortName = LOG_DEV_TRIND;
	m_aryModuleInfo[eTrind].u8Level = eError;
	m_aryModuleInfo[eBarcode].pcKeyName = KEY_BARCODE;
	m_aryModuleInfo[eBarcode].pcShortName = LOG_DEV_BARCODE;
	m_aryModuleInfo[eBarcode].u8Level = eError;
	m_aryModuleInfo[eCardReader].pcKeyName = KEY_CARDREADER;
	m_aryModuleInfo[eCardReader].pcShortName = LOG_DEV_CARDREADER;
	m_aryModuleInfo[eCardReader].u8Level = eError;
	m_aryModuleInfo[eContactlessReader].pcKeyName = KEY_CONTACTLESSREADER;
	m_aryModuleInfo[eContactlessReader].pcShortName = LOG_DEV_CLESSREADER;
	m_aryModuleInfo[eContactlessReader].u8Level = eError;
	m_aryModuleInfo[eKeypad].pcKeyName = KEY_KEYPAD;
	m_aryModuleInfo[eKeypad].pcShortName = LOG_DEV_KEYPAD;
	m_aryModuleInfo[eKeypad].u8Level = eError;
	m_aryModuleInfo[eCashAcceptor].pcKeyName = KEY_NOTEACCEPTOR;
	m_aryModuleInfo[eCashAcceptor].pcShortName = LOG_DEV_NOTEACCEPTOR;
	m_aryModuleInfo[eCashAcceptor].u8Level = eError;
	m_aryModuleInfo[eDisplay].pcKeyName = KEY_DISPLAY;
	m_aryModuleInfo[eDisplay].pcShortName = LOG_DEV_DISPLAY;
	m_aryModuleInfo[eDisplay].u8Level = eError;
	m_aryModuleInfo[ePOSInterface].pcKeyName = KEY_POSINTERFACE;
	m_aryModuleInfo[ePOSInterface].pcShortName = LOG_DEV_POSINTERFACE;
	m_aryModuleInfo[ePOSInterface].u8Level = eError;
	m_aryModuleInfo[eSmartMerch].pcKeyName = KEY_SMARTMERCH;
	m_aryModuleInfo[eSmartMerch].pcShortName = LOG_DEV_SMARTMERCH;
	m_aryModuleInfo[eSmartMerch].u8Level = eError;
	m_aryModuleInfo[eTimer].pcKeyName = KEY_TIMER;
	m_aryModuleInfo[eTimer].pcShortName = LOG_DEV_TIMER;
	m_aryModuleInfo[eTimer].u8Level = eError;
	m_aryModuleInfo[eBios].pcKeyName = KEY_BIOS;
	m_aryModuleInfo[eBios].pcShortName = LOG_DEV_BIOS;
	m_aryModuleInfo[eBios].u8Level = eError;
	m_aryModuleInfo[eSpot].pcKeyName = KEY_SPOT;
	m_aryModuleInfo[eSpot].pcShortName = LOG_DEV_SPOT;
	m_aryModuleInfo[eSpot].u8Level = eError;
	m_aryModuleInfo[eSpotProtocol].pcKeyName = KEY_SPOTPROTOCOL;
	m_aryModuleInfo[eSpotProtocol].pcShortName = LOG_DEV_SPOTPROTOCOL;
	m_aryModuleInfo[eSpotProtocol].u8Level = eError;
	m_aryModuleInfo[ePump].pcKeyName = KEY_PUMP;
	m_aryModuleInfo[ePump].pcShortName = LOG_DEV_PUMP;
	m_aryModuleInfo[ePump].u8Level = eError;
	m_aryModuleInfo[eCrindToCrind].pcKeyName = KEY_CRINDTOCRIND;
	m_aryModuleInfo[eCrindToCrind].pcShortName = LOG_DEV_CRINDTOCRIND;
	m_aryModuleInfo[eCrindToCrind].u8Level = eError;
	m_aryModuleInfo[ePrinter].pcKeyName = KEY_PRINTER;
	m_aryModuleInfo[ePrinter].pcShortName = LOG_DEV_PRINTER;
	m_aryModuleInfo[ePrinter].u8Level = eError;
	m_aryModuleInfo[eCommon].pcKeyName = KEY_COMMON;
	m_aryModuleInfo[eCommon].pcShortName = LOG_DEV_COMMON;
	m_aryModuleInfo[eCommon].u8Level = eError;
	m_aryModuleInfo[eMenuManager].pcKeyName = KEY_MENUMANAGER;
	m_aryModuleInfo[eMenuManager].pcShortName = LOG_DEV_MENUMANAGER;
	m_aryModuleInfo[eMenuManager].u8Level = eError;
	m_aryModuleInfo[eWinTemplate].pcKeyName = KEY_WIN_TEMPLATE;
	m_aryModuleInfo[eWinTemplate].pcShortName = LOG_DEV_WIN_TEMPLATE;
	m_aryModuleInfo[eWinTemplate].u8Level = eError;
	m_aryModuleInfo[eReport].pcKeyName = KEY_REPORT;
	m_aryModuleInfo[eReport].pcShortName = LOG_DEV_REPORT;
	m_aryModuleInfo[eReport].u8Level = eError;
	m_aryModuleInfo[eConfig].pcKeyName = KEY_CONFIG;
	m_aryModuleInfo[eConfig].pcShortName = LOG_DEV_CONFIG;
	m_aryModuleInfo[eConfig].u8Level = eError;
	m_aryModuleInfo[ePassThru].pcKeyName = KEY_PASSTHRU;
	m_aryModuleInfo[ePassThru].pcShortName = LOG_DEV_PASSTHRU;
	m_aryModuleInfo[ePassThru].u8Level = eError;
	m_aryModuleInfo[eResourcesService].pcKeyName = KEY_RESOURCES;
	m_aryModuleInfo[eResourcesService].pcShortName = LOG_DEV_RESOURCES;
	m_aryModuleInfo[eResourcesService].u8Level = eError;
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
		GSystem::Perror( "ERROR: Initialization file [%s] not found. Using defaults.\n", m_pcIniFilename );

		m_strLogFolder   = mst_pcLogPathName;
		m_strLogFilename = mst_pcLogFileName;
		OpenLogFile();

		return !m_blLogError;
	}

	m_blIniError = false;

	bool blEmbedded(IS_EMBEDDED_APP);

	if(blEmbedded)
	{
		SetLogFilename();
		m_strLogFolder = LOG_DEFAULT_PATHNAME;
	}
	else // standalone external (not embedded) applications.
	{
		oIni.GetPrivateProfileString( mst_pcGenSectionName, mst_pcPathKeyName, mst_pcLogPathName, arycBuffer, sizeof(arycBuffer) );
		m_strLogFolder = arycBuffer;
		if( m_strLogFolder[m_strLogFolder.length()-1] != mst_cPathSeparator )
			m_strLogFolder += mst_cPathSeparator;

		oIni.GetPrivateProfileString( mst_pcGenSectionName, mst_pcFileKeyName, mst_pcLogFileName, arycBuffer, sizeof(arycBuffer) );
		m_strLogFilename = arycBuffer;
	}

	int i = oIni.GetPrivateProfileInt(mst_pcGenSectionName, mst_pcConsLevelKeyName, eFatal);
	(i < eLowestLogLevel || i > eHighestLogLevel)?(i = eFatal):(m_u8ConsoleLevel = UINT8(i));

	i = oIni.GetPrivateProfileInt(mst_pcGenSectionName, mst_pcFileLevelKeyName, eError);
	(i < eLowestLogLevel || i > eHighestLogLevel)?(i = eError):(m_u8FileLevel = UINT8(i));

	for (int idx = eFirstDevice; idx <= eLastDevice; idx++)
	{
		const char* pcKey = m_aryModuleInfo[idx].pcKeyName;
		i = oIni.GetPrivateProfileInt(mst_pcModSectionName, pcKey, m_u8FileLevel); //default is file level value

		if (i < eLowestLogLevel || i > eHighestLogLevel)
		{
			i = m_u8FileLevel;
		}

		if (i != eError)
		{
			m_aryModuleInfo[idx].u8Level = UINT8(i);
		}
	}

	return OpenLogFile();
}

void GLoggerConfig::SetLogFilename()
{
	char arycFileDate[LOGFILE_DATE_SIZE];

	arycFileDate[0] = 0;
	GLogger::String_timestamp(arycFileDate, LOGFILE_DATE_SIZE ,
	                          GLogger::eOnlyNumbersDateYYYYMMDD);

	// Store the filename date for later comparison
	strncpy(m_arycLogFileDate, arycFileDate, LOGFILE_DATE_SIZE);

	// Set the filename using DIAGNOSTIC.YYYYMMDD pattern
	m_strLogFilename = LOG_DEFAULT_FILENAME;
	m_strLogFilename.replace(1 + DIAGNOSTIC_WORD_LENGTH, 8, arycFileDate, 8);
}

bool GLoggerConfig::OpenLogFile()
{
	std::string sFilename = (m_strLogFolder + m_strLogFilename);
	m_pLogFile = new GFile(sFilename);

	if(m_pLogFile == NULL)
	{
		GSystem::Perror("ERROR: Couldn't open file [%s] . Log file WILL NOT BE AVAILABLE.\n", sFilename.c_str());

		m_blLogError = true;
		return false;
	}

	m_blLogError = ! m_pLogFile->Open(eOpenAppend);
	return ! m_blLogError;
}

bool GLoggerConfig::SwitchLogFile()
{
	bool  blEmbedded(IS_EMBEDDED_APP);

	if( ! blEmbedded )
		return false; // this is a public method only working in embedded application.

	delete m_pLogFile;
	m_pLogFile = NULL;
	SetLogFilename();
	return OpenLogFile();
}

bool GLoggerConfig::HasDateChanged()
{
	int   iret = 0;
	char  arycCurrentDate[LOGFILE_DATE_SIZE];

	GLogger::String_timestamp(arycCurrentDate, LOGFILE_DATE_SIZE,
	                          GLogger::eOnlyNumbersDateYYYYMMDD);

	iret = strncmp(arycCurrentDate, m_arycLogFileDate, 8);

	return iret == 0 ? false : true;
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
	int i = eHighestLogLevel;
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
	if(eModule < eFirstDevice || eModule > eLastDevice)
	{
		return m_u8FileLevel; // generic log level
	}
	else
	{
		if( m_u8FileLevel >= m_aryModuleInfo[eModule].u8Level )
		{
			return m_u8FileLevel;
		}
		else
		{
			return m_aryModuleInfo[eModule].u8Level;
		}
	}
}

const char* GLoggerConfig::GetModuleShortName(eSubSystem eModule)
{
	if(eModule < eFirstDevice || eModule > eLastDevice)
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

	int i = eFirstDevice;
	for( ;i <= eLastDevice; i++ )
	{
		if( ! strcmp(pcModuleKey, m_aryModuleInfo[i].pcKeyName) )
		{
			return static_cast<eSubSystem>( i );
		}
	}

	return eUnCategorized;
}


