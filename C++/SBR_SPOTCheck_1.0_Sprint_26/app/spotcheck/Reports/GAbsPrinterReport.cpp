/**
 * GAbsPrinterReport.cpp
 *
 *  Created on: May 07 2013
 *      Author: G.Paris
 */

#include <stdio.h>
#include <string.h>
#include <stdarg.h>
#include "GAbsPrinterReport.h"

const int GAbsPrinterReport::mst_CiBufferSize = REPO_MAX_BUFFER_SIZE;

GAbsPrinterReport::GAbsPrinterReport()
	: m_pTemplateLines(NULL), m_pcOut(NULL),
	  m_iReportCount(0), m_iReportLength(0),
	  m_iLineCount(0)
{
	m_pcDumpBuffer = new char[mst_CiBufferSize];
}

GAbsPrinterReport::~GAbsPrinterReport()
{
	if(m_pcDumpBuffer)
	{
		delete m_pcDumpBuffer;
		m_pcDumpBuffer = NULL;
		m_pcOut = NULL;
	}
}

bool GAbsPrinterReport::OpenReport( int nLines, const char*  *pszTemplateLines )
{
	if(nLines <= 0 || !m_pcDumpBuffer || !pszTemplateLines)
		return false; // report can't be opened under this conditions

	m_iReportLength = m_iReportCount = 0;
	memset(m_pcDumpBuffer, 0, mst_CiBufferSize);
	m_pcOut = m_pcDumpBuffer;

	m_iLineCount = nLines;
	m_pTemplateLines = pszTemplateLines;
	return true;
}

int GAbsPrinterReport::DumpHeaderText(GReportHeader::eReportType eType)
{
	m_iReportCount = m_Header.DumpText(eType, m_pcDumpBuffer, mst_CiBufferSize);
	m_pcOut = m_pcDumpBuffer + m_iReportCount;

	return m_iReportCount;
}

int GAbsPrinterReport::PutVoidDataLine(int nLine)
{
	if(nLine < 0 || nLine >= m_iLineCount)
		return 0;

	if(!m_pcDumpBuffer || !m_pTemplateLines)
		return 0;

	int iCharsWritten = sprintf(m_pcOut, "%s", m_pTemplateLines[nLine]);

	m_pcOut        += iCharsWritten;
	m_iReportCount += iCharsWritten;

	return iCharsWritten;
}

int GAbsPrinterReport::PopulateLine(int nLine, ...)
{
	if(nLine < 0 || nLine >= m_iLineCount)
		return 0;

	if(!m_pcDumpBuffer || !m_pTemplateLines)
		return 0;

	int     iCharsWritten = 0;
	va_list args;

	va_start(args, nLine);
	iCharsWritten = vsprintf(m_pcOut, m_pTemplateLines[nLine], args);
	va_end(args);

	m_pcOut        += iCharsWritten;
	m_iReportCount += iCharsWritten;

	return iCharsWritten;
}

bool GAbsPrinterReport::CloseReport()
{
	if(!m_pcDumpBuffer || !m_pTemplateLines || !m_pcOut)
		return false; // report never opened or already closed

	m_pcOut = NULL;
	m_iReportLength = m_iReportCount;
	m_iReportCount = m_iLineCount = 0;
	return true;
}

bool GAbsPrinterReport::NoMoreRoomOnBuffer(int nChars /* = 0 */)
{
	if(nChars <= 0) //default action: check room for another paper row
		return ( m_iReportCount + m_Header.mst_iMaxLineLength >= mst_CiBufferSize );

	return ( m_iReportCount + nChars >= mst_CiBufferSize );
}


