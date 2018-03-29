/**
 * GTestInfoReport.cpp
 *
 *  Created on: May 07 2013
 *      Author: G.Paris
 */
#include <cstdio>
#include <cstring>
#include <string>
#include <exception>
#include "GTestInfoReport.h"
#include "Common/Util/DEBUGOUT.h"
#include "Services/Keypad/GKeypadService.h"
#include "Services/Display/GDisplayService.h"
#include "Services/ContactlessReader/GContactlessReaderService.h"
#include "Services/ContactlessReader/GContactlessReaderServiceStatistics.h"

const char* GTestInfoReport::mst_arypcTextLines[TESTREPO_MAX_LINES] =
{
	"-------------------------------\n", // line  0
	"Card Reader Statistics\n",          // line  1
	"\n",                                // line  2
	"       Tk1   Tk2   Tk3   Chip\n",   // line  3
	"Pass %5d %5d %5d  %5d\n",           // line  4
	"Fail %5d %5d %5d  %5d\n",           // line  5
	"N/P  %5d %5d %5d  %5d\n",           // line  6
	"\n",                                // line  7
	"   Total Swipes: %6d\n",            // line  8
	"\n",                                // line  9
	"-------------------------------\n", // line 10
	"Contactless Reader Statistics\n",   // line 11
	"\n",                                // line 12
	"Pass %7d\n",                        // line 13
	"Fail %7d\n",                        // line 14
	"\n",                                // line 15
	"    Total Tests: %7d\n",            // line 16
	"\n",                                // line 17
	"-------------------------------\n", // line 18
	"Keyboard's Statistics\n",           // line 19
	"\n",                                // line 20
	"Key pressed in this session:\n",    // line 21
	"\n",                                // line 22
	"Number keys:\n",                    // line 23
	"%s\n",                              // line 24
	"\n",                                // line 25
	"Control keys:\n",                   // line 26
	"%s\n",                              // line 27
	"\n",                                // line 28
	"Soft keys:\n",                      // line 29
	"%s\n",                              // line 30
	"\n",                                // line 31
	"Auxiliary keys:\n",                 // line 32
	"%s\n",                              // line 33
	"\n",                                // line 34
	"-------------------------------\n", // line 35
	"Display Statistics\n",              // line 36
	"\n",                                // line 37
	"White :  PASS %d   FAIL %d\n",      // line 38
	"Black :  PASS %d   FAIL %d\n",      // line 39
	"Red   :  PASS %d   FAIL %d\n",      // line 40
	"Green :  PASS %d   FAIL %d\n",      // line 41
	"Blue  :  PASS %d   FAIL %d\n",      // line 42
	"\n",                                // line 43
	"Summary:\n",                        // line 44
	"-------\n",                         // line 45
	"PASS %d, FAIL %d, TOTAL %d\n",      // line 46
	"\n",                                // line 47
	"-------------------------------\n", // line 48
	"\n",                                // line 49
	"%c%c\n",                            // line 50
};

GTestInfoReport::GTestInfoReport()
{

}

GTestInfoReport::~GTestInfoReport()
{
}

int GTestInfoReport::TextDump()
{
	return this->TextDump(GReportHeader::eTestResultsReport);
}

