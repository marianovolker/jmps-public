/*
 * GHWInfoReport.h
 * © 2013 Gilbarco Inc.
 * Confidential and Proprietary
 *
 * @file GHWInfoReport.h
 * @author Guillermo Paris
 * @date May 7 2013
 * @copyright © 2013 Gilbarco Inc. Confidential and Proprietary
 *
 */

#ifndef HWINFOREPORT_H_
#define HWINFOREPORT_H_

#include "Reports/GAbsPrinterReport.h"

const int HARDWARE_REPORT_MAX_LINES = 67;

class GHWInfoReport: public GAbsPrinterReport
{
	public:

		GHWInfoReport();
		~GHWInfoReport();

		virtual inline bool OpenReport();
		virtual inline int  TextDump();


	private:

		int TextDump(GReportHeader::eReportType eType);

		static const char* mst_arypcTextLines[HARDWARE_REPORT_MAX_LINES];
};


inline bool GHWInfoReport::OpenReport()
{
	return GAbsPrinterReport::OpenReport(HARDWARE_REPORT_MAX_LINES , mst_arypcTextLines);
}

inline int GHWInfoReport::TextDump()
{
	return this->TextDump(GReportHeader::eHardwareReport);
}


#endif /* HWINFOREPORT_H_ */
