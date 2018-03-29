#include <cstdio>
#include <cstring>
#include "GBarcodeScannerMenu.h"
#include "GBarcodeScannerManager.h"
#include "SPOT/SystemInformation/GGlobalSystemInformation.h"
#include "Services/BarcodeScanner/GBarcodeScannerService.h"
#include "Services/BarcodeScanner/GBarcodeScannerServiceStatistics.h"

// TODO: Remove this hack and populate this structure inside GlobalSystemInformation
//       object. The population data has got to be obtained from OPT SPOT message.

struct BARCODE_INFO
{
	UINT8 m_u8Status;

} oBarcodeInfo;

BARCODE_INFO& GetBarcodeInfo()
{
	return oBarcodeInfo;
}

const int GBarcodeScannerMenu::BCS_MAX_LINE_LEN = 33;

GBarcodeScannerMenu::GBarcodeScannerMenu() :
				m_pBCSManager(NULL)
{
	m_iWinID = eWinId_BarcodeScannerMenu;

	//TODO: Eliminate this when repository is working OK
	GetTagStrings(m_MenuLabelsMap);
}

GBarcodeScannerMenu::~GBarcodeScannerMenu()
{
}

bool GBarcodeScannerMenu::DrawMenu()
{
	return GAbsMenu::DrawMenu();
}

bool GBarcodeScannerMenu::HandleSPOTMessage(GSPOTMessage* pMsg)
{
	// this menu only shows different tag segments on SPOT screen
	return true;
}

void GBarcodeScannerMenu::Initialize()
{
	m_pBCSManager = new GBarcodeScannerManager(this);
	m_pBCSManager->Initialize();
}

void GBarcodeScannerMenu::ClearScreen()
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

void GBarcodeScannerMenu::ShowResultsScreen(char* pcResult)
{
	const char* arypcLabels[8];

	SetLabel(GSoftKeysWindowTemplate::eSOFT_L3, " Show test\n Statistics"); //hide the label for stats
	memset(arypcLabels, 0, sizeof(arypcLabels));

	OTI_INFO& rOtiInfo = GGlobalSystemInformation::GetInstance()->GetOTIInfo();

	// TODO: remove this, is to hack the service until is ready to report the real state.
	rOtiInfo.m_u8Status = 0xFF;


	if (rOtiInfo.m_u8Status == 0x00)
	{
		arypcLabels[0] = "";
		arypcLabels[1] = "Barcode scanner NOT operational ";
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
		arypcLabels[4] = "  Present barcode in front of scanner.";
		arypcLabels[5] = "";
		arypcLabels[6] = "";
	}

	UpdateUserLabels(7, arypcLabels);
}

void GBarcodeScannerMenu::ShowBarcodeSamplePrintingScreen()
{
	const char* arypcLabels[8];

	arypcLabels[0] = "";
	arypcLabels[1] = "Printing Barcode sample... ";
	arypcLabels[2] = "";
	arypcLabels[3] = "";
	arypcLabels[4] = "";
	arypcLabels[5] = "";
	arypcLabels[6] = "";

	UpdateUserLabels(7, arypcLabels);
}

void GBarcodeScannerMenu::ShowStatsScreen()
{
	const char* arypcLabels[8];

	SetLabel(GSoftKeysWindowTemplate::eSOFT_L3, " "); //hide the label for stats

	memset(arypcLabels, 0, sizeof(arypcLabels));

	UINT32 u32Pass = GBarcodeScannerService::GetService()->GetSessionStatistics().GetStatistics(GBarcodeScannerServiceStatistics::ePass);
	UINT32 u32Fail = GBarcodeScannerService::GetService()->GetSessionStatistics().GetStatistics(GBarcodeScannerServiceStatistics::eFail);

	UINT32 u32Total = GBarcodeScannerService::GetService()->GetSessionStatistics().GetTotalReadings();
	char cPass[BCS_MAX_LINE_LEN];
	char cFail[BCS_MAX_LINE_LEN];
	char cTotalSwipes[BCS_MAX_LINE_LEN];

	memset(cPass, 0, sizeof(cPass));
	memset(cFail, 0, sizeof(cPass));
	memset(cTotalSwipes, 0, sizeof(cPass));

	const char* pcTemplatePass = "PASS          %i";
	const char* pcTemplateFail = "FAIL          %i";
	const char* pcTemplateTotal = "Total Tests:  %i";

	snprintf(cPass, BCS_MAX_LINE_LEN, pcTemplatePass, u32Pass);
	snprintf(cFail, BCS_MAX_LINE_LEN, pcTemplateFail, u32Fail);
	snprintf(cTotalSwipes, BCS_MAX_LINE_LEN, pcTemplateTotal, u32Total);

	arypcLabels[0] = "Barcode scanner Statistics";
	arypcLabels[1] = cPass;
	arypcLabels[2] = cFail;
	arypcLabels[3] = "";
	arypcLabels[4] = cTotalSwipes;
	arypcLabels[5] = "";
	arypcLabels[6] = "";

	UpdateUserLabels(7, arypcLabels);
}

