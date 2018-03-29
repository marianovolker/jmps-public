/**
 * © 2013 Gilbarco Inc.
 * Confidential and Proprietary
 *
 * @file GLoggerConfig.h
 * @author Guillermo Paris
 * @date April 08 2013
 * @copyright © 2013 Gilbarco Inc. Confidential and Proprietary
 */

#ifndef GLOGGERCONFIG_H
#define GLOGGERCONFIG_H

#include <string>
#include <GTypes.h>
#include "ConfigLabels.h"

#include <GFile.h>

#define LOGFILE_DATE_SIZE             12

typedef struct
{
	UINT8        u8Level;
	const char*  pcShortName;
	const char*  pcKeyName;

} MODULE_INFO;

class GLoggerConfig
{
	public:
		GLoggerConfig(const char* pcIniFilename = NULL);
		~GLoggerConfig();

		       bool   Initialize();
		       bool   HasDateChanged();
		       bool   SwitchLogFile();
		inline GFile* GetLogStream();
		inline bool   IniNotFound();
		inline bool   LogfileNotAvailable();
		inline int    GetConsoleLevel();
		inline int    GetGeneralLogLevel();
		const char*   GetLevelLabel(eLogLevel e);
		eLogLevel     GetLevelIndex(const char* pcLevel);
		       int    GetModuleLogLevel(eSubSystem e);
		const  char*  GetModuleShortName(eSubSystem e);
		eSubSystem    GetModuleIndex(const char* pcModuleKey);
		inline int    GetRunningMode();
		inline void   SetRunningMode(int runningMode);
		const  char*  GetLogFilePath();
		void          CalcLogFileSize();
		unsigned long GetLogFileSize();

		static const char   mst_cPathSeparator;    // = PATH_SEPARATOR
		static const char*  mst_pcLogPathName;     // = LOG_DEFAULT_PATHNAME
		static const char*  mst_pcLogFileName;     // = LOG_DEFAULT_FILENAME
		static const char*  mst_pcIniConfigName;   // = INI_CONFIG_DEFAULT_FILE



		static const char*  mst_pcGenSectionName;  // = GENERAL_SECTION_NAME
		static const char*  mst_pcModSectionName;  // = MODULE_SECTION_NAME
		static const char*  mst_pcFileLevelKeyName;// = FILELEVEL_KEY_NAME
		static const char*  mst_pcConsLevelKeyName;// = CONSOLELEVEL_KEY_NAME
		static const char*  mst_pcPathKeyName;     // = PATH_KEY_NAME
		static const char*  mst_pcFileKeyName;     // = FILE_KEY_NAME
		static const UINT8  mst_u8DefaultConsLevel;// = eFatal
		static const UINT8  mst_u8DefaultFileLevel;// = eError
		static const char*  mst_arypcLevelLabels[eAllLevels];// log level labels

	private:
		bool         OpenLogFile();
		void         SetLogFilename();

		bool         m_blIniError;
		bool         m_blLogError;
		UINT8        m_u8ConsoleLevel;
		UINT8        m_u8FileLevel;
		const char*  m_pcIniFilename;
		GFile*		 m_pLogFile;
		char         m_arycLogFileDate[LOGFILE_DATE_SIZE];
		std::string  m_strLogFolder;
		std::string  m_strLogFilename;
		MODULE_INFO  m_aryModuleInfo[eAllSubSystems];
		int          m_iRunningMode;
		unsigned long m_logFileSize;
};

inline GFile* GLoggerConfig::GetLogStream()
{
	return m_pLogFile;
}

inline bool GLoggerConfig::IniNotFound()
{
	return m_blIniError;
}

inline bool GLoggerConfig::LogfileNotAvailable()
{
	return m_blLogError;
}

inline int GLoggerConfig::GetConsoleLevel()
{
	return m_u8ConsoleLevel;
}

inline int GLoggerConfig::GetGeneralLogLevel()
{
	return m_u8FileLevel;
}

inline int GLoggerConfig::GetRunningMode()
{
	return m_iRunningMode;
}

inline void GLoggerConfig::SetRunningMode(int runningMode)
{
	m_iRunningMode = runningMode;
}



#endif // GLOGGERCONFIG_H
