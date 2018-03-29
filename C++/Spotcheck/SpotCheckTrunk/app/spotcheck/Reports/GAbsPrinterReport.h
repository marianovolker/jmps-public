/**
 * GAbsPrinterReport.h
 *
 * © 2013 Gilbarco Inc.
 * Confidential and Proprietary
 *
 * @file GAbsPrinterReport.h
 * @author Guillermo Paris
 * @date May 7 2013
 * @copyright © 2013 Gilbarco Inc. Confidential and Proprietary
 *
 */

#ifndef GABSPRINTERREPORT_H_
#define GABSPRINTERREPORT_H_

#include "Reports/Header/GReportHeader.h"

const int REPO_MAX_BUFFER_SIZE = 2048;

class GAbsPrinterReport
{
	public:
		              GAbsPrinterReport();
		virtual       ~GAbsPrinterReport();

		virtual bool  OpenReport()  = 0; // Mandatory implementation in derived class
		virtual int   TextDump()    = 0;
		        bool  CloseReport();

		inline  int   GetHeaderLength()  { return m_Header.GetLength(); }
		inline  int   GetReportLength()  { return m_iReportLength; }
		inline  char* GetReportText()    { return m_pcDumpBuffer;  }
		inline  int   GetPartialCount()  { return m_iReportCount;  }
		inline  int   GetMaxCharPerLine(){ return m_Header.mst_iMaxLineLength; }


	protected:

		        // Helper methods to allow including parameters in derived class.
		        bool  OpenReport( int nLines, const char*  *pszTemplateLines );
		inline  int   TextDump(GReportHeader::eReportType eType);

		        int   DumpHeaderText(GReportHeader::eReportType eType);
		        int   PutVoidDataLine(int nLine);
		        int   PopulateLine(int nLine, ...);
		        bool  NoMoreRoomOnBuffer(int nChars = 0);

	static const int  mst_CiBufferSize;     // = REPO_MAX_BUFFER_SIZE


	private:

		int           m_iLineCount;
		int           m_iReportCount;
		int           m_iReportLength;
		const char*  *m_pTemplateLines;
		char*         m_pcDumpBuffer;
		char*         m_pcOut;
		GReportHeader m_Header;
};

inline int GAbsPrinterReport::TextDump(GReportHeader::eReportType eType)
{
	return 0; // Behavior undefined at this base class level
}

#endif /* GABSPRINTERREPORT_H_ */

