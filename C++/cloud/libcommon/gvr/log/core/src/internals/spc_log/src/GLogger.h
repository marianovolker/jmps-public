/**
 * © 2013 Gilbarco Inc.
 * Confidential and Proprietary
 *
 * @file GLogger.h
 * @author Guillermo Paris
 * @date April 08 2013
 * @copyright © 2013 2014 Gilbarco Inc. Confidential and Proprietary
 */

#ifndef GLOGGER_H
#define GLOGGER_H

#include <stdarg.h>
#include <GMutex.h>
#include <gvrlog/ModulesAndLevels.h>

/// @brief Gilbarco Domain
namespace gvr
{
/// @brief log domain
namespace log
{


class GLogger
{
	public:

		/**
		 * @enum eTimestampFormatType
		 * @brief  Specifies the chosen date/time format for a logging operation,
		 *         or as a part of the filename.
		 */
		enum eTimestampFormatType
		{
			eFullDateTimeStamp, ///< Full date/time/uSeconds stamp format: "YYYY-MM-DD HH:MM:SS.123456"
			eOnlyNumbersDateYYYYMMDD, ///< Only numeric date stamp format: "YYYYMMDD"
			eTimeHHMMSS         ///< Time stamp format: "HH:MM:SS"
		};

		/**
		 * Static constants generated from the literals defined at HarcodedLiterals.h
		 */
		static const unsigned int mst_CuiYEARBASE  = 1900; ///< base for year counting  ( =1900)
		static const unsigned int mst_CuiMONTHBASE =    1; ///< base for month counting ( =1 January).

		                          /// Logger's static buffer maximum size.
		static const          int mst_CiStaticLogbufferSize = MAX_STATIC_LOGBUFFER_SIZE;

		                          /// Maximum size for header buffer.
		static const          int mst_CiHeaderBufferSize    = HEADER_BUFFER_SIZE;

		                          /// Maximum size for timestamp buffer.
		static const          int mst_CiTimestampBufferSize = TIMESTAMP_BUFFER_SIZE;


		inline        GLoggerConfig* GetConfig();   ///< Gets the configuration object supplied to this log.
		inline static GLogger*       GetInstance(); ///< Gets the only instance (singleton) of this class.

		/**
		 * @brief Creates the only instance (singleton) of this class,
		 *        if it doesn't exist.
		 * @param GLoggerConfig* The pointer to the configuration object.
		 * @return The unique logger instance.
		 */
		static GLogger*     CreateInstance( GLoggerConfig* );
		static void         DestroyInstance();///< Destroy the only instance if it exists.

		/**
		 * @brief Logs the supplied information on console and into the log file.
		  *
		 * @note This method is intended to be accessed internally
		 *       by the log.h functions.
		 *
		 * @param blUnconditional If set to @b true then the following two parameters
		 *        say if the logfile and console outputs are allowed or not for this
		 *        call (intended as a line logging).
		 * @param blLogFile enables or not the file output. This parameter is discarded
		 *        if the first one blUnconditional is set to false.
		 * @param blLogCons enables or not the console output. This parameter is
		 *        discarded if the first one blUnconditional is set to false.
		 * @param eModule The module (subsystem, library, etc) whose short name
		 *        will appear in the output.
		 * @param eLevel The desired log level (from eDebug to eFatal) according to ModulesAndLevels.h
		 * @param pcFmt The printf() like format string. The caller need to provide their own '\n'
		 *        in order to go down to the following line.
		 * @param var_arg_list variable list of arguments according to what is specified in
		 *        the format string.
		 *
		 * @return @b true if the supplied information could be logged to at least one output channel,
		 *         that is console, log file, etc.
		 */
		static bool         Log( bool blUnconditional, bool blLogFile, bool blLogCons,
		                         eSubSystem eModule, eLogLevel eLevel,
		                         const char* pcFmt, va_list var_arg_list);

		/**
		 * @brief Logs the supplied information on console and into the log file.
		 *
		 * @note  This method is intended to be accessed by outside world.
		 *        It is only provided for completeness, Log interface provided
		 *        by log.h is for sure the best practice.
		 *
		 * @param eModule The module (subsystem, library, etc) whose short name
		 *        will appear in the output.
		 * @param eLevel The desired log level (from eDebug to eFatal) according to ModulesAndLevels.h
		 * @param pcFmt The printf() like format string. The caller need to provide their own '\n'
		 *        in order to go down to the following line.
		 * @param ... The variable list of arguments supplied by the expansion of the LOG() macro.
		 *
		 * @return @b true if the supplied information could be logged to at least one output channel,
		 *         that is console, log file, etc.
		 *
		 * @since 1.0.0
		 *
		 */
		static bool         Log( eSubSystem eModule, eLogLevel eLevel,
		                         const char* pcFmt, ... );

