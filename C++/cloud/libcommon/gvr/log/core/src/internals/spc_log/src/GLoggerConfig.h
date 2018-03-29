/**
 * © 2013 Gilbarco Inc.
 * Confidential and Proprietary
 *
 * @file GLoggerConfig.h
 * @author Guillermo Paris
 * @date April 08 2013
 * @copyright © 2013 2014 Gilbarco Inc. Confidential and Proprietary
 */

#ifndef GLOGGERCONFIG_H
#define GLOGGERCONFIG_H

#include <string>
#include <GTypes.h>
#include <GFile.h>
#include <gvrlog/ModulesAndLevels.h>
#include "HardcodedLiterals.h"


/// @brief Gilbarco Domain
namespace gvr
{
/// @brief log domain
namespace log
{

/**
 * @class GLoggerConfig
 *
 * @brief  The main responsibility of this class is to read and hold the configuration variables
 *         for the logger object, and also to open its logfile for reading in appending mode.
 *
 * @author Guillermo Paris
 * @date   2014-10-02
 * @since  0.0.0
 */
class GLoggerConfig
{
	public:

		/**
		 * @struct MODULE_INFO
		 * @brief  Contains the basic information of a software module for logging.
		 */
		struct MODULE_INFO
		{
			/// The desired log level as it appears in enumeration eLogLevel at ModulesAndLevels.h
			UINT8        u8Level;

			/// The acronym or "short name" of the module (no more than 3 chars)
			const char*  pcShortName;

			///< The "key name" as it appears in config file. ( i.e. CardReader=3 )
			const char*  pcKeyName;
		};

		/**
		 * Static constants generated from the literals defined at HarcodedLiterals.h
		 */
		static const char    mst_cPathSeparator        = PATH_SEPARATOR; ///< '/'
		static const UINT8   mst_u8DefaultConsLevel    = eFatal; ///< default minimum level for console logging.
		static const UINT8   mst_u8DefaultFileLevel    = eError; ///< default minimum level for file logging.

		                     /// length of "DIAGS"
		static const int     mst_CiDiagnosticAcronymLength = DIAGNOSTIC_WORD_LENGTH;

		                     /// length of "YYYYMMDD"
		static const int     mst_CiOnlyDateFmtMaxSize      = DATEFORMAT_YYYYMMDD_LENGTH;

		                     /// enough buffer size to hold "YYYY-MM-DD HH:mm:SS.123456"
		static const int     mst_CiLogfilenameDateMaxSize  = LOGFILENAME_DATE_MAXSIZE;

		                     /// "./"
		static const char*   mst_pcLogPathName;     // = LOG_DEFAULT_PATHNAME

		                     /// "libgvrcommon.log"
		static const char*   mst_pcLogFileName;     // = LOG_DEFAULT_FILENAME

		                     /// "libgvrlog.ini"
		static const char*   mst_pcIniConfigName;   // = INI_CONFIG_DEFAULT_FILE

		                     /// Default general section name into the .ini file.
		static const char*   mst_pcGenSectionName;  // = GENERAL_SECTION_NAME

		                     /// Default module section name into the .ini file.
		static const char*   mst_pcModSectionName;  // = MODULE_SECTION_NAME

		                     /// Key name to retrieve the default file logging level from .ini file.
		static const char*   mst_pcFileLevelKeyName;// = FILELEVEL_KEY_NAME

		                     /// DKey name to retrieve the default console logging level from .ini file.
		static const char*   mst_pcConsLevelKeyName;// = CONSOLELEVEL_KEY_NAME

		                     /// Key name to retrieve the logfile directory path from .ini file
		static const char*   mst_pcPathKeyName;     // = PATH_KEY_NAME

		                     /// Key name to retrieve the log filename from .ini file
		static const char*   mst_pcFileKeyName;     // = FILE_KEY_NAME

		                     /// Array of text labels for each log level ranging from eDebug to eFatal .
		static const char*   mst_arypcLevelLabels[eAllLevels];// log level labels

		/**
		 * @brief This constructor initializes all internal variables to default levels, including the module table,
		 *        and takes and hold the full pathname of the configuration file for later reading.
		 *
		 * @param pcIniFilename The full pathname of the configuration file. If this is NULL,
		 *                      then the default pathname "libgvrlog.h" is taken into account.
		 */
		GLoggerConfig(const char* pcIniFilename = NULL);
		~GLoggerConfig(); ///< Destructor

		/**
		 * @brief Initializes the configuration object reading the configuration file,
		 *        set defaults values on errors, and finally opens the log file for appending.
		 *
		 * @return @b false if there was an error opening the log file. @b true otherwise.
		 * @since 0.0.0
		 *
		 */
		       bool          Initialize();

		/**
		 * @brief  Says if the current day (date) is the same to the one the logfile was open.
		 *
		 * @return @b true if the current date is exactly the same as the logfile creation date.
		 *         @b false otherwise.
		 *
		 * @since 0.0.0
		 *
		 */
		       bool          HasDateChanged();

		/**
		 * @brief Closes the current log file, and opens a new one with
		 *        the current date stamp in its name, according to SPOT logging rules.
		 *
		 * @note Due this implementation was ported from SPOTCheck project, this feature
		 *       is currently disabled until it is tested exhaustively in all platforms.
		 *
		 * @return @b true if the new file could be opened OK, @b false otherwise.
		 * @since 0.0.0
		 *
		 */
		       bool          SwitchLogFile();
		inline GFile*        GetLogStream();  ///< Returns a reference to the logger's underlying stream.
		inline bool          IniNotFound(); ///< Returns @b true if the configuration file was not found.
		inline bool          LogfileNotAvailable();///< Returns @b true if the log file couldn't be opened.
		inline int           GetConsoleLevel();   ///< Returns the minimum allowed level for console logging.
		       bool          SetConsoleLevel(int);///< Sets the minimum allowed level for console logging.
		inline int           GetGeneralLogLevel();///< Returns the minimum allowed level for file logging.
		       bool          SetGeneralLogLevel(int);///< Sets the minimum allowed level for file logging.

