#include <cstdio>
#include "SPOT/SystemInformation/GGlobalSystemInformation.h"
#include "GDisplayMenu.h"
#include "Common/Util/DEBUGOUT.h"
#include "Services/Display/GDisplayService.h"

#include "GDisplayManager.h"

GDisplayMenu::GDisplayMenu(void):
    m_u16Width(0),
    m_u16Height(0),
    m_pDYManager(NULL)
{
	DY_HW_INFO& rDYInfo = GGlobalSystemInformation::GetInstance()->GetDisplayInfo();

	m_u16Width  = rDYInfo.m_DevHwrConfig.m_u16Xres;
	m_u16Height = rDYInfo.m_DevHwrConfig.m_u16Yres;
	m_iWinID    = eWinId_DisplayMenu;

	//TODO: Eliminate this when repository is working OK
	GetTagStrings(m_MenuLabelsMap);

	//TODO: Uncomment this if not displaying the menu!!!
	//DrawMenu();
}

GDisplayMenu::~GDisplayMenu(void)
{
	if(m_pDYManager)
	{
		m_pDYManager->Dispose();
		delete m_pDYManager;
		m_pDYManager = NULL;
	}
}

void GDisplayMenu::Initialize()
{
	m_pDYManager = new GDisplayManager(this);
	m_pDYManager->Initialize();
}

bool GDisplayMenu::DrawMainMenu()
{
	GetTagStrings(m_MenuLabelsMap);

	return DrawMenu();
}

bool GDisplayMenu::DrawCheckTestMenu(std::string text)
{
	m_MenuLabelsMap[GSoftKeysWindowTemplate::eSOFT_L1] = std::string(" Yes");
	m_MenuLabelsMap[GSoftKeysWindowTemplate::eSOFT_L2] = std::string(" Again");
	m_MenuLabelsMap[GSoftKeysWindowTemplate::eSOFT_L3] = std::string(" No");
	m_MenuLabelsMap[GSoftKeysWindowTemplate::eSOFT_L4] = std::string(" Previous\n Menu");

	m_MenuLabelsMap[GSoftKeysWindowTemplate::eSEGMENT_1] = "";
	m_MenuLabelsMap[GSoftKeysWindowTemplate::eSEGMENT_2] = "";
	m_MenuLabelsMap[GSoftKeysWindowTemplate::eSEGMENT_3] = "";
	m_MenuLabelsMap[GSoftKeysWindowTemplate::eSEGMENT_4] = text;
	m_MenuLabelsMap[GSoftKeysWindowTemplate::eSEGMENT_5] = "";
	m_MenuLabelsMap[GSoftKeysWindowTemplate::eSEGMENT_6] = "";
	m_MenuLabelsMap[GSoftKeysWindowTemplate::eSEGMENT_7] = "";
	m_MenuLabelsMap[GSoftKeysWindowTemplate::eSEGMENT_8] = "";
	m_MenuLabelsMap[GSoftKeysWindowTemplate::eSEGMENT_9] = "";

	m_MenuLabelsMap[GSoftKeysWindowTemplate::eSOFT_R1] = "";
	m_MenuLabelsMap[GSoftKeysWindowTemplate::eSOFT_R2] = "";
	m_MenuLabelsMap[GSoftKeysWindowTemplate::eSOFT_R3] = "";
	m_MenuLabelsMap[GSoftKeysWindowTemplate::eSOFT_R4] = "Summary ";

	return DrawMenu();
}

