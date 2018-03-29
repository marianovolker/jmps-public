#include <cstdio>
#include <cstring>
#include "GContactlessReaderMenu.h"
#include "GContactlessReaderManager.h"
#include "SPOT/SystemInformation/GGlobalSystemInformation.h"
#include "Services/ContactlessReader/GContactlessReaderService.h"
#include "Services/ContactlessReader/GContactlessReaderServiceStatistics.h"

const int GContactlessReaderMenu::CLESS_MAX_LINE_LEN = 33;

GContactlessReaderMenu::GContactlessReaderMenu() :
				m_pCLRManager(NULL)
{
	m_iWinID = eWinId_ContactlessMenu;

	//TODO: Eliminate this when repository is working OK
	GetTagStrings(m_MenuLabelsMap);
}

GContactlessReaderMenu::~GContactlessReaderMenu()
{
}

bool GContactlessReaderMenu::DrawMenu()
{
	return GAbsMenu::DrawMenu();
}

bool GContactlessReaderMenu::HandleSPOTMessage(GSPOTMessage* pMsg)
{
	// this menu only shows different tag segments on SPOT screen
	return true;
}

void GContactlessReaderMenu::Initialize()
{
	m_pCLRManager = new GContactlessReaderManager(this);
	m_pCLRManager->Initialize();
}

void GContactlessReaderMenu::ClearScreen()
{
	const char* arypcLabels[8];

	memset(arypcLabels, 0, sizeof(arypcLabels));

	arypcLabels[0] = "";
	arypcLabels[1] = "";
	arypcLabels[2] = "";
	arypcLabels[3] = "";
	arypcLabels[4] = "";
	arypcLabels[5] = "";
	arypcLabels[6] = "";

	UpdateUserLabels(7, arypcLabels);
}

void GContactlessReaderMenu::ShowResultsScreen(char* pcResult)
{
	const char* arypcLabels[8];

	SetLabel(GSoftKeysWindowTemplate::eSOFT_L3, " Show test\n Statistics"); //hide the label for stats
	memset(arypcLabels, 0, sizeof(arypcLabels));

	OTI_INFO& rOtiInfo = GGlobalSystemInformation::GetInstance()->GetOTIInfo();

	if (rOtiInfo.m_u8Status == 0x00)
	{ //no OTI reader connected

		arypcLabels[0] = "";
		arypcLabels[1] = "Contactless reader NOT operational ";
		arypcLabels[2] = "";
		arypcLabels[3] = "";
		arypcLabels[4] = "";
		arypcLabels[5] = "";
		arypcLabels[6] = "";

	}
	else
	{
		arypcLabels[0] = "";
		arypcLabels[1] = pcResult;
		arypcLabels[2] = "";
		arypcLabels[3] = "";
		arypcLabels[4] = "  Present SPOT Check contactless diagnostic ";
		arypcLabels[5] = "test card in front of reader.";
		arypcLabels[6] = "";
	}

	UpdateUserLabels(7, arypcLabels);
}

void GContactlessReaderMenu::ShowStatsScreen()
{
	const char* arypcLabels[8];

	SetLabel(GSoftKeysWindowTemplate::eSOFT_L3, " "); //hide the label for stats

	memset(arypcLabels, 0, sizeof(arypcLabels));

	UINT32 u32Pass = GContactlessReaderService::GetService()->GetSessionStatistics().GetStatistics(GContactlessReaderServiceStatistics::ePass);
	UINT32 u32Fail = GContactlessReaderService::GetService()->GetSessionStatistics().GetStatistics(GContactlessReaderServiceStatistics::eFail);

	UINT32 u32Total = GContactlessReaderService::GetService()->GetSessionStatistics().GetTotalWaves();
	char cPass[CLESS_MAX_LINE_LEN];
	char cFail[CLESS_MAX_LINE_LEN];
	char cTotalSwipes[CLESS_MAX_LINE_LEN];

	memset(cPass, 0, sizeof(cPass));
	memset(cFail, 0, sizeof(cPass));
	memset(cTotalSwipes, 0, sizeof(cPass));

	const char* pcTemplatePass = "PASS          %i";
	const char* pcTemplateFail = "FAIL          %i";
	const char* pcTemplateTotal = "Total Tests:  %i";

	snprintf(cPass, CLESS_MAX_LINE_LEN, pcTemplatePass, u32Pass);
	snprintf(cFail, CLESS_MAX_LINE_LEN, pcTemplateFail, u32Fail);
	snprintf(cTotalSwipes, CLESS_MAX_LINE_LEN, pcTemplateTotal, u32Total);

	arypcLabels[0] = "Contactless Reader Statistics";
	arypcLabels[1] = cPass;
	arypcLabels[2] = cFail;
	arypcLabels[3] = "";
	arypcLabels[4] = cTotalSwipes;
	arypcLabels[5] = "";
	arypcLabels[6] = "";

	UpdateUserLabels(7, arypcLabels);
}

