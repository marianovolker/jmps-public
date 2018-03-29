#include <cstdio>
#include <string>

#include "Common/Util/DEBUGOUT.h"
#include "GAbsKeyboardMenu.h"
#include "Menus/Keypad/GAbsKeyboardManager.h"
#include "SPOT/SystemInformation/GGlobalSystemInformation.h"



GAbsKeyboardMenu::GAbsKeyboardMenu() : m_pKBManager(NULL)
{
}


/**
 *
 * VIRTUAL METHODS
 *
 */

GAbsKeyboardMenu::~GAbsKeyboardMenu()
{
	if (m_pKBManager)
	{
		m_pKBManager->Dispose();
		delete m_pKBManager;
		m_pKBManager = NULL;
	}
}

/**
 * @note Virtual CHAIN IS BROKEN in this method to assure this message don't reach
 * and disturb the descendant's logic of service message treatment.
 * This menu family (keypad and soft & aux keys) ONLY receive SPOT messages
 * FROM the SERVICE except for soft-key notifications.
 */
bool GAbsKeyboardMenu::HandleSPOTMessage(GSPOTMessage* pMsg)
{
	return true;
}

bool GAbsKeyboardMenu::DrawMenu( bool blDisablePost /* = false */ )
{
	if( m_pKBManager->GetMenuStatus() == GAbsKeyboardManager::eShowInitial )
		return GAbsMenu::DrawMenu( blDisablePost );

	LOG(eKeypad, eDebug, "GAbsKeyboardMenu::DrawMenu() - Soft-keys enable.\n");
	EnableSoftKeys();
	return true;
}

void GAbsKeyboardMenu::HandleSoftKeyEvent(UINT8 KeyCode)
{
	switch (KeyCode)
	{
		case eL1:  // L1  show test screen
		           //change state to test window
			m_pKBManager->ProposeChangeStateTo(GAbsKeyboardManager::eShowTest);
			break;

		case eL3:  // L3  show statistics
		           //change state to statistics window
			m_pKBManager->ProposeChangeStateTo(GAbsKeyboardManager::eShowStats);
			break;

		case eL4:  // L4 - back to previous menu
			//back to device's menu
			m_pKBManager->ProposeChangeStateTo(GAbsKeyboardManager::eShowInitial);
			break;

		case eL2:  // L2
		case eR1:  // R1
		case eR2:  // R2
		case eR3:  // R3
		case eR4:  // R4
		default:
			//Enable soft keys in all these cases
			EnableSoftKeys();
			break;
	}
}


/**
 *
 * STATIC METHODS
 *
 */

int GAbsKeyboardMenu::GetTagStrings(GSoftKeysWindowTemplate::tStringTagMap & rMap)
{
	// initialize map of tag-id/label pairs for initial window creation
	rMap[GSoftKeysWindowTemplate::eSOFT_L1] = std::string(" Begin\n Test");
	rMap[GSoftKeysWindowTemplate::eSOFT_L3] = std::string(" Show\n Statistics");
	rMap[GSoftKeysWindowTemplate::eSOFT_L4] = std::string(" Previous\n Menu");

	return 3;
}