int GTestInfoReport::TextDump(GReportHeader::eReportType eType)
{
	GCardReaderService* pCRSvc = GCardReaderService::GetService();
	GDisplayService* pDYSvc = GDisplayService::GetService();
	const GCardReaderServiceStatistics & rCRStats = pCRSvc->GetSessionStatistics();
	const GDisplayServiceStatistics & rDYStats = pDYSvc->GetStatistics();

	int itk1good = rCRStats.GetStatistics(GCardReaderServiceStatistics::ePass,
	                GCardReaderServiceStatistics::eTrack1);
	int itk2good = rCRStats.GetStatistics(GCardReaderServiceStatistics::ePass,
	                GCardReaderServiceStatistics::eTrack2);
	int itk3good = rCRStats.GetStatistics(GCardReaderServiceStatistics::ePass,
	                GCardReaderServiceStatistics::eTrack3);
	int ichipgood = rCRStats.GetStatistics(GCardReaderServiceStatistics::ePass,
	                GCardReaderServiceStatistics::eChip);

	int itk1bad = rCRStats.GetStatistics(GCardReaderServiceStatistics::eFail,
	                GCardReaderServiceStatistics::eTrack1);
	int itk2bad = rCRStats.GetStatistics(GCardReaderServiceStatistics::eFail,
	                GCardReaderServiceStatistics::eTrack2);
	int itk3bad = rCRStats.GetStatistics(GCardReaderServiceStatistics::eFail,
	                GCardReaderServiceStatistics::eTrack3);
	int ichipbad = rCRStats.GetStatistics(GCardReaderServiceStatistics::eFail,
	                GCardReaderServiceStatistics::eChip);

	int itk1np = rCRStats.GetStatistics(GCardReaderServiceStatistics::eNotPresent,
	                GCardReaderServiceStatistics::eTrack1);
	int itk2np = rCRStats.GetStatistics(GCardReaderServiceStatistics::eNotPresent,
	                GCardReaderServiceStatistics::eTrack2);
	int itk3np = rCRStats.GetStatistics(GCardReaderServiceStatistics::eNotPresent,
	                GCardReaderServiceStatistics::eTrack3);
	int ichipnp = rCRStats.GetStatistics(GCardReaderServiceStatistics::eNotPresent,
	                GCardReaderServiceStatistics::eChip);

	int itotalswipes = rCRStats.GetTotalSwipes();
	int iclessgood = 0;
	int iclessbad = 0;
	int iclesstot = 0;
	int nLine = 0;

	std::string sKeys;  // "0, 1, 2, 3, 4, 5, 6, 7, 8, 9"

	int iWhiteOk = rDYStats.GetStatistics(GDisplayServiceStatistics::ePass,
	                GDisplayServiceStatistics::eTest1_White_Color);
	int iBlackOk = rDYStats.GetStatistics(GDisplayServiceStatistics::ePass,
	                GDisplayServiceStatistics::eTest2_Black_Color);
	int iRedOk = rDYStats.GetStatistics(GDisplayServiceStatistics::ePass,
	                GDisplayServiceStatistics::eTest3_Red_Color);
	int iGreenOk = rDYStats.GetStatistics(GDisplayServiceStatistics::ePass,
	                GDisplayServiceStatistics::eTest4_Green_Color);
	int iBlueOk = rDYStats.GetStatistics(GDisplayServiceStatistics::ePass,
	                GDisplayServiceStatistics::eTest5_Blue_Color);

	int iWhiteBad = rDYStats.GetStatistics(GDisplayServiceStatistics::eFail,
	                GDisplayServiceStatistics::eTest1_White_Color);
	int iBlackBad = rDYStats.GetStatistics(GDisplayServiceStatistics::eFail,
	                GDisplayServiceStatistics::eTest2_Black_Color);
	int iRedBad = rDYStats.GetStatistics(GDisplayServiceStatistics::eFail,
	                GDisplayServiceStatistics::eTest3_Red_Color);
	int iGreenBad = rDYStats.GetStatistics(GDisplayServiceStatistics::eFail,
	                GDisplayServiceStatistics::eTest4_Green_Color);
	int iBlueBad = rDYStats.GetStatistics(GDisplayServiceStatistics::eFail,
	                GDisplayServiceStatistics::eTest5_Blue_Color);
	int iTotalScreens = rDYStats.GetTotalTests();

	DumpHeaderText(eType);
	if (NoMoreRoomOnBuffer())
	    return GetPartialCount(); // no more room for another text line

	while (nLine < 4)  // lines 0 to 3
	{
		PutVoidDataLine(nLine++);
		if (NoMoreRoomOnBuffer())
		    return GetPartialCount(); // no more room for another text line
	}

	PopulateLine(nLine++, itk1good, itk2good, itk3good, ichipgood); // line 4
	if (NoMoreRoomOnBuffer())
	    return GetPartialCount();

	PopulateLine(nLine++, itk1bad, itk2bad, itk3bad, ichipbad); // line 5
	if (NoMoreRoomOnBuffer())
	    return GetPartialCount();

	PopulateLine(nLine++, itk1np, itk2np, itk3np, ichipnp); // line 6
	if (NoMoreRoomOnBuffer())
	    return GetPartialCount();

	PutVoidDataLine(nLine++); // line 7
	if (NoMoreRoomOnBuffer())
	    return GetPartialCount();

	PopulateLine(nLine++, itotalswipes); // line 8
	if (NoMoreRoomOnBuffer())
	    return GetPartialCount();

	while (nLine < 13)  // lines 9 to 12
	{
		PutVoidDataLine(nLine++);
		if (NoMoreRoomOnBuffer())
		    return GetPartialCount(); // no more room for another text line
	}

	// Contactless

	UINT32 u32Pass = GContactlessReaderService::GetService()->GetSessionStatistics().GetStatistics(GContactlessReaderServiceStatistics::ePass);
	UINT32 u32Fail = GContactlessReaderService::GetService()->GetSessionStatistics().GetStatistics(GContactlessReaderServiceStatistics::eFail);
	UINT32 u32Total = GContactlessReaderService::GetService()->GetSessionStatistics().GetTotalWaves();

	PopulateLine(nLine++, u32Pass); // line 13
	PopulateLine(nLine++, u32Fail); // line 14

	PutVoidDataLine(nLine++); // line 15
	if (NoMoreRoomOnBuffer())
		return GetPartialCount();

	PopulateLine(nLine++, u32Total); // line 16

	while (nLine < 24)  // lines 17 to 23
	{
		PutVoidDataLine(nLine++);
		if (NoMoreRoomOnBuffer())
			return GetPartialCount();
	}

	// Fill numbers
	sKeys = GetNumKeysString(); // line 24
	PopulateLine(nLine++, sKeys.c_str());
	if (NoMoreRoomOnBuffer(17)) // no more room for 17 chars
		return GetPartialCount();

	PutVoidDataLine(nLine++); // line 25
	PutVoidDataLine(nLine++); // line 26

	// Fill control keys
	sKeys = GetCtrlKeysString(); // line 27
	PopulateLine(nLine++, sKeys.c_str());
	if (NoMoreRoomOnBuffer(14)) // no more room for 14 chars
		return GetPartialCount();

	PutVoidDataLine(nLine++); // line 28
	PutVoidDataLine(nLine++); // line 29

	// Fill soft keys
	sKeys = GetSoftKeysString(); // line 30
	PopulateLine(nLine++, sKeys.c_str());
	if (NoMoreRoomOnBuffer(19)) // no more room for 19 chars
		return GetPartialCount();

	PutVoidDataLine(nLine++); // line 31
	PutVoidDataLine(nLine++); // line 32

	// Fill auxiliary keys
	sKeys = GetAuxiliaryKeysString(); // line 33
	PopulateLine(nLine++, sKeys.c_str());
	if (NoMoreRoomOnBuffer(64))
		return GetPartialCount();

	while (nLine < 38)  // lines 34 to 37
		PutVoidDataLine(nLine++);

	PopulateLine(nLine++, iWhiteOk, iWhiteBad); // line 38
	PopulateLine(nLine++, iBlackOk, iBlackBad); // line 39
	PopulateLine(nLine++, iRedOk, iRedBad);     // line 40
	PopulateLine(nLine++, iGreenOk, iGreenBad); // line 41
	PopulateLine(nLine++, iBlueOk, iBlueBad);   // line 42

	if (NoMoreRoomOnBuffer(7 * GetMaxCharPerLine()))
		return GetPartialCount(); // no more room for another 7 text lines

	while (nLine < 46)  // lines 43 to 45
		PutVoidDataLine(nLine++);

	PopulateLine(nLine++, iWhiteOk + iBlackOk + iRedOk + iGreenOk + iBlueOk,
	             iWhiteBad + iBlackBad + iRedBad + iGreenBad + iBlueBad,
	             iTotalScreens);  // line 46

	if (NoMoreRoomOnBuffer(40))
		return GetPartialCount();

	while (nLine < 50)  // lines 47 to 49
		PutVoidDataLine(nLine++);

	PopulateLine(nLine, 0x1B, 'E'); // line 50: paper advance escape character
	return GetPartialCount();
}

