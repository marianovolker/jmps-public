/**
 * GReportHeader.h
 *
 * © 2013 Gilbarco Inc.
 * Confidential and Proprietary
 *
 * @file GReportHeader.h
 * @author Guillermo Paris
 * @date May 7 2013
 * @copyright © 2013 Gilbarco Inc. Confidential and Proprietary
 *
 */

#ifndef GREPORTHEADER_H_
#define GREPORTHEADER_H_

const int HEADER_MAX_LINES = 11;
const int REPO_MAX_COLUMNS = 32;
const int HEADER_MAX_CHARS = HEADER_MAX_LINES * REPO_MAX_COLUMNS;

class GReportHeader
{
public:
	enum eReportType
	{
		eHardwareReport,
		eTestResultsReport,
		eReportTypeQty // number of different types of reports
	};

	              GReportHeader();
	              ~GReportHeader();

	inline int    GetLength()  { return m_iHeaderLength; }
	       int    DumpText(eReportType eRepoType, char* pcBuffer, int iMaxChars);

	static const int    mst_iMaxLineLength; // = REPO_MAX_COLUMNS + 1

private:
	char*         GetSPOTSerialNo();
	const  char*  GetSPOTType();
	const  char*  GetSPOTIPAddr();
	inline bool   NoMoreRoomOnBuffer();
	       int    PopulateLine(int nLine, ...);

	static const int    mst_iLineCount; // = HEADER_MAX_LINES
	static const char*  mst_arypcHeaderLines[HEADER_MAX_LINES];
	static const char*  mst_arypcVgdHeaderLines[HEADER_MAX_LINES];

	int                 m_iHeaderLength;
	int                 m_iMaxBufferLength;
	char*               m_pcOut;
};

inline bool GReportHeader::NoMoreRoomOnBuffer()
{
	return ( m_iHeaderLength + mst_iMaxLineLength >= m_iMaxBufferLength );
}

#endif /* GREPORTHEADER_H_ */