bool GDisplayMenu::DrawTestResultMenu()
{
	m_MenuLabelsMap[GSoftKeysWindowTemplate::eSOFT_L1] = "";
	m_MenuLabelsMap[GSoftKeysWindowTemplate::eSOFT_L2] = "";
	m_MenuLabelsMap[GSoftKeysWindowTemplate::eSOFT_L3] = "";
	m_MenuLabelsMap[GSoftKeysWindowTemplate::eSOFT_L4] = std::string(" Previous\n Menu");

	char arycBuffer[64];
	GDisplayServiceStatistics & oStats = GDisplayService::GetService()->GetStatistics();

	UINT8  u8PassCont1, u8PassCont2, u8PassCont3, u8PassCont4, u8PassCont5 = 0;
	UINT8  u8FailCont1, u8FailCont2, u8FailCont3, u8FailCont4, u8FailCont5 = 0;
	UINT16 u16TotalTest = GDisplayService::GetService()->GetStatistics().GetTotalTests();

	m_MenuLabelsMap[GSoftKeysWindowTemplate::eSEGMENT_1] = "The result of display test was:";

	u8PassCont1= oStats.GetStatistics(GDisplayServiceStatistics::ePass, GDisplayServiceStatistics::eTest1_White_Color);
	u8FailCont1= oStats.GetStatistics(GDisplayServiceStatistics::eFail, GDisplayServiceStatistics::eTest1_White_Color);
	snprintf(arycBuffer, sizeof(arycBuffer), "White color: PASS [%d] FAIL [%d].", u8PassCont1, u8FailCont1 );
	m_MenuLabelsMap[GSoftKeysWindowTemplate::eSEGMENT_2] = arycBuffer;

	u8PassCont2= oStats.GetStatistics(GDisplayServiceStatistics::ePass, GDisplayServiceStatistics::eTest2_Black_Color);
	u8FailCont2= oStats.GetStatistics(GDisplayServiceStatistics::eFail, GDisplayServiceStatistics::eTest2_Black_Color);
	snprintf(arycBuffer, sizeof(arycBuffer), "Black color: PASS [%d] FAIL [%d].", u8PassCont2, u8FailCont2 );
	m_MenuLabelsMap[GSoftKeysWindowTemplate::eSEGMENT_3] = arycBuffer;

	u8PassCont3= oStats.GetStatistics(GDisplayServiceStatistics::ePass, GDisplayServiceStatistics::eTest3_Red_Color);
	u8FailCont3= oStats.GetStatistics(GDisplayServiceStatistics::eFail, GDisplayServiceStatistics::eTest3_Red_Color);
	snprintf(arycBuffer, sizeof(arycBuffer), "Red color: PASS [%d] FAIL [%d].", u8PassCont3, u8FailCont3 );
	m_MenuLabelsMap[GSoftKeysWindowTemplate::eSEGMENT_4] = arycBuffer;

	u8PassCont4= oStats.GetStatistics(GDisplayServiceStatistics::ePass, GDisplayServiceStatistics::eTest4_Green_Color);
	u8FailCont4= oStats.GetStatistics(GDisplayServiceStatistics::eFail, GDisplayServiceStatistics::eTest4_Green_Color);
	snprintf(arycBuffer, sizeof(arycBuffer), "Green color: PASS [%d] FAIL [%d].", u8PassCont4, u8FailCont4 );
	m_MenuLabelsMap[GSoftKeysWindowTemplate::eSEGMENT_5] = arycBuffer;

	u8PassCont5= oStats.GetStatistics(GDisplayServiceStatistics::ePass, GDisplayServiceStatistics::eTest5_Blue_Color);
	u8FailCont5= oStats.GetStatistics(GDisplayServiceStatistics::eFail, GDisplayServiceStatistics::eTest5_Blue_Color);
	snprintf(arycBuffer, sizeof(arycBuffer), "Blue color: PASS [%d] FAIL [%d].", u8PassCont5, u8FailCont5 );
	m_MenuLabelsMap[GSoftKeysWindowTemplate::eSEGMENT_6] = arycBuffer;

	m_MenuLabelsMap[GSoftKeysWindowTemplate::eSEGMENT_7] = "Summary:";

	snprintf(arycBuffer, sizeof(arycBuffer), "PASS [%d] - FAIL [%d] - TOTAL [%d]",
											 (u8PassCont1+u8PassCont2+u8PassCont3+u8PassCont4+u8PassCont5),
											 (u8FailCont1+u8FailCont2+u8FailCont3+u8FailCont4+u8FailCont5),
											 u16TotalTest );
	m_MenuLabelsMap[GSoftKeysWindowTemplate::eSEGMENT_8] = arycBuffer;

	m_MenuLabelsMap[GSoftKeysWindowTemplate::eSEGMENT_9] = "";

	m_MenuLabelsMap[GSoftKeysWindowTemplate::eSOFT_R1] = "";
	m_MenuLabelsMap[GSoftKeysWindowTemplate::eSOFT_R2] = "";
	m_MenuLabelsMap[GSoftKeysWindowTemplate::eSOFT_R3] = "";
	m_MenuLabelsMap[GSoftKeysWindowTemplate::eSOFT_R4] = "";

	return DrawMenu();
}