std::string GTestInfoReport::GetCtrlKeysString()
{
	std::string    s( GetKeysString(GKeypadService::eKeyHelp, GKeypadService::eKeyNo) );
	std::string sYes( GetKeysString(GKeypadService::eKeyYes, GKeypadService::eKeyYes) );

	if( sYes.empty() )
		return s;

	s += ", ";
	s += sYes;
	return s;
}

std::string GTestInfoReport::GetKeysString(GKeypadService::eKeyValue e1, GKeypadService::eKeyValue e2)
{
	std::string strOutput;
	GKeypadService* m_pKPService = GKeypadService::GetService();
	if (m_pKPService == NULL || e1 > e2)
		return "";

	// Fill string to return
	for(int i = e1; i <= e2; ++i)
	{
		//get number of keystrokes for the key
		UINT8 u8times = m_pKPService->GetKeyStatistics(UINT8(i));

		// add the label of the key to the returning string.
		if (u8times != 0xFF && u8times > 0)
		{
			strOutput += m_pKPService->GetKeyLabel(UINT8(i));
			strOutput += ", ";
		}
	}

	if(strOutput.empty())
		return strOutput;

	//extract last comma and space
	try
	{
		strOutput = strOutput.substr(0, (strOutput.length() - 2));
	}
	catch (exception & e)
	{
		LOG(eReport, eError, "GTestInfoReport::GetKeysString() - ERROR creating labels for statistics.\n %s\n", e.what());
	}

	return strOutput;
}

