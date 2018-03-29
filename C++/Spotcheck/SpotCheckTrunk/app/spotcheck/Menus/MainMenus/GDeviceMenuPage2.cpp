#include "Common/Util/DEBUGOUT.h"
#include "Menus/MainMenus/GDeviceMenuPage2.h"
#include "Menus/BNA/GBNAMainMenu.h"
#include "Menus/CardReader/GCardReaderMenu.h"

//#include "Menus/BarcodeScanner/GBarcodeScannerMenu.h"
#include "Menus/ContactlessReader/GContactlessReaderMenu.h"
#include "Menus/Printer/GPrinterMenu.h"
#include "Menus/SecureAudio/GSecureAudioMenu.h"
#include "Menus/Keypad/GKeyboardSelectorMenu.h"
#include "Menus/Display/GDisplayMenu.h"
#include "Menus/FeatureConnector/GFeatureConnectorMenu.h"
#include "SPOT/SystemInformation/GGlobalSystemInformation.h"
#include "GSystem.h"
#include "GTypes.h"

using namespace GSpotDefs;

GDeviceMenuPage2::GDeviceMenuPage2()
{
	m_iWinID = eWinId_DeviceMenu;

	//TODO: Eliminate this when repository is working OK
	GetTagStrings(m_MenuLabelsMap);
}

GDeviceMenuPage2::~GDeviceMenuPage2()
{
}

bool GDeviceMenuPage2::DrawMenu()
{
	return GAbsMenu::DrawMenu();
}

bool GDeviceMenuPage2::HandleSPOTMessage(GSPOTMessage* pMsg)
{
   // this menu only responds to soft key events, which come through "HandleSoftKeyEvent(KeyCode)"
   return true;
}

void GDeviceMenuPage2::HandleSoftKeyEvent(UINT8 KeyCode)
{
	switch (KeyCode)
	{

		case eL1:  // L1
		{
			GFeatureConnectorMenu* pMenu = new GFeatureConnectorMenu();
			m_pManager->PushMenu(pMenu);

		}
		break;

		case eL4:  // L4 - back to previous menu
		{
			m_pManager->QueueMenuPop();
			break;
		}

		case eL2:
		case eL3:
		case eR1:
		case eR2:
		case eR3:
		case eR4:  // unused (for now, until a third device page is added)
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

int GDeviceMenuPage2::GetTagStrings(GSoftKeysWindowTemplate::tStringTagMap & rMap)
{
	// Initialize map of tag-id/label pairs for initial window creation

	rMap[GSoftKeysWindowTemplate::eSOFT_L1] = std::string(" Feature\n Connector");
	rMap[GSoftKeysWindowTemplate::eSOFT_L4] = std::string(" Previous\n Page");

	return 2;
}
