#include <cstdio>
#include "SPOT/SystemInformation/GGlobalSystemInformation.h"
#include "GSecureAudioMenu.h"
#include "Common/Util/DEBUGOUT.h"
#include "Services/Display/GDisplayService.h"

#include "GSecureAudioManager.h"

GSecureAudioMenu::GSecureAudioMenu(void):
    m_u16Width(0),
    m_u16Height(0),
    m_pDYManager(NULL)
{
	DY_HW_INFO& rDYInfo = GGlobalSystemInformation::GetInstance()->GetDisplayInfo();

	m_u16Width  = rDYInfo.m_DevHwrConfig.m_u16Xres;
	m_u16Height = rDYInfo.m_DevHwrConfig.m_u16Yres;
	m_iWinID    = eWinId_SecureAudioMenu;

	//TODO: Eliminate this when repository is working OK
	GetTagStrings(m_MenuLabelsMap);

	//TODO: Uncomment this if not displaying the menu!!!
	//DrawMenu();
}

GSecureAudioMenu::~GSecureAudioMenu(void)
{
	if(m_pDYManager)
	{
		m_pDYManager->Dispose();
		delete m_pDYManager;
		m_pDYManager = NULL;
	}
}

void GSecureAudioMenu::Initialize()
{
	m_pDYManager = new GSecureAudioManager(this);
	m_pDYManager->Initialize();
}

bool GSecureAudioMenu::DrawMainMenu()
{
	ClearScreen();
	m_MenuLabelsMap[GSoftKeysWindowTemplate::eSOFT_L1] = std::string(" ");
	m_MenuLabelsMap[GSoftKeysWindowTemplate::eSOFT_L2] = std::string(" ");
	m_MenuLabelsMap[GSoftKeysWindowTemplate::eSOFT_L3] = std::string(" ");
	m_MenuLabelsMap[GSoftKeysWindowTemplate::eSOFT_L4] = std::string(" ");

	m_MenuLabelsMap[GSoftKeysWindowTemplate::eSOFT_R1] = "";
	m_MenuLabelsMap[GSoftKeysWindowTemplate::eSOFT_R2] = "";
	m_MenuLabelsMap[GSoftKeysWindowTemplate::eSOFT_R3] = "";
	m_MenuLabelsMap[GSoftKeysWindowTemplate::eSOFT_R4] = "";


	GetTagStrings(m_MenuLabelsMap);

	return DrawMenu();
}

