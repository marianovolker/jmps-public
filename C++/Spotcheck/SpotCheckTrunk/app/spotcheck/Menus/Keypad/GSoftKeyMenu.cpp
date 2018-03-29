#include <cstdio>
#include <string>

#include "Common/Util/DEBUGOUT.h"
#include "GSoftKeyMenu.h"
#include "GSoftKeyManager.h"


GSoftKeyMenu::GSoftKeyMenu()
{
	m_iWinID = eWinId_SoftKeyMenu;

	//TODO: Eliminate this when repository is working OK
	GetTagStrings(m_MenuLabelsMap);
}

/**
 *
 * VIRTUAL METHODS
 *
 */

GSoftKeyMenu::~GSoftKeyMenu()
{
}

void GSoftKeyMenu::Initialize()
{
	m_pKBManager = new GSoftKeyManager(this);
	m_pKBManager->Initialize();
}

bool GSoftKeyMenu::DrawMenu( bool blDisablePost /* = false */ )
{
	return GAbsKeyboardMenu::DrawMenu( blDisablePost );
}

void GSoftKeyMenu::HandleSoftKeyEvent(UINT8 u8KeyCode)
{
	const char* pcMethod = "GSoftKeysMenu::HandleSoftKeyEvent() - ";

	LOG(eKeypad, eTrace, "%sEntry with param 0x%02X.\n", pcMethod, UINT(u8KeyCode));

	char ckey = (u8KeyCode < eR1 ? 'L' : 'R');

	LOG(eKeypad, eDebug, "%sMark %c%d key as pressed.\n",
	    pcMethod, ckey, u8KeyCode - (eL1 - 1) - (u8KeyCode < eR1 ? 0 : 4));

	if(m_pKBManager->GetMenuStatus() == GAbsKeyboardManager::eShowTest)
	{
		m_pKBManager->SetLastKeyPressed(u8KeyCode); // remember it for coming actions
													// inside the manager.
		if(u8KeyCode != eL4)
		{
			m_pKBManager->ProposeChangeStateTo(GAbsKeyboardManager::eShowTest);
		}
		else // L4 to exit test
		{
			m_pKBManager->ProposeChangeStateTo(GAbsKeyboardManager::eShowInitial);
		}

		return;
	}

	GAbsKeyboardMenu::HandleSoftKeyEvent(u8KeyCode);
}



/**
 *
 * STATIC METHODS
 *
 */

int GSoftKeyMenu::GetTagStrings(GSoftKeysWindowTemplate::tStringTagMap & rMap)
{
	// initialize map of tag-id/label pairs for initial window creation
	return GAbsKeyboardMenu::GetTagStrings(rMap);
}