bool GDisplayMenu::DrawErrorMenu(std::string text)
{
	m_MenuLabelsMap[GSoftKeysWindowTemplate::eSOFT_L1] = "";
	m_MenuLabelsMap[GSoftKeysWindowTemplate::eSOFT_L2] = "";
	m_MenuLabelsMap[GSoftKeysWindowTemplate::eSOFT_L3] = "";
	m_MenuLabelsMap[GSoftKeysWindowTemplate::eSOFT_L4] = std::string(" Previous\n Menu");

	m_MenuLabelsMap[GSoftKeysWindowTemplate::eSEGMENT_1] = "";
	m_MenuLabelsMap[GSoftKeysWindowTemplate::eSEGMENT_2] = "";
	m_MenuLabelsMap[GSoftKeysWindowTemplate::eSEGMENT_3] = "";
	m_MenuLabelsMap[GSoftKeysWindowTemplate::eSEGMENT_4] = text;
	m_MenuLabelsMap[GSoftKeysWindowTemplate::eSEGMENT_5] = "";
	m_MenuLabelsMap[GSoftKeysWindowTemplate::eSEGMENT_6] = "";
	m_MenuLabelsMap[GSoftKeysWindowTemplate::eSEGMENT_7] = "";
	m_MenuLabelsMap[GSoftKeysWindowTemplate::eSEGMENT_8] = "";
	m_MenuLabelsMap[GSoftKeysWindowTemplate::eSEGMENT_9] = "";

	m_MenuLabelsMap[GSoftKeysWindowTemplate::eSOFT_R1] = "";
	m_MenuLabelsMap[GSoftKeysWindowTemplate::eSOFT_R2] = "";
	m_MenuLabelsMap[GSoftKeysWindowTemplate::eSOFT_R3] = "";
	m_MenuLabelsMap[GSoftKeysWindowTemplate::eSOFT_R4] = "Summary ";

	return DrawMenu();
}

bool GDisplayMenu::DrawTimeoutMenu(std::string text)
{
	m_MenuLabelsMap[GSoftKeysWindowTemplate::eSOFT_L1] = "";
	m_MenuLabelsMap[GSoftKeysWindowTemplate::eSOFT_L2] = "";
	m_MenuLabelsMap[GSoftKeysWindowTemplate::eSOFT_L3] = "";
	m_MenuLabelsMap[GSoftKeysWindowTemplate::eSOFT_L4] = "";

	m_MenuLabelsMap[GSoftKeysWindowTemplate::eSEGMENT_1] = "";
	m_MenuLabelsMap[GSoftKeysWindowTemplate::eSEGMENT_2] = "";
	m_MenuLabelsMap[GSoftKeysWindowTemplate::eSEGMENT_3] = "";
	m_MenuLabelsMap[GSoftKeysWindowTemplate::eSEGMENT_4] = text;
	m_MenuLabelsMap[GSoftKeysWindowTemplate::eSEGMENT_5] = "";
	m_MenuLabelsMap[GSoftKeysWindowTemplate::eSEGMENT_6] = "";
	m_MenuLabelsMap[GSoftKeysWindowTemplate::eSEGMENT_7] = "";
	m_MenuLabelsMap[GSoftKeysWindowTemplate::eSEGMENT_8] = "";
	m_MenuLabelsMap[GSoftKeysWindowTemplate::eSEGMENT_9] = "";

	m_MenuLabelsMap[GSoftKeysWindowTemplate::eSOFT_R1] = "";
	m_MenuLabelsMap[GSoftKeysWindowTemplate::eSOFT_R2] = "";
	m_MenuLabelsMap[GSoftKeysWindowTemplate::eSOFT_R3] = "";
	m_MenuLabelsMap[GSoftKeysWindowTemplate::eSOFT_R4] = "";

	return DrawMenu();
}