void GContactlessReaderMenu::HandleSoftKeyEvent(UINT8 KeyCode)
{
	switch (KeyCode)
	{
		case eL1:  // L1
			m_pCLRManager->ProcessEventReceived(GContactlessReaderManager::eEventSoftKeyL1);
		break;

		case eL2:  // L2
			m_pCLRManager->ProcessEventReceived(GContactlessReaderManager::eEventSoftKeyL2);
		break;

		case eL3:  // L3
			m_pCLRManager->ProcessEventReceived(GContactlessReaderManager::eEventSoftKeyL3);
		break;

		case eL4:  // L4
			m_pCLRManager->ProcessEventReceived(GContactlessReaderManager::eEventSoftKeyL4);
		break;

		case eR1:  // R1
			m_pCLRManager->ProcessEventReceived(GContactlessReaderManager::eEventSoftKeyR1);
		break;

		case eR2:  // R2
			m_pCLRManager->ProcessEventReceived(GContactlessReaderManager::eEventSoftKeyR2);
		break;

		case eR3:  // R3
			m_pCLRManager->ProcessEventReceived(GContactlessReaderManager::eEventSoftKeyR3);
		break;

		case eR4:  // R4
			m_pCLRManager->ProcessEventReceived(GContactlessReaderManager::eEventSoftKeyR4);
		break;
	}

	EnableSoftKeys();
}

void GContactlessReaderMenu::Quit()
{
	m_pManager->QueueMenuPop();
	EnableSoftKeys();
}


/**
 *
 * STATIC METHODS
 *
 */

int GContactlessReaderMenu::GetTagStrings(GSoftKeysWindowTemplate::tStringTagMap & rMap)
{
	// initialize map of tag-id/label pairs for initial window creation
	rMap[GSoftKeysWindowTemplate::eSOFT_L3] = std::string(" Show test\n Statistics");
	rMap[GSoftKeysWindowTemplate::eSOFT_L4] = std::string(" Previous\n Menu");

	rMap[GSoftKeysWindowTemplate::eSEGMENT_1] = std::string("Contactless Reader Diagnostics");

	OTI_INFO&   rOtiInfo = GGlobalSystemInformation::GetInstance()->GetOTIInfo();

	if (rOtiInfo.m_u8Status == 0x00)
	{ //no OTI reader connected

		rMap[GSoftKeysWindowTemplate::eSEGMENT_3] = std::string("Contactless reader NOT operational ");
	}
	else //there is an OTI reader connected
	{
		rMap[GSoftKeysWindowTemplate::eSEGMENT_2] = std::string("Contactless reader operational ");

		switch (rOtiInfo.m_u8Status)
		{
			case 0x01:
				rMap[GSoftKeysWindowTemplate::eSEGMENT_3] = std::string("Status:  Startup  ");
			break;
			case 0x02:
				rMap[GSoftKeysWindowTemplate::eSEGMENT_3] = std::string("Status:  Error  ");
			break;
			case 0x03:
				rMap[GSoftKeysWindowTemplate::eSEGMENT_3] = std::string("Status:  Disabled ");
			break;

			case 0x04:
				rMap[GSoftKeysWindowTemplate::eSEGMENT_3] = std::string("Status:  Enabled  ");
			break;
			case 0x06:
				rMap[GSoftKeysWindowTemplate::eSEGMENT_3] = std::string("Status:  Fatal error  ");
			break;
			case 0x07:
				rMap[GSoftKeysWindowTemplate::eSEGMENT_3] = std::string("Status:  download firmware  ");
			break;

			default:
				rMap[GSoftKeysWindowTemplate::eSEGMENT_3] = std::string("Status:  unknown");
			break;
		}

		rMap[GSoftKeysWindowTemplate::eSEGMENT_4] = std::string(" ");
		rMap[GSoftKeysWindowTemplate::eSEGMENT_6] = std::string("  Present SPOT Check contactless diagnostic ");
		rMap[GSoftKeysWindowTemplate::eSEGMENT_7] = std::string("test card in front of reader.");
	}

	return 8;
}

