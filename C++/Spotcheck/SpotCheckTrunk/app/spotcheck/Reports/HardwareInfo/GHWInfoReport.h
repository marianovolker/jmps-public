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
#include "SPOT/SystemInformation/GGlobalSystemInformation.h"

const int HARDWARE_REPORT_MAX_LINES = 67;
const int HARDWARE_REPORT_VGD_MAX_LINES = 64;

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
		static const char* mst_arypcTextLines_vgd[HARDWARE_REPORT_VGD_MAX_LINES];
};


inline bool GHWInfoReport::OpenReport()
{
	if( GGlobalSystemInformation::GetInstance()->isVGDUnit() )
	{
		return GAbsPrinterReport::OpenReport(HARDWARE_REPORT_MAX_LINES , mst_arypcTextLines_vgd);
	}
	else
	{
		return GAbsPrinterReport::OpenReport(HARDWARE_REPORT_MAX_LINES , mst_arypcTextLines);
	}
}

inline int GHWInfoReport::TextDump()
{
	return this->TextDump(GReportHeader::eHardwareReport);
}


#endif /* HWINFOREPORT_H_ */