bool GDisplayMenu::HandleSPOTMessage(GSPOTMessage* pMsg)
{
	// this menu only shows different tag segments on SPOT screen
	return true;
}

void GDisplayMenu::HandleSoftKeyEvent(UINT8 KeyCode)
{
	switch(KeyCode)
	{
		case eL1:  // L1
			m_pDYManager->ProcessEventReceived(GDisplayManager::eEventSoftKeyL1);
		break;

		case eL2:  // L2
			m_pDYManager->ProcessEventReceived(GDisplayManager::eEventSoftKeyL2);
		break;

		case eL3:  // L3
			m_pDYManager->ProcessEventReceived(GDisplayManager::eEventSoftKeyL3);
		break;

		case eL4:  // L4
			m_pDYManager->ProcessEventReceived(GDisplayManager::eEventSoftKeyL4);
		break;

		case eR1:  // R1
			m_pDYManager->ProcessEventReceived(GDisplayManager::eEventSoftKeyR1);
		break;

		case eR2:  // R2
			m_pDYManager->ProcessEventReceived(GDisplayManager::eEventSoftKeyR2);
		break;

		case eR3:  // R3
			m_pDYManager->ProcessEventReceived(GDisplayManager::eEventSoftKeyR3);
		break;

		case eR4:  // R4
			m_pDYManager->ProcessEventReceived(GDisplayManager::eEventSoftKeyR4);
		break;
	}

	EnableSoftKeys();
}

void GDisplayMenu::ClearScreen()
{
	const char* arypcLabels[7];

	arypcLabels[0] = "";
	arypcLabels[1] = "";
	arypcLabels[2] = "";
	arypcLabels[3] = "";
	arypcLabels[4] = "";
	arypcLabels[5] = "";
	arypcLabels[6] = "";

	UpdateUserLabels(7, arypcLabels);
}

void GDisplayMenu::Quit()
{
	m_pManager->QueueMenuPop();
	EnableSoftKeys();
}


/**
 *
 * STATIC METHODS
 *
 */

int GDisplayMenu::GetTagStrings(GSoftKeysWindowTemplate::tStringTagMap & rMap)
{
	DY_HW_INFO& rDYInfo = GGlobalSystemInformation::GetInstance()->GetDisplayInfo();
	char arycBuffer[64];

	rMap[GSoftKeysWindowTemplate::eSOFT_L1] = std::string(" Begin\n Test");
	rMap[GSoftKeysWindowTemplate::eSOFT_L4] = std::string(" Previous\n Menu");

	rMap[GSoftKeysWindowTemplate::eSEGMENT_1] = std::string("DISPLAY Diagnostics");
	sprintf(arycBuffer, "Type:  %02X  ", int(rDYInfo.m_u8DevType));

	rMap[GSoftKeysWindowTemplate::eSEGMENT_2] = std::string(arycBuffer);
	snprintf(arycBuffer, sizeof(arycBuffer), "Model:  %s  ", rDYInfo.m_pcDevModel);

	rMap[GSoftKeysWindowTemplate::eSEGMENT_3] = std::string(arycBuffer);
	snprintf(arycBuffer, sizeof(arycBuffer), "Resolution:  %dpx X %dpx X %d bit color.",
	                    int(rDYInfo.m_DevHwrConfig.m_u16Xres),
	                    int(rDYInfo.m_DevHwrConfig.m_u16Yres),
	                    int(rDYInfo.m_DevHwrConfig.m_u8ColorDepth) );

	rMap[GSoftKeysWindowTemplate::eSEGMENT_4] = std::string(arycBuffer);
	rMap[GSoftKeysWindowTemplate::eSEGMENT_6] = std::string(" Press Begin Test to start the test.");
	rMap[GSoftKeysWindowTemplate::eSEGMENT_7] = std::string(" Summary at any time will bypass tests.");


	rMap[GSoftKeysWindowTemplate::eSOFT_R4] = "Summary ";

	return 9;
}

