
/**
 * GTestInfoReportUT.h
 *
 * © 2013 Gilbarco Inc.
 * Confidential and Proprietary
 *
 * @file GTestInfoReport.h
 * @author Guillermo Paris
 * @date Jun 10 2013
 * @copyright © 2013 Gilbarco Inc. Confidential and Proprietary
 *
 */

#ifndef GTESTINFOREPORTUT_H_
#define GTESTINFOREPORTUT_H_

#include <string>
#include <gtest/gtest.h>
#include "Reports/TestInfo/GTestInfoReport.h"
#include "Services/Display/GDisplayService.h"
//std::string GTestInfoReport::GetFailingScreens(std::string & strHeaderTitle)
class GUnitTestedInfoReport : public GTestInfoReport
{
public:
	GUnitTestedInfoReport();
	~GUnitTestedInfoReport();

	std::string GetBadScreens(std::string & strHeaderTitle);
};

class GTestInfoReportUT: public testing::Test
{
public:
	GTestInfoReportUT();
	~GTestInfoReportUT();

	void   SetUp();
	void   TearDown();
	void   SetValues(unsigned int uWhite, unsigned int uBlack,
	                 unsigned int uRed, unsigned int uGreen, unsigned int uBlue);

protected:

	GUnitTestedInfoReport*      m_pReport;
	GDisplayServiceStatistics*  m_pDYStatistics;
	std::string                 m_strWantedHeader;
	std::string                 m_strWantedValues;

};

#endif /* GTESTINFOREPORTUT_H_ */
