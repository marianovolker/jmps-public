#include <cstdio>
#include <cstring>

#include "GTrindMenu.h"

#include "SPOT/SystemInformation/GGlobalSystemInformation.h"
#include "Menus/Trind/GTrindManager.h"

const char* GTrindMenu::m_arpcTRLEDLabels[]= {
		"No Test",
		"Pass   ",
		"Fail   ",
};


GTrindMenu::GTrindMenu()
	: m_pTRManager(NULL),
	  m_TagPassCount(0),
	  m_TagFailCount(0),
	  m_eTRLEDResult(eTRInitial)
{
	m_iWinID = eWinId_TrindMenu;

	//TODO: Eliminate this when repository is working OK
	GetTagStrings(m_MenuLabelsMap);
}

GTrindMenu::~GTrindMenu()
{
	if(m_pTRManager)
	{
		m_pTRManager->Dispose();
		delete m_pTRManager;
		m_pTRManager = NULL;
	}
}

void GTrindMenu::Initialize()
{
	m_pTRManager = new GTrindManager(this);
	m_pTRManager->Initialize();
}

bool GTrindMenu::DrawMenu()
{
	return GAbsMenu::DrawMenu();
}

void GTrindMenu::ShowStatsScreen()
{
	const char* arypcLabels[8];
	SetLabel(GSoftKeysWindowTemplate::eSEGMENT_1, "TRIND Diagnostics");
	SetLabel(GSoftKeysWindowTemplate::eSOFT_L1, " ");
	SetLabel(GSoftKeysWindowTemplate::eSOFT_L2, " ");
	SetLabel(GSoftKeysWindowTemplate::eSOFT_R3, " "); //hide the label
	SetLabel(GSoftKeysWindowTemplate::eSOFT_L3, " "); //hide the label

	memset( arypcLabels, 0, sizeof(arypcLabels) );

	char ledStats[MAX_LINE_LEN2];
	char tPass[MAX_LINE_LEN2];
	char tFail[MAX_LINE_LEN2];
	char tTotal[MAX_LINE_LEN2];


	GTrindServiceStatistics & oSessionStats = GTrindService::GetService()->GetSessionStatistics();

	snprintf(ledStats,MAX_LINE_LEN2,"LED Test  :    %s",m_arpcTRLEDLabels[oSessionStats.GetLEDStatistics()]);

	snprintf(tPass,MAX_LINE_LEN2,"Pass   %9d",oSessionStats.GetTagStatistics(GTrindServiceStatistics::eTrPass));
	snprintf(tFail,MAX_LINE_LEN2,"Fail   %9d",oSessionStats.GetTagStatistics(GTrindServiceStatistics::eTrFail));
	snprintf(tTotal,MAX_LINE_LEN2,"Total  %9d",oSessionStats.GetTagStatistics(GTrindServiceStatistics::eTrPass)+
											   oSessionStats.GetTagStatistics(GTrindServiceStatistics::eTrFail));
	arypcLabels[0] = "Test Statistics";
	arypcLabels[1] = ledStats;
	arypcLabels[2] = "-------         Tags";
	arypcLabels[3] = tPass;
	arypcLabels[4] = tFail;
	arypcLabels[5] = tTotal;
	arypcLabels[6] = "Present test Tag in front of the TRIND";

	UpdateUserLabels(7, arypcLabels);
}


void GTrindMenu::ShowTagRead()
{
	const char* arypcLabels[8];
	SetLabel(GSoftKeysWindowTemplate::eSOFT_L2, " ");
	SetLabel(GSoftKeysWindowTemplate::eSOFT_L3, " ");
	SetLabel(GSoftKeysWindowTemplate::eSOFT_R3, " "); //hide the label


	memset( arypcLabels, 0, sizeof(arypcLabels) );


	arypcLabels[0] = "";
	arypcLabels[1] = "";
	arypcLabels[2] = "";
	arypcLabels[3] = "Tag read successfully";
	arypcLabels[4] = "";
	arypcLabels[5] = "";
	arypcLabels[6] = " ";

	UpdateUserLabels(7, arypcLabels);
}

