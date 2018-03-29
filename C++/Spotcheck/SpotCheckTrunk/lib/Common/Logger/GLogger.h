/*!
 * © 2013 Gilbarco Inc.
 * Confidential and Proprietary
 *
 * @file GLogger.h
 * @author Guillermo Paris
 * @date April 08 2013
 * @copyright © 2013 Gilbarco Inc. Confidential and Proprietary
 */

#ifndef GLOGGER_H
#define GLOGGER_H

#include <GMutex.h>
#include <stdarg.h>

#define MAX_BUFFER_SIZE         (1024*8) // 8Kb for vnsprintf()
#define HEADER_BUFFER_SIZE            64
#define TIMESTAMP_BUFFER_SIZE         30


class GLogger
{
	public:
		enum eTimestampFormatType
		{
			eFullDateTimeStamp, eOnlyNumbersDateYYYYMMDD, eTimeHHMMSS
		};

	public:
		inline        GLoggerConfig* GetConfig();
		inline static GLogger*       GetInstance();

		static GLogger*     CreateInstance( GLoggerConfig* );
		static void         DestroyInstance();
		static bool         Log(eSubSystem eModule, eLogLevel eLevel, const char* pcFmt, ...);
		static int          GetPrintSize (const char * format, va_list args);
		static int          String_timestamp(char* szTs, int iSize,
		                                     eTimestampFormatType eTs = eFullDateTimeStamp);

		static const unsigned int CuiYEARBASE;
		static const unsigned int CuiMONTHBASE;

	private:
		                    GLogger( GLoggerConfig* );
		                    ~GLogger();

		inline int          GetAllowedLevel(eSubSystem eModule);
		inline int          GetModuleIndex(const char* pcModuleKey);
		inline const char*  GetModuleShortName(eSubSystem e);
			   bool         WriteHeader(eSubSystem eModule, eLogLevel eLevel, bool bFile=true);
			   bool         WriteToFile(const char* buffer, int iSize);
			   bool         WriteToConsole(const char* buffer, int iSize);

		GLoggerConfig*      m_pConfig;

		static char         mst_arycbuffer		[MAX_BUFFER_SIZE];
		static char         mst_arycHeader		[HEADER_BUFFER_SIZE];
		static char         mst_arycTimeStamp	[TIMESTAMP_BUFFER_SIZE];
		static GMutex       mst_ClassMutex;
		static GLogger*     mst_pInstance;
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

#endif // GLOGGER_H
