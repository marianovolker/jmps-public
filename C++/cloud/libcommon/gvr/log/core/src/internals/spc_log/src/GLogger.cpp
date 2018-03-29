/**
 * © 2013 Gilbarco Inc.
 * Confidential and Proprietary
 *
 * @file GLogger.cpp
 * @author Guillermo Paris
 * @date April 08 2013
 * @copyright © 2013 2014 Gilbarco Inc. Confidential and Proprietary
 */

#include <string.h>
#include <time.h>
#include <sys/time.h>
#include <GSystem.h>
#include <gvrlog/ModulesAndLevels.h>
#include "GLoggerConfig.h"
#include "GLogger.h"

using namespace gvr::log;

//const unsigned int GLogger::mst_CuiYEARBASE = 1900;
//const unsigned int GLogger::mst_CuiMONTHBASE =   1;

GMutex   GLogger::mst_ClassMutex;
GLogger* GLogger::mst_pInstance = NULL;
char     GLogger::mst_arycbuffer    [mst_CiStaticLogbufferSize];
char     GLogger::mst_arycHeader    [mst_CiHeaderBufferSize];
char     GLogger::mst_arycTimeStamp [mst_CiTimestampBufferSize];

GLogger::GLogger(GLoggerConfig* pConfig)
	: m_pConfig(pConfig)
{
}

GLogger::~GLogger()
{
}

GLogger* GLogger::CreateInstance(GLoggerConfig* pConfig)
{
	if(mst_pInstance)
	{
		return mst_pInstance; // logger already instantiated
	}

	GMutexLock oLock(mst_ClassMutex);

	if(mst_pInstance == NULL) // race condition protection
	{
		if( pConfig == NULL )
		{
			return NULL; // no configuration supplied
		}

		mst_pInstance = new GLogger(pConfig);
	}

	return mst_pInstance;
}

void GLogger::DestroyInstance()
{
	if( mst_pInstance == NULL )
	{
		return; // logger not instantiated.
	}

	GMutexLock oLock(mst_ClassMutex);

	if( mst_pInstance == NULL )
	{
		return; // race condition protection
	}
	else
	{
		delete mst_pInstance;
		mst_pInstance = NULL;
	}
}

bool GLogger::Log(bool blUnconditional, bool blToFile, bool blToConsole,
                  eSubSystem eModule, eLogLevel eLevel, const char* format, va_list var_arg_list)
{
	if( mst_pInstance == NULL )
	{
		return false; // logger not instantiated.
	}

	GMutexLock oLock(mst_ClassMutex);

	if( mst_pInstance == NULL )
	{
		return false; // race condition protection
	}

	bool blLogFile = false;
	bool blLogCons = false;

	if( blUnconditional )
	{
		blLogFile = blToFile;    // logging destination is controlled by 2nd and 3rd parameters
		blLogCons = blToConsole;
	}
	else // need to check a condition to log
	{
		GLoggerConfig*  pLogConfig = mst_pInstance->GetConfig();

		blLogFile = !pLogConfig->LogfileNotAvailable();

		if( blLogFile )
		{
			blLogFile = ( eLevel >= pLogConfig->GetModuleLogLevel(eModule) );
		}

		blLogCons = ( eLevel >= pLogConfig->GetConsoleLevel() );
	}

	if( !blLogCons && !blLogFile )
	{
		return false; // nothing to do
	}

	bool blret1 = true, blret2 = true; // default result is OK unless forced to perform log operation

	va_list args;
	va_copy(args, var_arg_list);

	int iLenBuffer = GLogger::GetPrintSize(format, args);

	memset(mst_arycbuffer, 0, sizeof(mst_arycbuffer));

	if( iLenBuffer >= sizeof(mst_arycbuffer) )
	{
		// This happens exceptionally when large SPOT messages (file browse replies)
		// are dumped to log (in debug level)
		// This portion of code is for splitting the entire buffer because it's
		// too big for the internal log buffer. vnsprintf doesn't finish after
		// the specified number of characters are written, but it continues indeed.
		//
		//**********************************************************************
		char szBeginBuffer[52];
		char szEndBuffer[52];
		memset(szBeginBuffer, 0, sizeof(szBeginBuffer));
		memset(szEndBuffer, 0, sizeof(szEndBuffer));
		int iPartialLenBeginBuffer = snprintf(szBeginBuffer, (sizeof(szBeginBuffer)-1), "Truncated: [");
		int iPartialLenEndBuffer = snprintf(szEndBuffer, (sizeof(szEndBuffer)-1), "]].\n");
		int iTotalLenToCopy = ((sizeof(mst_arycbuffer)-1) - iPartialLenBeginBuffer - iPartialLenEndBuffer);

		char * pBuff = new char [iLenBuffer+4];
		memset(pBuff, 0, iLenBuffer+4);
		vsnprintf(pBuff, iTotalLenToCopy, format, var_arg_list);
		memcpy(mst_arycbuffer, szBeginBuffer, iPartialLenBeginBuffer);
		memcpy(&mst_arycbuffer[iPartialLenBeginBuffer], pBuff, iTotalLenToCopy);
		memcpy(&mst_arycbuffer[(iTotalLenToCopy+iPartialLenBeginBuffer)], szEndBuffer, iPartialLenEndBuffer);
		delete[] pBuff;
		iLenBuffer = sizeof(mst_arycbuffer) - 1;
		//***************************************************************************
	}
	else // ordinary situation
	{
		iLenBuffer = vsnprintf(mst_arycbuffer, (sizeof(mst_arycbuffer)-1), format, var_arg_list);
	}
	va_end(args);

	if( blLogFile )
	{
		blret1 = mst_pInstance->WriteHeader(eModule, eLevel, true);
		blret1 = blret1 && mst_pInstance->WriteToFile(mst_arycbuffer, iLenBuffer);
	}

	if( blLogCons )
	{
		blret2 = mst_pInstance->WriteHeader(eModule, eLevel, false);
		blret2 = blret2 && mst_pInstance->WriteToConsole(mst_arycbuffer, iLenBuffer);
	}

	return (blret1 && blret2);
}


