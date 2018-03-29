/**
 * GTestInfoReport.h
 *
 * © 2013 Gilbarco Inc.
 * Confidential and Proprietary
 *
 * @file GTestInfoReport.h
 * @author Guillermo Paris
 * @date May 7 2013
 * @copyright © 2013 Gilbarco Inc. Confidential and Proprietary
 *
 */

#ifndef GTESTINFOREPORT_H_
#define GTESTINFOREPORT_H_

#include "Reports/GAbsPrinterReport.h"
#include "Services/CardReader/GCardReaderService.h"
#include "Services/Keypad/GKeypadService.h"


const int TESTREPO_MAX_LINES = 51;


class GTestInfoReport : public GAbsPrinterReport
{
	public:
		                     GTestInfoReport();
		virtual              ~GTestInfoReport();

		virtual inline bool  OpenReport();
		virtual        int   TextDump();

	protected:
		std::string          GetFailingScreenTable(std::string & sHeader);

	private:

		int                  TextDump(GReportHeader::eReportType  eType);
		inline std::string   GetNumKeysString();
		       std::string   GetCtrlKeysString();
		inline std::string   GetSoftKeysString();
		inline std::string   GetAuxiliaryKeysString();
		       std::string   GetKeysString(GKeypadService::eKeyValue e1, GKeypadService::eKeyValue e2);

		static const char*   mst_arypcTextLines[TESTREPO_MAX_LINES];
};


inline bool GTestInfoReport::OpenReport()
{
	return GAbsPrinterReport::OpenReport(TESTREPO_MAX_LINES, mst_arypcTextLines);
}

inline std::string GTestInfoReport::GetNumKeysString()
{
	return GetKeysString(GKeypadService::eKey00, GKeypadService::eKey09);
}

inline std::string GTestInfoReport::GetSoftKeysString()
{
	return GetKeysString(GKeypadService::eKeyL1, GKeypadService::eKeyR4);
}

inline std::string GTestInfoReport::GetAuxiliaryKeysString()
{
	return GetKeysString(GKeypadService::eKeyP1, GKeypadService::eKeyP10);
}


#endif /* GTESTINFOREPORT_H_ */