void GBarcodeScannerMenu::HandleSoftKeyEvent(UINT8 KeyCode)
{
	switch (KeyCode)
	{
		case eL1:  // L1
			m_pBCSManager->ProcessEventReceived(GBarcodeScannerManager::eEventSoftKeyL1);
		break;

		case eL2:  // L2
			m_pBCSManager->ProcessEventReceived(GBarcodeScannerManager::eEventSoftKeyL2);
		break;

		case eL3:  // L3
			m_pBCSManager->ProcessEventReceived(GBarcodeScannerManager::eEventSoftKeyL3);
		break;

		case eL4:  // L4
			m_pBCSManager->ProcessEventReceived(GBarcodeScannerManager::eEventSoftKeyL4);
		break;

		case eR1:  // R1
			m_pBCSManager->ProcessEventReceived(GBarcodeScannerManager::eEventSoftKeyR1);
		break;

		case eR2:  // R2
			m_pBCSManager->ProcessEventReceived(GBarcodeScannerManager::eEventSoftKeyR2);
		break;

		case eR3:  // R3
			m_pBCSManager->ProcessEventReceived(GBarcodeScannerManager::eEventSoftKeyR3);
		break;

		case eR4:  // R4
			m_pBCSManager->ProcessEventReceived(GBarcodeScannerManager::eEventSoftKeyR4);
		break;
	}

	EnableSoftKeys();
}

void GBarcodeScannerMenu::Quit()
{
	m_pManager->QueueMenuPop();
	EnableSoftKeys();
}


/**
 *
 * STATIC METHODS
 *
 */

int GBarcodeScannerMenu::GetTagStrings(GSoftKeysWindowTemplate::tStringTagMap & rMap)
{
	// initialize map of tag-id/label pairs for initial window creation
	rMap[GSoftKeysWindowTemplate::eSOFT_L1] = std::string(" Begin Test");
	rMap[GSoftKeysWindowTemplate::eSOFT_L2] = std::string(" Print\n Barcode");
	rMap[GSoftKeysWindowTemplate::eSOFT_L3] = std::string(" Show test\n Statistics");
	rMap[GSoftKeysWindowTemplate::eSOFT_L4] = std::string(" Previous\n Menu");

	rMap[GSoftKeysWindowTemplate::eSEGMENT_1] = std::string("Barcode Scanner Diagnostics");

	// TODO: Remove this, it is just to hack the service,
	//       until the real status is reported by SPOT application (OPT).
	BARCODE_INFO& rBarcodeInfo = GetBarcodeInfo();
	rBarcodeInfo.m_u8Status = 0x00;


	if (rBarcodeInfo.m_u8Status == 0x00)
	{ //no OTI reader connected

		rMap[GSoftKeysWindowTemplate::eSEGMENT_3] = std::string("Barcode scanner NOT operational ");
	}
	else //there is a reader connected
	{
		rMap[GSoftKeysWindowTemplate::eSEGMENT_2] = std::string("Barcode scanner operational ");

		switch (rBarcodeInfo.m_u8Status)
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
		rMap[GSoftKeysWindowTemplate::eSEGMENT_6] = std::string("  Present barcode in front of scanner.");
		rMap[GSoftKeysWindowTemplate::eSEGMENT_7] = std::string("");
	}

	return 10;
}
