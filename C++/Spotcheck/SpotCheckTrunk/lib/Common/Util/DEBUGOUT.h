/*
 * © 2011, 2012, 2013 Gilbarco Inc.
 * Confidential and Proprietary
 *
 * @file DEBUGOUT.h
 * @author Vance Tate & Guillermo Paris
 * @date Apr 11 2013
 * @copyright © 2013 Gilbarco Inc. Confidential and Propietary
 *
 */

#ifndef DEBUGOUT_h
#define DEBUGOUT_h

#include <stdio.h>

typedef enum
{
	eLowestLogLevel,
	eDebug = eLowestLogLevel,  //0
	eTrace,						//1
	eNotice,					//2
	eInfo,						//3
	eWarning,					//4
	eError,						//5
	eFatal,						//6
	eSecurity,					//7
	eHighestLogLevel = eSecurity,
	eAllLevels
} eLogLevel;
 
typedef enum
{
	eFirstDevice,
	ePrinter = eFirstDevice,
	eTrind,
	eBarcode,
	eCardReader,
	eContactlessReader,
	eKeypad,
	eCashAcceptor,
	eDisplay,
	ePOSInterface,
	eSmartMerch,
	eTimer,
	eBios,
	eSpot,
	eSpotProtocol,
	ePump,
	eCrindToCrind,
	eCommon,
	eMenuManager,
	eWinTemplate,
	eReport,
	eConfig,
	ePassThru,
	eResourcesService,
	eUnCategorized,
	eLastDevice = eUnCategorized,
	eAllSubSystems
} eSubSystem;

#include "Common/Logger/GLoggerConfig.h"
#include "Common/Logger/GLogger.h"

// Macros use for new logging framework.

#ifdef _NO_LOG // not logging at all

#define LOG(ss,ll,fmt,...)
#define LOG_NO_TIMESTAMP(ss,ll,fmt,...)

#else // YES_LOG ! :)

// Log with time stamp.
#define LOG(ss,ll,fmt,...) \
{ \
	if( GLogger::GetInstance() != NULL ) \
	{ \
		GLogger::Log(ss, ll, fmt, ##__VA_ARGS__); \
	} \
} \

/* Former log macro used for simple debugging
#define LOG(ss,ll,fmt,...) \
{ \
	char buf[1024]; \
	snprintf(buf,sizeof(buf),fmt,__VA_ARGS__); \
	printf("%s\n",buf); \
}
*/

//
// Log without a time stamp.
#define LOG_NO_TIMESTAMP(ss,ll,fmt,...) \
{ \
	char buf[1024]; \
	snprintf(buf,sizeof(buf),fmt,__VA_ARGS__); \
	printf("%s\n",buf); \
}
//

#endif // _NO_LOG

#endif // DEBUGOUT_h

