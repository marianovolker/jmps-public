#include <cstdio>
#include <cstring>

#include "GCardReaderMenu.h"

#include "SPOT/SystemInformation/GGlobalSystemInformation.h"
#include "Menus/CardReader/GCardReaderManager.h"


GCardReaderMenu::GCardReaderMenu()
	: m_pCRManager(NULL)
{
	m_iWinID = eWinId_CardReaderMenu;

	//TODO: Eliminate this when repository is working OK
	GetTagStrings(m_MenuLabelsMap);
}

GCardReaderMenu::~GCardReaderMenu()
{
	if(m_pCRManager)
	{
		m_pCRManager->Dispose();
		delete m_pCRManager;
		m_pCRManager = NULL;
	}
}

void GCardReaderMenu::Initialize()
{
	m_pCRManager = new GCardReaderManager(this);
	m_pCRManager->Initialize();
}

bool GCardReaderMenu::DrawMenu()
{
	return GAbsMenu::DrawMenu();
}

void GCardReaderMenu::ShowCardInsertNotification()
{
	const char* arypcLabels[8];

	SetLabel(GSoftKeysWindowTemplate::eSOFT_R3, " "); //hide the label


	memset( arypcLabels, 0, sizeof(arypcLabels) );
	arypcLabels[4] = "Please remove card.";
	arypcLabels[5] = " ";
	arypcLabels[6] = " ";

	UpdateUserLabels(7, arypcLabels);
}

void GCardReaderMenu::ShowStatsScreen()
{
	const char* arypcLabels[8];

	SetLabel(GSoftKeysWindowTemplate::eSOFT_R3, " "); //hide the label
	SetLabel(GSoftKeysWindowTemplate::eSOFT_L3, " "); //hide the label

	memset( arypcLabels, 0, sizeof(arypcLabels) );


	GCardReaderService* pCRSvc = GCardReaderService::GetService();
	const GCardReaderServiceStatistics & rCRStats = pCRSvc->GetSessionStatistics();

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


//	"Pass %5d %5d %5d  %5d\n",           // line  4
//	"Fail %5d %5d %5d  %5d\n",           // line  5
//	"N/P  %5d %5d %5d  %5d\n",           // line  6

//	"   Total Swipes: %6d\n",            // line  8

    char cPass[MAX_LINE_LEN];
    char cFail[MAX_LINE_LEN];
    char cNotPresent[MAX_LINE_LEN];
    char cTotalSwipes[MAX_LINE_LEN];


    snprintf(cPass,MAX_LINE_LEN,"Pass %5d  %5d  %5d  %5d",itk1good, itk2good, itk3good, ichipgood);
    snprintf(cFail,MAX_LINE_LEN,"Fail  %5d  %5d  %5d  %5d",itk1bad, itk2bad, itk3bad, ichipbad);
    snprintf(cNotPresent,MAX_LINE_LEN,"N/P  %5d  %5d  %5d  %5d",itk1np, itk2np, itk3np, ichipnp);
    snprintf(cTotalSwipes,MAX_LINE_LEN,"   Total Swipes: %6d", itotalswipes);

	arypcLabels[0] = "Test Statistics";
	arypcLabels[1] = "      -----   Tk1   Tk2   Tk3   Chip";
	arypcLabels[2] = cPass;
	arypcLabels[3] = cFail;
	arypcLabels[4] = cNotPresent;
	arypcLabels[5] = cTotalSwipes;
	arypcLabels[6] = "Insert SPOT Check Diagnostic Test Card.";

	UpdateUserLabels(7, arypcLabels);
}


void GCardReaderMenu::ShowChipCardDetected()
{
	const char* arypcLabels[8];

	SetLabel(GSoftKeysWindowTemplate::eSOFT_R3, " "); //hide the label


	memset( arypcLabels, 0, sizeof(arypcLabels) );


	arypcLabels[0] = "";
	arypcLabels[1] = "";
	arypcLabels[2] = "";
	arypcLabels[3] = "Chip detected,";
	arypcLabels[4] = "";
	arypcLabels[5] = "please wait...";
	arypcLabels[6] = " ";

	UpdateUserLabels(7, arypcLabels);
}

void GCardReaderMenu::ShowProcessingCard()
{
	const char* arypcLabels[8];

	SetLabel(GSoftKeysWindowTemplate::eSOFT_R3, " "); //hide the label


	memset( arypcLabels, 0, sizeof(arypcLabels) );


	arypcLabels[0] = "";
	arypcLabels[1] = "";
	arypcLabels[2] = "";
	arypcLabels[3] = "Processing card,";
	arypcLabels[4] = "";
	arypcLabels[5] = "please wait...";
	arypcLabels[6] = " ";

	UpdateUserLabels(7, arypcLabels);
}
//Shows the resumed test results.
void GCardReaderMenu::ShowCardRemoveNotification(const char* pcLabel1,const char* pcLabel2)
{

	const char* arypcLabels[7];

	SetLabel(GSoftKeysWindowTemplate::eSOFT_L3, " Show test\n Statistics"); //hide the label

	if (m_pCRManager->isACardWasRead())
	{
		SetLabel(GSoftKeysWindowTemplate::eSOFT_R3, "Show \nDetails "); //change label for softkey
	}
	else
	{
		SetLabel(GSoftKeysWindowTemplate::eSOFT_R3, " "); //change label for softkey
	}

	arypcLabels[0] = "";
	arypcLabels[1] = pcLabel1;
	arypcLabels[2] = pcLabel2;
	arypcLabels[3] = "";
	arypcLabels[4] = "";
	arypcLabels[5] = "";
	arypcLabels[6] = "Insert SPOT Check Diagnostic Test Card.";

	UpdateUserLabels(7, arypcLabels);
}

