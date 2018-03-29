/*!
 * © 2013 Gilbarco Inc.
 * Confidential and Proprietary
 *
 * @file GLogger.cpp
 * @author Guillermo Paris
 * @date April 08 2013
 * @copyright © 2013 Gilbarco Inc. Confidential and Proprietary
 */

#include <string.h>
#include <time.h>
#include <sys/time.h>

#include "Common/Util/DEBUGOUT.h"
#include <GSystem.h>

const unsigned int GLogger::CuiYEARBASE = 1900; // base for year counting
const unsigned int GLogger::CuiMONTHBASE =   1; // base for month counting (Jan)

GMutex   GLogger::mst_ClassMutex;
GLogger* GLogger::mst_pInstance = NULL;
char GLogger::mst_arycbuffer	[MAX_BUFFER_SIZE]		= {0x00};
char GLogger::mst_arycHeader	[HEADER_BUFFER_SIZE]	= {0x00};
char GLogger::mst_arycTimeStamp	[TIMESTAMP_BUFFER_SIZE]	= {0x00};

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
		return mst_pInstance;
	}

	GMutexLock oLock(mst_ClassMutex);

	if(mst_pInstance == NULL)
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
	if(mst_pInstance)
	{
		delete mst_pInstance;
		mst_pInstance = NULL;
	}
}

bool GLogger::Log(eSubSystem eModule, eLogLevel eLevel, const char* format, ...)
{

	GLoggerConfig*  pLogConfig = mst_pInstance->GetConfig();

	int iRunningMode = pLogConfig->GetRunningMode();
	bool blLogCons = (iRunningMode !=eEmbedded && iRunningMode != eInjected) && ( eLevel >= pLogConfig->GetConsoleLevel() );

	bool blLogFile = ( !pLogConfig->LogfileNotAvailable() );

	if( blLogFile )
	{
		blLogFile = ( eLevel >= pLogConfig->GetModuleLogLevel(eModule) );
	}

	if( !blLogCons && !blLogFile )
	{
		return true; // nothing to do
	}

	bool blret1(false), blret2(false);

	va_list args;
	va_start(args, format);
	int iLenBuffer = GLogger::GetPrintSize(format, args);
	GMutexLock oLock(mst_ClassMutex);

	memset(mst_arycbuffer, 0, sizeof(mst_arycbuffer));
	if( iLenBuffer >= sizeof(mst_arycbuffer) )
	{
		// This happens exceptionally when large SPOT messages (file browse replies)
		// are dumped to log (in debug level)
		// This portion of code is for splitting the entire buffer because it's
		// too big for the internal log buffer. vnsprintf doesn't finish after
		// the specified characteres are written, but it continues indeed.
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
		vsnprintf(pBuff, iTotalLenToCopy, format, args);
		memcpy(mst_arycbuffer, szBeginBuffer, iPartialLenBeginBuffer);
		memcpy(&mst_arycbuffer[iPartialLenBeginBuffer], pBuff, iTotalLenToCopy);
		memcpy(&mst_arycbuffer[(iTotalLenToCopy+iPartialLenBeginBuffer)], szEndBuffer, iPartialLenEndBuffer);
		delete[] pBuff;
		iLenBuffer = sizeof(mst_arycbuffer) - 1;
		//***************************************************************************
	}
	else // ordinary situation
	{
		iLenBuffer = vsnprintf(mst_arycbuffer, (sizeof(mst_arycbuffer)-1), format, args);
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

int GLogger::GetPrintSize (const char * format, va_list pargs)
{
	int retval;
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
	int iRunningMode =pLogConfig->GetRunningMode();

	if( iRunningMode == eEmbedded ||iRunningMode== eInjected ||iRunningMode ==eDummyEmbedded ||iRunningMode == eDummyInjected)
	{
		std::string sAppName(DIAGNOSTIC_WORD, 5); // "DIAGN"

		sAppName.append(1, 'S');
		String_timestamp(mst_arycTimeStamp,
		                 sizeof(mst_arycTimeStamp), eTimeHHMMSS);

		iLenHeader = snprintf( mst_arycHeader, sizeof(mst_arycHeader),
		                       "%s [%6s] ", mst_arycTimeStamp, sAppName.c_str() );

		return mst_pInstance->WriteToFile(mst_arycHeader, iLenHeader);
	}
	else if( bFile )
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

	if( (iRunningMode == eEmbedded || iRunningMode ==eInjected || iRunningMode ==eDummyEmbedded ||iRunningMode == eDummyInjected) &&
		m_pConfig->HasDateChanged() )
	{
		m_pConfig->SwitchLogFile();
	}

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


/*!
 * It prepends timestamp to the logged string
 * helper function
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
			             m.tm_year + CuiYEARBASE, m.tm_mon + CuiMONTHBASE, m.tm_mday);
			break;

		case eTimeHHMMSS:
			n = snprintf(szTs, (iSize-1), "%02d:%02d:%02d",
			             m.tm_hour, m.tm_min, m.tm_sec);
			break;

		case eFullDateTimeStamp:
		default:
			n = snprintf(szTs, (iSize-1), "%4d-%02d-%02d %02d:%02d:%02d.%06ld",
			             m.tm_year + CuiYEARBASE, m.tm_mon + CuiMONTHBASE, m.tm_mday,
			             m.tm_hour, m.tm_min, m.tm_sec, tv.tv_usec);
			break;
	}

	szTs[iSize-1] = '\0';

	// returns the timestamp length
	return n;
}

