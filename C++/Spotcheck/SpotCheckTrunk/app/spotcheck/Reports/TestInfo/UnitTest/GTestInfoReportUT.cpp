/*
 * GTestInfoReportUT.cpp
 *
 *  Created on: 10/06/2013
 *      Author: gparis
 */

#include "GTestInfoReportUT.h"

// I really really really hate long names !!
typedef GDisplayServiceStatistics::eTypeStatistics   eTypeStat;
typedef GDisplayServiceStatistics::eTests            eTestNo;

GUnitTestedInfoReport::GUnitTestedInfoReport()
{
}

GUnitTestedInfoReport::~GUnitTestedInfoReport()
{
}

std::string GUnitTestedInfoReport::GetBadScreens(std::string & strHeaderTitle)
{
	return GetFailingScreenTable(strHeaderTitle);
}



GTestInfoReportUT::GTestInfoReportUT()
	: m_pReport(NULL), m_pDYStatistics(NULL),
	  m_strWantedHeader("No problems detected.  "),
	  m_strWantedValues("")
{
}

GTestInfoReportUT::~GTestInfoReportUT()
{
}

void GTestInfoReportUT::SetUp()
{
	GDisplayService* pDYService = GDisplayService::CreateInstance(NULL);

	m_pReport = new GUnitTestedInfoReport();

	if(m_pReport == NULL)
		return;

	if(pDYService == NULL)
		return;

	m_pDYStatistics = & pDYService->GetStatistics();
}

void GTestInfoReportUT::TearDown()
{
	delete m_pReport;
	m_pReport = NULL;
}

void GTestInfoReportUT::SetValues(unsigned int uWhite, unsigned int uBlack,
                                  unsigned int uRed, unsigned int uGreen,
                                  unsigned int uBlue)
{
	// I really hate long long long names !!
	const eTypeStat eBad   = GDisplayServiceStatistics::eFail;
	const eTestNo   eWhite = GDisplayServiceStatistics::eTest1_White_Color;
	const eTestNo   eBlack = GDisplayServiceStatistics::eTest2_Black_Color;
	const eTestNo   eRed   = GDisplayServiceStatistics::eTest3_Red_Color;
	const eTestNo   eGreen = GDisplayServiceStatistics::eTest4_Green_Color;
	const eTestNo   eBlue  = GDisplayServiceStatistics::eTest5_Blue_Color;

	m_pDYStatistics->SetStatistics(eBad, eWhite, uWhite);
	m_pDYStatistics->SetStatistics(eBad, eBlack, uBlack);
	m_pDYStatistics->SetStatistics(eBad, eRed,   uRed);
	m_pDYStatistics->SetStatistics(eBad, eGreen, uGreen);
	m_pDYStatistics->SetStatistics(eBad, eBlue,  uBlue);
}

TEST_F(GTestInfoReportUT, TestFailingScreensList)
{
	EXPECT_FALSE(m_pReport == NULL);
	EXPECT_FALSE(m_pDYStatistics == NULL);

	std::string sHeader(m_strWantedHeader), sValues("");

	sValues = m_pReport->GetBadScreens(sHeader);
	// If statistics are still null then the proposed header must be untouched.
	EXPECT_EQ(sHeader, m_strWantedHeader);
	EXPECT_EQ(sValues, m_strWantedValues);

	m_strWantedHeader = "White  Black  Red  Green  Blue  ";
	m_strWantedValues = "    1     22  333    444     5  ";
	SetValues(1, 22, 333, 444, 5);
	sValues = m_pReport->GetBadScreens(sHeader);
	EXPECT_EQ(sHeader, m_strWantedHeader);
	EXPECT_EQ(sValues, m_strWantedValues);

	m_strWantedHeader = "White  Red  Blue  ";
	m_strWantedValues = "  111    2    33  ";
	SetValues(111, 0, 2, 0, 33);
	sValues = m_pReport->GetBadScreens(sHeader);
	EXPECT_EQ(sHeader, m_strWantedHeader);
	EXPECT_EQ(sValues, m_strWantedValues);
}

