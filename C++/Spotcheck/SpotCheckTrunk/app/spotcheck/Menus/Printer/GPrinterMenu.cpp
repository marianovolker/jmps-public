#include <string.h>
#include <stdio.h>
#include "GPrinterMenu.h"
#include "GPrinterManager.h"
#include "SPOT/SystemInformation/GGlobalSystemInformation.h"
#include "Menus/GMenuManager.h"

const char* GPrinterMenu::mst_pcPassed = "PASSED";
const char* GPrinterMenu::mst_pcFail   = "FAIL";

GPrinterMenu::GPrinterMenu(void)
	: m_pPRManager(NULL)
{
	m_iWinID = eWinId_PrinterMenu;

	//TODO: Eliminate this when repository is working OK
	GetTagStrings(m_MenuLabelsMap);
}

GPrinterMenu::~GPrinterMenu(void)
{
	if(m_pPRManager)
	{
		m_pPRManager->Dispose();
		delete m_pPRManager;
		m_pPRManager = NULL;
	}
}

void GPrinterMenu::Initialize()
{
	m_pPRManager = new GPrinterManager(this);
	m_pPRManager->Initialize();
}

void GPrinterMenu::NotifyPrintResult(const char* pcResult)
{
	if(!pcResult)
		return; // nothing to do

	const char* arypcLabels[5];

	memset( arypcLabels, 0, sizeof(arypcLabels) );
	arypcLabels[3] = pcResult;
	UpdateUserLabels(4, arypcLabels);
	EnableSoftKeys(); // enable soft-key notification from the session.
}

void GPrinterMenu::NotifyPrinterStatus(const char* pcStatus)
{
	if(!pcStatus)
		return; // nothing to do

	const char* arypcLabels[7];

	memset( arypcLabels, 0, sizeof(arypcLabels) );
	arypcLabels[5] = pcStatus;
	UpdateUserLabels(6, arypcLabels);
}

bool GPrinterMenu::DrawMenu()
{
	return GAbsMenu::DrawMenu();
}

bool GPrinterMenu::HandleSPOTMessage(GSPOTMessage* pMsg)
{
	// this menu only shows different tag segments on SPOT screen
	return true;
}

void GPrinterMenu::HandleSoftKeyEvent(UINT8 KeyCode)
{
	const char* arypcLabels[5];

	memset( arypcLabels, 0, sizeof(arypcLabels) );
	arypcLabels[3] = "Printing ...";

	switch(KeyCode)
	{
		case eL4:  // L4 - back to previous menu
			m_pManager->QueueMenuPop();
			return;

		case eL1:  // L1
			UpdateUserLabels(4, arypcLabels);
			m_pPRManager->PrintHardwareReportRequest();
			break; // soft-keys will remain disabled until the end of printout

		case eL2:  // L2
			UpdateUserLabels(4, arypcLabels);
			m_pPRManager->PrintTestResultsReportRequest();
			break; // soft-keys will remain disabled until the end of printout

		case eL3:  // L3
		case eR1:  // R1
		case eR2:  // R2
		case eR3:  // R3
		case eR4:  // R4
		default:
			EnableSoftKeys();
			break;
	}
}


/**
 *
 * STATIC METHODS
 *
 */

int GPrinterMenu::GetTagStrings(GSoftKeysWindowTemplate::tStringTagMap & rMap)
{
	// Initialize map of tag-id/label pairs for initial window creation
	PR_HW_INFO& rPRInfo = GGlobalSystemInformation::GetInstance()->GetPrinterInfo();
	char arycBuffer[32];

	// initialize map of tag-id/label pairs (maps segments to text for those segments)
	rMap[GSoftKeysWindowTemplate::eSOFT_L1] = std::string(" Sys. Health\n Report");
	rMap[GSoftKeysWindowTemplate::eSOFT_L2] = std::string(" Test\n Report");
	rMap[GSoftKeysWindowTemplate::eSOFT_L4] = std::string(" Previous\n Menu");

	rMap[GSoftKeysWindowTemplate::eSEGMENT_1] = std::string("Printer test and report printouts");

	snprintf(arycBuffer, sizeof(arycBuffer), "Device Type:  %02X", int(rPRInfo.m_u8DeviceType));
	rMap[GSoftKeysWindowTemplate::eSEGMENT_2] = std::string(arycBuffer);

	snprintf(arycBuffer, sizeof(arycBuffer), "Device Model:  %s",rPRInfo.m_pcDevModel );
	rMap[GSoftKeysWindowTemplate::eSEGMENT_3] = std::string(arycBuffer);

	snprintf(arycBuffer, sizeof(arycBuffer), "S/N:  %s", rPRInfo.m_pcSerialNumber);
	rMap[GSoftKeysWindowTemplate::eSEGMENT_4] = std::string(arycBuffer);

	return 7;
}