bool GLogger::Log(eSubSystem eModule, eLogLevel eLevel, const char* format, ...)
{
	if( mst_pInstance == NULL )
	{
		return false; // logger not instantiated.
	}

	mst_ClassMutex.Lock();

	if( mst_pInstance == NULL )
	{
		mst_ClassMutex.Unlock();
		return false; // race condition protection
	}

	bool blLogFile = false;
	bool blLogCons = false;

	GLoggerConfig*  pLogConfig = mst_pInstance->GetConfig();

	blLogFile = !pLogConfig->LogfileNotAvailable();

	if( blLogFile )
	{
		blLogFile = ( eLevel >= pLogConfig->GetModuleLogLevel(eModule) );
	}

	blLogCons = ( eLevel >= pLogConfig->GetConsoleLevel() );

	if( !blLogCons && !blLogFile )
	{
		mst_ClassMutex.Unlock();
		return false; // nothing to do
	}

	va_list args;
	va_start(args, format);
	mst_ClassMutex.Unlock(); // locked again while executing the method below

	bool blret = Log( true, blLogFile, blLogCons, eModule, eLevel, format, args );

	va_end(args);
	return blret;
}

int GLogger::GetPrintSize (const char * format, va_list pargs)
{
	int retval = 0;
	va_list argcopy;
	va_copy(argcopy, pargs);
	retval = vsnprintf(NULL, 0, format, argcopy);
	va_end(argcopy);
	return retval;
}

bool GLogger::WriteHeader(eSubSystem eModule, eLogLevel eLevel, bool bFile)
{

	GLoggerConfig*   pLogConfig = mst_pInstance->GetConfig();
	int iLenHeader;

	memset(mst_arycHeader	, 0, sizeof(mst_arycHeader));
	memset(mst_arycTimeStamp, 0, sizeof(mst_arycTimeStamp));

	if( bFile )
	{
		String_timestamp(mst_arycTimeStamp, sizeof(mst_arycTimeStamp));

		iLenHeader = snprintf( mst_arycHeader, sizeof(mst_arycHeader),
		                       "%s [%-3s] [%-7s]: ", mst_arycTimeStamp,
		                       mst_pInstance->GetModuleShortName(eModule),
		                       pLogConfig->GetLevelLabel(eLevel) );

		return mst_pInstance->WriteToFile(mst_arycHeader, iLenHeader);
	}
	else // console output doesn't have timestamp prefix.
	{
		iLenHeader = snprintf( mst_arycHeader, sizeof(mst_arycHeader),
		                       "[%-3s] [%-7s]: ",
		                       mst_pInstance->GetModuleShortName(eModule),
		                       pLogConfig->GetLevelLabel(eLevel) );

		return mst_pInstance->WriteToConsole(mst_arycHeader, iLenHeader);
	}
}

bool GLogger::WriteToFile(const char* buffer, int iSize)
{
	bool bResult = false;

	int iRunningMode = m_pConfig->GetRunningMode();

/*
	TODO: See why this provides buggy filename when daily switch file is implemented !
	if( m_pConfig->HasDateChanged() )
	{
		m_pConfig->SwitchLogFile();
	}
*/
	GFile* pLogFile = m_pConfig->GetLogStream();

	if( pLogFile )
	{
		int iBytesWritten = 0;
		bResult = ( pLogFile->Write((const char*)buffer, iSize, iBytesWritten) && (iSize == iBytesWritten) );
	}

	if(!bResult)
	{
		GSystem::Perror("ERROR: writing on log file.");
	}

	return bResult;
}

bool GLogger::WriteToConsole(const char* buffer, int iSize)
{
	bool bResult = false;

	int iBytesWritten = printf("%s", buffer);
	bResult = ( iSize == iBytesWritten );

	if(!bResult)
	{
		GSystem::Perror("ERROR: writing at console.");
	}

	return bResult;
}


/**
 * It prepends timestamp to the logged string.
 * This is a helper function
 */
int GLogger::String_timestamp(char* szTs, int iSize,
                              eTimestampFormatType eTSFormat /* = eFullDateTimeStamp */)
{
	int              n;
	struct tm        m;
	struct timeval  tv;
	struct timezone tz;

	memset(szTs, 0, iSize);

	gettimeofday(&tv, &tz);

	GSystem::LocalTime(&tv.tv_sec, &m);

	switch(eTSFormat)
	{
		case eOnlyNumbersDateYYYYMMDD:
			n = snprintf(szTs, (iSize-1), "%4d%02d%02d",
			             m.tm_year + mst_CuiYEARBASE, m.tm_mon + mst_CuiMONTHBASE, m.tm_mday);
			break;

		case eTimeHHMMSS:
			n = snprintf(szTs, (iSize-1), "%02d:%02d:%02d",
			             m.tm_hour, m.tm_min, m.tm_sec);
			break;

		case eFullDateTimeStamp:
		default:
			n = snprintf(szTs, (iSize-1), "%4d-%02d-%02d %02d:%02d:%02d.%06ld",
			             m.tm_year + mst_CuiYEARBASE, m.tm_mon + mst_CuiMONTHBASE, m.tm_mday,
			             m.tm_hour, m.tm_min, m.tm_sec, tv.tv_usec);
			break;
	}

	szTs[iSize-1] = '\0';

	// returns the timestamp length
	return n;
}