std::string GTestInfoReport::GetFailingScreenTable(std::string & strHeaderTitle)
{
	std::string strNumericValues("");
	GDisplayService* m_pDYService = GDisplayService::GetService();
	if (m_pDYService == NULL)
	    return strNumericValues;

	// initial test number
	GDisplayServiceStatistics::eTests e0 = GDisplayServiceStatistics::eTest1_White_Color;
	GDisplayServiceStatistics::eTests etest = e0;
	GDisplayServiceStatistics dss = m_pDYService->GetStatistics();
	char arycNumValue[16];
	bool blFailReported = false;

	memset(arycNumValue, 0, 16);

	for (int i = e0; i < GDisplayServiceStatistics::eTotalTests; i++)
	{
		etest = GDisplayServiceStatistics::eTests(i);
		int nFails = dss.GetStatistics(GDisplayServiceStatistics::eFail, etest);
		if (nFails == 0)
		    continue;

		if (!blFailReported) // first fail detected - reset the header string
		{
			strHeaderTitle.clear();
			blFailReported = true;
		}

		const char* pcTitleWord = dss.GetTestLabel(etest);
		int lenTitle = strlen(pcTitleWord);
		int l = 0;
		while (l < lenTitle - 3) // length minus 3 space padding
			arycNumValue[l++] = ' ';

		strHeaderTitle += pcTitleWord;
		strHeaderTitle += "  ";
		snprintf(arycNumValue + l, 15, "%3d  ", nFails);
		strNumericValues += arycNumValue;
	}

	return strNumericValues;
}

