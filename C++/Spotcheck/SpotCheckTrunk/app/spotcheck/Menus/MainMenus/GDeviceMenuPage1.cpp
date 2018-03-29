#include "Common/Util/DEBUGOUT.h"
#include "Menus/MainMenus/GDeviceMenuPage1.h"
#include "Menus/MainMenus/GDeviceMenuPage2.h"
#include "Menus/BNA/GBNAMainMenu.h"
#include "Menus/CardReader/GCardReaderMenu.h"

//#include "Menus/BarcodeScanner/GBarcodeScannerMenu.h"
#include "Menus/ContactlessReader/GContactlessReaderMenu.h"
#include "Menus/Printer/GPrinterMenu.h"
#include "Menus/Trind/GTrindMenu.h"
#include "Menus/SecureAudio/GSecureAudioMenu.h"
#include "Menus/Keypad/GKeyboardSelectorMenu.h"
#include "Menus/Display/GDisplayMenu.h"
#include "SPOT/SPOTProtocolInterface/GSPOTDefs.h"
#include "SPOT/SystemInformation/GGlobalSystemInformation.h"
#include "GSystem.h"
#include "GTypes.h"
#include "Menus/Trind/GTrindManager.h"

using namespace GSpotDefs;

GDeviceMenuPage1::GDeviceMenuPage1()
{
	m_iWinID = eWinId_DeviceMenu;

	//TODO: Eliminate this when repository is working OK
	GetTagStrings(m_MenuLabelsMap);
}

GDeviceMenuPage1::~GDeviceMenuPage1()
{
}

bool GDeviceMenuPage1::DrawMenu()
{
	return GAbsMenu::DrawMenu();
}

bool GDeviceMenuPage1::HandleSPOTMessage(GSPOTMessage* pMsg)
{
   // this menu only responds to soft key events, which come through "HandleSoftKeyEvent(KeyCode)"
   return true;
}

void GDeviceMenuPage1::HandleSoftKeyEvent(UINT8 KeyCode)
{
	switch (KeyCode)
	{


		case eL3:  // L3
		{
			GGlobalSystemInformation* pSysInfo = GGlobalSystemInformation::GetInstance();
			if(pSysInfo)
			{
				if(pSysInfo->isVGDUnit())
				{
					GSecureAudioMenu* pMenu = new GSecureAudioMenu();
					pMenu->Initialize();
					m_pManager->PushMenu(pMenu);
					break;

				}
			}
			GContactlessReaderMenu* pMenu = new GContactlessReaderMenu();
			pMenu->Initialize();
			m_pManager->PushMenu(pMenu);
		}
		break;

		case eL2:  // L2
		{
			GCardReaderMenu* pMenu = new GCardReaderMenu();
			pMenu->Initialize();
			m_pManager->PushMenu(pMenu);
		}
		break;

		case eL4:  // L4 - back to previous menu
			m_pManager->QueueMenuPop();
		break;

		case eR1:  // R1
		{
			GPrinterMenu* pMenu = new GPrinterMenu();
			pMenu->Initialize();
			m_pManager->PushMenu(pMenu);
		}
		break;

		case eR2:  // R2
		{
			GKeyboardSelectorMenu* pMenu = new GKeyboardSelectorMenu();
			m_pManager->PushMenu(pMenu);
		}
		break;

		case eR3:  // R3
		{
			GDisplayMenu* pMenu = new GDisplayMenu();
			pMenu->Initialize();
			m_pManager->PushMenu(pMenu);
		}
		break;

		case eL1:  // L1
			{
//					//not implemented yet
		//			GBarcodeScannerMenu* pMenu = new GBarcodeScannerMenu();
		//			pMenu->Initialize();
		//			m_pManager->PushMenu(pMenu);
//				}
			//	break;
			GGlobalSystemInformation* pSysInfo = GGlobalSystemInformation::GetInstance();
			if(pSysInfo)
			{
				if(pSysInfo->isVGDUnit())
				{
					LOG(eTrind,eDebug,"DevicePage1: Enable Trind\n");
					GTrindMenu* pMenu = new GTrindMenu();
					pMenu->Initialize();
					m_pManager->PushMenu(pMenu);
					break;
				}
			}
			EnableSoftKeys();

			}
			break;



		case eR4:  // Next Page
		{
			GGlobalSystemInformation* pSysInfo = GGlobalSystemInformation::GetInstance();
			if(pSysInfo)
			{
				if(pSysInfo->isVGDUnit())
				{
					GDeviceMenuPage2* pMenu = new GDeviceMenuPage2();
					m_pManager->PushMenu(pMenu);
					break;

				}
			}
			EnableSoftKeys();
		}
		break;

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

int GDeviceMenuPage1::GetTagStrings(GSoftKeysWindowTemplate::tStringTagMap & rMap)
{
	// Initialize map of tag-id/label pairs for initial window creation

//	Future " Note\n Acceptor" menu option.
//	m_MenuLabelsMap[GSoftKeysWindowTemplate::eSOFT_L1] = std::string(" Barcode\n Scanner");

	GGlobalSystemInformation* pSysInfo = GGlobalSystemInformation::GetInstance();

	if(pSysInfo)
	{
		if(!pSysInfo->isVGDUnit())
		{
			rMap[GSoftKeysWindowTemplate::eSOFT_L3] = std::string(" Contactless\n Reader");
		}
		else
		{
			rMap[GSoftKeysWindowTemplate::eSOFT_L1] = std::string(" TRIND");
			rMap[GSoftKeysWindowTemplate::eSOFT_L3] = std::string(" Secure\n Audio");
			rMap[GSoftKeysWindowTemplate::eSOFT_R4] = std::string(" Next \n Page ");
		}
	}
	rMap[GSoftKeysWindowTemplate::eSOFT_L2] = std::string(" Card\n Reader");
	rMap[GSoftKeysWindowTemplate::eSOFT_L4] = std::string(" Previous\n Menu");
	rMap[GSoftKeysWindowTemplate::eSOFT_R1] = std::string("Printer ");
	rMap[GSoftKeysWindowTemplate::eSOFT_R2] = std::string("Keyboards ");
	rMap[GSoftKeysWindowTemplate::eSOFT_R3] = std::string("Display ");



	return 6;
}