		/**
		 * @brief Returns the text label for the specified level number.
		 *
		 * @note
		 *
		 * @param e The specified log level. Should range from eDebug to eFatal values.
		 *
		 * @return const char*  The text label for the specified level number.
		 *
		 * @example      pLogConfig->GetLevelLabel(eError) == "ERROR"
		 *
		 * @since 0.0.0
		 *
		 */
		const  char*         GetLevelLabel(eLogLevel e);

		/**
		 * @brief Returns the level number for the specified text label.
		 *
		 * @param pcLevel The specified text label corresponding to the enumeration value.
		 *
		 * @return The enumeration level value corresponding to the supplied text label.
		 *         If the text label does not match to any enumeration label representation,
		 *         then eError is returned.
		 *
		 * @since 0.0.0
		 *
		 */
		       eLogLevel     GetLevelIndex(const char* pcLevel);

		/**
		 * @brief Returns the module log level (in module table) for the specified module.
		 *        The returned module log level is replaced for the general file level
		 *        if the former is lower than the last one.
		 *
		 * @param e The enumeration code corresponding to the desired module (subsystem, library, etc.)
		 *
		 * @return The log level of the module if it is higher than the general file level.
		 *         The general file level otherwise.
		 *
		 * @since 0.0.0
		 *
		 */
		       int           GetModuleLogLevel(eSubSystem e);

		/**
		 * @brief Returns the module short name (max. 3 chars) for the specified module.
		 *
		 * @param e The enumeration code corresponding to the desired module (subsystem, library, etc.).
		 *
		 * @return The short name corresponding to the supplied module code.
		 * @example      pLogConfig->GetModuleShortName(eCardReader) == "CRD"
		 *
		 * @since 0.0.0
		 *
		 */
		const  char*         GetModuleShortName(eSubSystem e);

		/**
		 * @brief Returns the module enumeration code corresponding to the supplied key name
		 *        as it appears in the left column inside the .ini configuration file.
		 *
		 * @param pcModuleKey The key name associated with the module.
		 *
		 * @return The corresponding module enumeration code if the key name is properly recognized.
		 *         If the key name is not recognized, then eUnCategorized is returned.
		 * @since 0.0.0
		 *
		 */
		       eSubSystem    GetModuleIndex(const char* pcModuleKey);

		/**
		 * @brief  Set all the module information into the module table, overriding then
		 * the configuration file and the defaults values.
		 *
		 * @note Use this method sparingly.
		 *
		 * @param iMod Module code (code index in the table) whose information is to be updated.
		 * @param iLvl The new logging level for this module.
		 * @param pcShortName (Optional) The new short name value (clipped to 3 chars max.)
		 * @param pcKeyName   (Optional) The new key name corresponding to the module
		 *
		 * @return @b true if the first and second parameters are in correct range,
		 *         @b false otherwise.
		 *
		 * @since 0.0.0
		 *
		 */
		       bool          SetModuleInfo(int iMod, int iLvl,
		                                   const char* pcShortName = NULL,
                                           const char* pcKeyName = NULL);

		/**
		 * @brief  Gets the current running mode.
		 *
		 * @note By now the only running mode allowed is "external" 0. For future versions,
		 *       embedded mode could be added if the logging behavior or log filename need
		 *       modifications.
		 *
		 * @return The running mode (always 0, reserved for future versions).
		 *
		 * @since 0.0.0
		 *
		 */
		inline int           GetRunningMode();
		inline void          SetRunningMode(int runningMode);///< Sets the running mode (unused).
		const  char*         GetLogFilePath(); ///< Gets the full pathname to the log file.
		void                 CalcLogFileSize();///< Force the calculation of the log filename size.

		inline unsigned long GetLogFileSize();///< Gets the log file size at the time the last calculation was forced.

	private:
		bool                 OpenLogFile();   ///< Opens the log file for appending.
		void                 SetLogFilename();///< Set the new log filename according to SPOT rules using the current date.

		bool                 m_blIniError;     ///< In true denotes an error opening the configuration file.
		bool                 m_blLogError;     ///< In true denotes an error opening the log file.
		UINT8                m_u8ConsoleLevel; ///< Log level priority for logging on console.
		UINT8                m_u8FileLevel;    ///< General log level priority for logging in file
		const char*          m_pcIniFilename;  ///< Full pathname to the configuration file.
		GFile*               m_pLogFile;       ///< Pointer to the logger's underlying stream.
		char                 m_arycLogFileDate[mst_CiLogfilenameDateMaxSize]; ///< Char buffer for date/time stamp.
		std::string          m_strLogFolder;   ///< Full pathname to the logfile folder.
		std::string          m_strLogFilename; ///< Filename for the log file.
		MODULE_INFO          m_aryModuleInfo[eAllSubSystems]; ///< internal module table.
		int                  m_iRunningMode;   ///< The running mode. Currently 0. Reserved for future use.
		unsigned long        m_logFileSize;    ///< The actual size of the log file at the time the last calculation was forced.
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
	/* m_iRunningMode = runningMode; The running mode to be used in future versions. */
}

inline unsigned long GLoggerConfig::GetLogFileSize()
{
	return m_logFileSize;
}


} // end namespace log
} // end namespace gvr


#endif // GLOGGERCONFIG_H