bool GTrindMenu::DrawTrindLEDCheckTestMenu(std::string text)
{
	m_MenuLabelsMap[GSoftKeysWindowTemplate::eSOFT_L1] = std::string(" Yes");
	m_MenuLabelsMap[GSoftKeysWindowTemplate::eSOFT_L2] = std::string("");
	m_MenuLabelsMap[GSoftKeysWindowTemplate::eSOFT_L3] = std::string(" No");
	m_MenuLabelsMap[GSoftKeysWindowTemplate::eSOFT_L4] = std::string(" Previous\n Menu");

	m_MenuLabelsMap[GSoftKeysWindowTemplate::eSEGMENT_1] = "TRIND LED Test";
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

void GTrindMenu::IncrementTagCount( eTagResult result)
{
	if(result == eTrPass)
	{
		m_TagPassCount++;
	}
	else
	{
		m_TagFailCount++;
	}
}

void GTrindMenu::SetLEDTestResult(eTRLEDResult result)
{
	m_eTRLEDResult =result;
}


bool GTrindMenu::HandleSPOTMessage(GSPOTMessage* pMsg)
{
	// this menu only shows different tag segments on SPOT screen
	return true;
}

void GTrindMenu::HandleSoftKeyEvent(UINT8 KeyCode)
{
	switch( int(KeyCode) )
	{
		case eL2:  // L2
				//see status for action on L2
				if (m_pTRManager->GetStatus() == GTrindManager::eShowInitial)
				{	m_pTRManager->DisableService();
					m_pTRManager->EnableLED();
					m_pTRManager->DrawTrindLEDCheckTestMenu("Is the LED ON ?") ;
				}
				EnableSoftKeys();
				break;

		case eR3:  // R3
					//see status for action on R3
				if (m_pTRManager->GetStatus() == GTrindManager::eShowStats)
				{
					m_pTRManager->ShowStatsScreen();
				}
				EnableSoftKeys();
				break;

		case eL4:  // L4 - back to previous menu
			if (m_pTRManager->GetStatus() == GTrindManager::eShowInitial)
			{
				m_pManager->QueueMenuPop();
			}
			else if(m_pTRManager->GetStatus() == GTrindManager::eShowStats)
			{
				ClearScreen();
				GetTagStrings(m_MenuLabelsMap);
				DrawMenu();
				m_pTRManager->ChangeStateTo(GTrindManager::eShowInitial);

			}
			else if(m_pTRManager->GetStatus() == GTrindManager::eShowLEDCheck)
			{
				m_pTRManager->DisableLED();
				m_pTRManager->EnableService();
				ClearScreen();
				GetTagStrings(m_MenuLabelsMap);
				DrawMenu();
				m_pTRManager->ChangeStateTo(GTrindManager::eShowInitial);
			}

			EnableSoftKeys();
			break;

		case eL3:  // L3
			if (m_pTRManager->GetStatus() != GTrindManager::eShowTagRead
			                && m_pTRManager->GetStatus() != GTrindManager::eShowStats)
			{
				if(m_pTRManager->GetStatus() == GTrindManager::eShowLEDCheck)
				{
					m_pTRManager->DisableLED();
					m_pTRManager->SetLEDTestResult(false);
					m_pTRManager->EnableService();
				}
				m_pTRManager->ShowStatsScreen();
			}
			EnableSoftKeys();
			break;

		case eL1:  // L1
			if (m_pTRManager->GetStatus() == GTrindManager::eShowLEDCheck)
			{
				m_pTRManager->DisableLED();
				m_pTRManager->SetLEDTestResult(true);
				m_pTRManager->EnableService();
				m_pTRManager->ShowStatsScreen();
			}
			EnableSoftKeys();
			break;

		case eR1:  // R1
		case eR2:  // R2
		case eR4:  // R4  //not used key anymore
		default:
			EnableSoftKeys();
			break;
	}
}

void GTrindMenu::ClearScreen()
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


/**
 *
 * STATIC METHODS
 *
 */

int GTrindMenu::GetTagStrings(GSoftKeysWindowTemplate::tStringTagMap & rMap)
{
	// initialize map of tag-id/label pairs for initial window creation
	TR_HW_INFO& rTRInfo = GGlobalSystemInformation::GetInstance()->GetTrindInfo();
	char arycBuffer[40];

	// initialize map of tagid/label pairs (maps segments to text for those segments)
	rMap[GSoftKeysWindowTemplate::eSOFT_L1] = "";
	rMap[GSoftKeysWindowTemplate::eSOFT_L2] = " Test\n LED";
	rMap[GSoftKeysWindowTemplate::eSOFT_L3] = " Show test\n Statistics";
	rMap[GSoftKeysWindowTemplate::eSOFT_L4] = " Previous\n Menu";

	rMap[GSoftKeysWindowTemplate::eSEGMENT_1] = "TRIND Diagnostics";

	sprintf(arycBuffer, "Hardware ID:  %02X%02X  ", int(rTRInfo.m_aryu8HwrID[0]), int(rTRInfo.m_aryu8HwrID[1]));
	//rMap[GSoftKeysWindowTemplate::eSEGMENT_2] = arycBuffer;
	rMap[GSoftKeysWindowTemplate::eSEGMENT_2] = "";
	sprintf(arycBuffer, "Type:  %02X  ", int(rTRInfo.m_u8DeviceType));
	//rMap[GSoftKeysWindowTemplate::eSEGMENT_3] = arycBuffer;
	rMap[GSoftKeysWindowTemplate::eSEGMENT_3] = "";
	snprintf(arycBuffer, sizeof(arycBuffer), "Model:  %s  ", rTRInfo.m_pcDevModel);
	//rMap[GSoftKeysWindowTemplate::eSEGMENT_4] = arycBuffer;
	rMap[GSoftKeysWindowTemplate::eSEGMENT_4] = "";
	snprintf(arycBuffer, sizeof(arycBuffer), "S/N:  %s  ", rTRInfo.m_pcSerialNumber);
	//rMap[GSoftKeysWindowTemplate::eSEGMENT_5] = arycBuffer;
	rMap[GSoftKeysWindowTemplate::eSEGMENT_5] = "";
	rMap[GSoftKeysWindowTemplate::eSEGMENT_7] = "Present test Tag in front of the TRIND";

	return 8;
}