		/**
		 * @brief Gets the calculated print size as the number of characters resulting for the expansion
		 *        of the format string applying all the parameters supplied by the variable list args.
		 *        Nothing is either printed or buffered. it is just a size calculation.
		 *
		 * @return The size of the hypothetical printing.
		 */
		static int          GetPrintSize (const char * format, va_list args);

		/**
		 * @brief Fills the supplied buffer with a timestamp according to the specified format.
		 *
		 * @param szTs [Out] The supplied character buffer that will hold the timestamp.
		 * @param iSize maximum number of characters to be written in the buffer. It should be at most
		 *        the buffer size minus one to allow the '\0' string terminator.
		 * @param eTs The specified timestamp format, as indicated by the former enum eTimestampFormatType.
		 */
		static int          String_timestamp(char* szTs, int iSize,
		                                     eTimestampFormatType eTs = eFullDateTimeStamp);


	private:
		/**
		 * @brief Private Constructor (singleton)
		 */
		                    GLogger( GLoggerConfig* );
		                    ~GLogger(); ///< Private Destructor (singleton)

		inline int          GetAllowedLevel(eSubSystem eModule);///< Gets the allowed level for the given module.
		inline int          GetModuleIndex(const char* pcModuleKey);///< Gets the module enum code for the given module key name.
		inline const char*  GetModuleShortName(eSubSystem e);///< Gets the module short name for the given module enum code.

		/**
		 * @brief Generates and writes the line header for the line being logged.
		 * @param eModule
		 * @param eLevel
		 * @param bFile (Optional) If it is @b true then the output will be the log file.
		 *                         If it is @b false then the output will be the console.
		 *
		 * @return @b true if all the characters were successfully written to the output
		 *             channel (console or log file).
		 * @since 0.0.0
		 */
		             bool   WriteHeader(eSubSystem eModule, eLogLevel eLevel, bool bFile=true);

		/**
		 * @brief Writes the supplied character line to the log file.
		 * @param buffer The null terminated character buffer to be written to the log file.
		 * @param iSize The maximum number of characters to write.
		 *
		 * @return @b true if the number of characters successfully written is the planned iSize parameter.
		 *         @b false otherwise.
		 *
		 * @since 0.0.0
		 */
		             bool   WriteToFile(const char* buffer, int iSize);

		/**
		 * @brief Writes the supplied character line to the console.
		 * @param buffer The null terminated character buffer to be written to the console.
		 * @param iSize The maximum number of characters to write.
		 *
		 * @return @b true if the number of characters successfully written is the planned iSize parameter.
		 *         @b false otherwise.
		 *
		 * @since 0.0.0
		 */
		             bool   WriteToConsole(const char* buffer, int iSize);

		GLoggerConfig*      m_pConfig; ///< Pointer to the configuration object.

		static char         mst_arycbuffer[mst_CiStaticLogbufferSize];///< The logger's static print buffer.
		static char         mst_arycHeader[mst_CiHeaderBufferSize];///< Buffer to temporary print the line header.
		static char         mst_arycTimeStamp[mst_CiTimestampBufferSize];///< Buffer to temporary print the line timestamp.
		static GMutex       mst_ClassMutex;///< The logger's mutex
		static GLogger*     mst_pInstance;///< Pointer to the logger's unique instance object.
};


inline GLogger*  GLogger::GetInstance()
{
	return mst_pInstance;
}

inline GLoggerConfig*  GLogger::GetConfig()
{
	return m_pConfig;
}

inline int  GLogger::GetAllowedLevel(eSubSystem eModule)
{
	return m_pConfig->GetModuleLogLevel(eModule);
}

inline int  GLogger::GetModuleIndex(const char* pcModuleKey)
{
	return m_pConfig->GetModuleIndex(pcModuleKey);
}

inline const char*  GLogger::GetModuleShortName(eSubSystem eModule)
{
	return m_pConfig->GetModuleShortName(eModule);
}

} // end namespace log
} // end namespace gvr

#endif // GLOGGER_H