void GCardReaderMenu::ShowDetailsScreen(const char* pcLabel1, const char* pcLabel2, const char* pcLabel3, const char* pcLabel4)
{

	const char* arypcLabels[7];

	SetLabel(GSoftKeysWindowTemplate::eSOFT_R3, "Hide \nDetails "); //change label for softkey


	arypcLabels[0] = "";
	arypcLabels[1] = pcLabel1;
	arypcLabels[2] = pcLabel2;
	arypcLabels[3] = pcLabel3;
	arypcLabels[4] = pcLabel4;
	arypcLabels[5] = "";
	arypcLabels[6] = "Insert SPOT Check Diagnostic Test Card.";

	UpdateUserLabels(7, arypcLabels);
}

bool GCardReaderMenu::HandleSPOTMessage(GSPOTMessage* pMsg)
{
	// this menu only shows different tag segments on SPOT screen
	return true;
}

void GCardReaderMenu::HandleSoftKeyEvent(UINT8 KeyCode)
{
	switch( int(KeyCode) )
	{
		case eR3:  // R3
					//see status for action on R3
					if (m_pCRManager->GetStatus() == GCardReaderManager::eShowDetails)
					{
						m_pCRManager->GoToTestResultScreen();
					}
					else if (m_pCRManager->GetStatus() == GCardReaderManager::eShowTestResult && m_pCRManager->isACardWasRead())
					{
						m_pCRManager->GoToDetailsScreen();
					}

					EnableSoftKeys();
					break;

		case eL4:  // L4 - back to previous menu
			if (m_pCRManager->GetStatus() == GCardReaderManager::eShowStats)
			{
				m_pCRManager->GoToTestResultScreen();

			}
			else
			{
				m_pManager->QueueMenuPop();
			}

			EnableSoftKeys();
			break;

		case eL3:  // L3
			if (m_pCRManager->GetStatus() != GCardReaderManager::eShowChipDetected
			                && m_pCRManager->GetStatus() != GCardReaderManager::eShowPleaseRemoveCard
			                && m_pCRManager->GetStatus() != GCardReaderManager::eShowStats)
			{
				m_pCRManager->GoToStatistics();
			}

			EnableSoftKeys();
			break;

		case eL1:  // L1
		case eL2:  // L2
		case eR1:  // R1
		case eR2:  // R2
		case eR4:  // R4  //not used key anymore
		default:
			EnableSoftKeys();
			break;
	}
}

void GCardReaderMenu::ClearScreen()
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

int GCardReaderMenu::GetTagStrings(GSoftKeysWindowTemplate::tStringTagMap & rMap)
{
	// initialize map of tag-id/label pairs for initial window creation
	CR_HW_INFO& rCRInfo = GGlobalSystemInformation::GetInstance()->GetCardReaderInfo();
	char arycBuffer[40];

	// initialize map of tagid/label pairs (maps segments to text for those segments)
	rMap[GSoftKeysWindowTemplate::eSOFT_L3] = " Show test\n Statistics";
	rMap[GSoftKeysWindowTemplate::eSOFT_L4] = " Previous\n Menu";

	rMap[GSoftKeysWindowTemplate::eSEGMENT_1] = "CARD READER Diagnostics";

	sprintf(arycBuffer, "Hardware ID:  %02X%02X  ", int(rCRInfo.m_aryu8HwrID[0]), int(rCRInfo.m_aryu8HwrID[1]));
	rMap[GSoftKeysWindowTemplate::eSEGMENT_2] = arycBuffer;

	sprintf(arycBuffer, "Type:  %02X  ", int(rCRInfo.m_u8DevType));
	rMap[GSoftKeysWindowTemplate::eSEGMENT_3] = arycBuffer;

	snprintf(arycBuffer, sizeof(arycBuffer), "Model:  %s  ", rCRInfo.m_pcDevModel);
	rMap[GSoftKeysWindowTemplate::eSEGMENT_4] = arycBuffer;

	snprintf(arycBuffer, sizeof(arycBuffer), "S/N:  %s  ", rCRInfo.m_pcSerialNumber);
	rMap[GSoftKeysWindowTemplate::eSEGMENT_5] = arycBuffer;

	rMap[GSoftKeysWindowTemplate::eSEGMENT_8] = "Insert SPOT Check Diagnostic Test Card.";

	return 8;
}
