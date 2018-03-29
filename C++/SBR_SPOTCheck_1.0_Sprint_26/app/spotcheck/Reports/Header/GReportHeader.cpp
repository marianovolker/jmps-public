/**
 * GReportHeader.cpp
 *
 *  Created on: May 07 2013
 *      Author: G.Paris
 */

#include <stdio.h>
#include <stdarg.h>
#include "Version/version.h"
#include "Version/buildinfo.h"
#include "SPOT/SystemInformation/GGlobalSystemInformation.h"
#include "Common/Util/DEBUGOUT.h"
#include "GReportHeader.h"

const char* STR_HWR_REPO  = "SYSTEM HEALTH";
const char* STR_TEST_REPO = "TEST INFO";

const char*  GReportHeader::mst_arypcHeaderLines[HEADER_MAX_LINES] =
{
	"\n",                                // line  0
	"%19.19s\n",                         // line  1
	"SPOTCheck %9s rev. %s\n",           // line  2
	"%s REPORT\n",                       // line  3
	"\n",                                // line  4
	"-------------------------------\n", // line  5
	"Unit Identification:\n",            // line  6
	"SPOT S/N:  %16s\n",                 // line  7
	"SPOT type: %16s\n",                 // line  8
	"SPOT IP:   %16s\n",                 // line  9
	"\n"                                 // line 10
};

const int GReportHeader::mst_iMaxLineLength   = REPO_MAX_COLUMNS + 1;
const int GReportHeader::mst_iLineCount       = HEADER_MAX_LINES;

GReportHeader::GReportHeader()
	: m_iMaxBufferLength(HEADER_MAX_CHARS),
	  m_iHeaderLength(0), m_pcOut(NULL)
{
}

GReportHeader::~GReportHeader()
{
}

int GReportHeader::PopulateLine(int nLine, ...)
{
	if(!m_pcOut || nLine < 0 || nLine >= mst_iLineCount)
		return 0;

	int     iCharsWritten = 0;
	va_list args;

	va_start(args, nLine);
	iCharsWritten = vsnprintf(m_pcOut, mst_iMaxLineLength,
	                          mst_arypcHeaderLines[nLine], args);
	va_end(args);

	m_pcOut         += iCharsWritten;
	m_iHeaderLength += iCharsWritten;

	return iCharsWritten;
}

int GReportHeader::DumpText(eReportType eRepoType, char* pcBuffer, int iMaxChars)
{
	if(pcBuffer == NULL || iMaxChars <= mst_iMaxLineLength)
		return 0; // bad parameters

	const char* pcRepoType = STR_HWR_REPO;
	char arycTimestamp[mst_iMaxLineLength];

	arycTimestamp[0] = '\0';
	m_iMaxBufferLength = iMaxChars;
	m_iHeaderLength = 0;
	m_pcOut = pcBuffer;

	if(eRepoType == eTestResultsReport)
		pcRepoType = STR_TEST_REPO;

	PopulateLine(0);
	if( NoMoreRoomOnBuffer() )
		return m_iHeaderLength; // no more room for another header text line

	GLogger::String_timestamp(arycTimestamp, mst_iMaxLineLength);
	PopulateLine(1, arycTimestamp);
	if( NoMoreRoomOnBuffer() )
		return m_iHeaderLength; // no more room for another header text line

	PopulateLine(2, gszVersion, gBuildNumber);
	if( NoMoreRoomOnBuffer() )
		return m_iHeaderLength;

	PopulateLine(3, pcRepoType);
	if( NoMoreRoomOnBuffer() )
		return m_iHeaderLength;

	PopulateLine(4);
	if( NoMoreRoomOnBuffer() )
		return m_iHeaderLength;

	PopulateLine(5);
	if( NoMoreRoomOnBuffer() )
		return m_iHeaderLength;

	PopulateLine(6);
	if( NoMoreRoomOnBuffer() )
		return m_iHeaderLength;

	PopulateLine(7, GetSPOTSerialNo());
	if( NoMoreRoomOnBuffer() )
		return m_iHeaderLength;

	PopulateLine(8, GetSPOTType());
	if( NoMoreRoomOnBuffer() )
		return m_iHeaderLength; // no more room for another header text line

	PopulateLine(9, GetSPOTIPAddr());
	if( m_iHeaderLength + 2 >= iMaxChars )
		return m_iHeaderLength; // no more room for appending "\n\0"

	PopulateLine(10);
	pcBuffer[ m_iHeaderLength ] = '\0'; // safety

	return m_iHeaderLength;
}

char* GReportHeader::GetSPOTSerialNo()
{
	GGlobalSystemInformation* pSysInfo = GGlobalSystemInformation::GetInstance();
	return (pSysInfo->GetSPOTInfo()).m_pcSerialNumber;
}

const char* GReportHeader::GetSPOTType()
{
	GGlobalSystemInformation* pSysInfo = GGlobalSystemInformation::GetInstance();

	if( pSysInfo->isM3Unit() )
		return "M3";
	else if( pSysInfo->isM5Unit() )
		return "M5";
	else
		return "Unknown";
}

const char* GReportHeader::GetSPOTIPAddr()
{
	GGlobalSystemInformation* pSysInfo = GGlobalSystemInformation::GetInstance();
	return pSysInfo->GetIPAddress();
}