bool GSecureAudioMenu::DrawNonSecureCheckTestMenu(std::string text)
{
	m_MenuLabelsMap[GSoftKeysWindowTemplate::eSOFT_L1] = std::string(" Yes");
	m_MenuLabelsMap[GSoftKeysWindowTemplate::eSOFT_L2] = std::string("");
	m_MenuLabelsMap[GSoftKeysWindowTemplate::eSOFT_L3] = std::string(" No");
	m_MenuLabelsMap[GSoftKeysWindowTemplate::eSOFT_L4] = std::string(" Previous\n Menu");

	m_MenuLabelsMap[GSoftKeysWindowTemplate::eSEGMENT_1] = "Non Secure Test";
	m_MenuLabelsMap[GSoftKeysWindowTemplate::eSEGMENT_2] = "";
	m_MenuLabelsMap[GSoftKeysWindowTemplate::eSEGMENT_3] = "";
	m_MenuLabelsMap[GSoftKeysWindowTemplate::eSEGMENT_4] = "";
	m_MenuLabelsMap[GSoftKeysWindowTemplate::eSEGMENT_5] = text;
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

bool GSecureAudioMenu::DrawSecureCheckTestMenu(std::string text)
{
	m_MenuLabelsMap[GSoftKeysWindowTemplate::eSOFT_L1] = std::string(" Yes");
	m_MenuLabelsMap[GSoftKeysWindowTemplate::eSOFT_L2] = std::string(" Again");
	m_MenuLabelsMap[GSoftKeysWindowTemplate::eSOFT_L3] = std::string(" No");
	m_MenuLabelsMap[GSoftKeysWindowTemplate::eSOFT_L4] = std::string(" Previous\n Menu");

	m_MenuLabelsMap[GSoftKeysWindowTemplate::eSEGMENT_1] = "Secure Mode Test";
	m_MenuLabelsMap[GSoftKeysWindowTemplate::eSEGMENT_2] = "";
	m_MenuLabelsMap[GSoftKeysWindowTemplate::eSEGMENT_3] = "";
	m_MenuLabelsMap[GSoftKeysWindowTemplate::eSEGMENT_4] = "Did you hear audio from the speaker";
	m_MenuLabelsMap[GSoftKeysWindowTemplate::eSEGMENT_5] = "during secure test session?";
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

bool GSecureAudioMenu::DrawSecureTestMenu(std::string text)
{
	m_MenuLabelsMap[GSoftKeysWindowTemplate::eSOFT_L1] = std::string(" Start");
	m_MenuLabelsMap[GSoftKeysWindowTemplate::eSOFT_L2] = std::string(" ");
	m_MenuLabelsMap[GSoftKeysWindowTemplate::eSOFT_L3] = std::string(" ");
	m_MenuLabelsMap[GSoftKeysWindowTemplate::eSOFT_L4] = std::string(" Previous\n Menu");

	m_MenuLabelsMap[GSoftKeysWindowTemplate::eSEGMENT_1] = " Secure Mode Test";
	m_MenuLabelsMap[GSoftKeysWindowTemplate::eSEGMENT_2] = "";
	m_MenuLabelsMap[GSoftKeysWindowTemplate::eSEGMENT_3] = "";
	m_MenuLabelsMap[GSoftKeysWindowTemplate::eSEGMENT_4] = "";
	m_MenuLabelsMap[GSoftKeysWindowTemplate::eSEGMENT_5] = text;
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

bool GSecureAudioMenu::DrawTestResultMenu(std::string text5, std::string text6)
{
	ClearScreen();
	m_MenuLabelsMap[GSoftKeysWindowTemplate::eSOFT_L1] = "";
	m_MenuLabelsMap[GSoftKeysWindowTemplate::eSOFT_L2] = "";
	m_MenuLabelsMap[GSoftKeysWindowTemplate::eSOFT_L3] = "";
	m_MenuLabelsMap[GSoftKeysWindowTemplate::eSOFT_L4] = std::string(" Previous\n Menu");

	m_MenuLabelsMap[GSoftKeysWindowTemplate::eSEGMENT_1] = "The result of Secure Audio test is:";

	m_MenuLabelsMap[GSoftKeysWindowTemplate::eSEGMENT_5] = text5;
	m_MenuLabelsMap[GSoftKeysWindowTemplate::eSEGMENT_6] = text6;

	m_MenuLabelsMap[GSoftKeysWindowTemplate::eSEGMENT_9] = "";

	m_MenuLabelsMap[GSoftKeysWindowTemplate::eSOFT_R1] = "";
	m_MenuLabelsMap[GSoftKeysWindowTemplate::eSOFT_R2] = "";
	m_MenuLabelsMap[GSoftKeysWindowTemplate::eSOFT_R3] = "";
	m_MenuLabelsMap[GSoftKeysWindowTemplate::eSOFT_R4] = "";

	return DrawMenu();
}

bool GSecureAudioMenu::DrawErrorMenu(std::string text)
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

bool GSecureAudioMenu::DrawTimeoutMenu(std::string text)
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

bool GSecureAudioMenu::HandleSPOTMessage(GSPOTMessage* pMsg)
{
	// this menu only shows different tag segments on SPOT screen
	return true;
}

void GSecureAudioMenu::HandleSoftKeyEvent(UINT8 KeyCode)
{
	switch(KeyCode)
	{
		case eL1:  // L1
			m_pDYManager->ProcessEventReceived(GSecureAudioManager::eEventSoftKeyL1);
		break;

		case eL2:  // L2
			m_pDYManager->ProcessEventReceived(GSecureAudioManager::eEventSoftKeyL2);
		break;

		case eL3:  // L3
			m_pDYManager->ProcessEventReceived(GSecureAudioManager::eEventSoftKeyL3);
		break;

		case eL4:  // L4
			m_pDYManager->ProcessEventReceived(GSecureAudioManager::eEventSoftKeyL4);
		break;

		case eR1:  // R1
			m_pDYManager->ProcessEventReceived(GSecureAudioManager::eEventSoftKeyR1);
		break;

		case eR2:  // R2
			m_pDYManager->ProcessEventReceived(GSecureAudioManager::eEventSoftKeyR2);
		break;

		case eR3:  // R3
			m_pDYManager->ProcessEventReceived(GSecureAudioManager::eEventSoftKeyR3);
		break;

		case eR4:  // R4
			m_pDYManager->ProcessEventReceived(GSecureAudioManager::eEventSoftKeyR4);
		break;
	}

	EnableSoftKeys();
}


void GSecureAudioMenu::ClearScreen()
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

void GSecureAudioMenu::Quit()
{
	m_pManager->QueueMenuPop();
	EnableSoftKeys();
}

void GSecureAudioMenu::PushMenu(GAbsMenu* pMenu)
{
	m_pManager->PushMenu(pMenu);
}


/**
 *
 * STATIC METHODS
 *
 */

int GSecureAudioMenu::GetTagStrings(GSoftKeysWindowTemplate::tStringTagMap & rMap)
{
	DY_HW_INFO& rDYInfo = GGlobalSystemInformation::GetInstance()->GetDisplayInfo();
	char arycBuffer[64];

	rMap[GSoftKeysWindowTemplate::eSOFT_L1] = std::string(" Begin\n Test");
	rMap[GSoftKeysWindowTemplate::eSOFT_L4] = std::string(" Previous\n Menu");

	rMap[GSoftKeysWindowTemplate::eSEGMENT_1] = std::string("Secure Audio Diagnostics");

	rMap[GSoftKeysWindowTemplate::eSEGMENT_4] = std::string(" Press Begin Test to start the test.");
	rMap[GSoftKeysWindowTemplate::eSEGMENT_5] = std::string(" Summary at any time will bypass tests.");


	rMap[GSoftKeysWindowTemplate::eSOFT_R4] = "Summary ";

	return 9;
}

