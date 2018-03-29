#include <cstdio>
#include <string>

#include "Common/Util/DEBUGOUT.h"
#include "GAuxKeyMenu.h"
#include "GAuxKeyManager.h"
#include "Services/Keypad/GKeypadService.h"

GAuxKeyMenu::GAuxKeyMenu()
{
	m_iWinID = eWinId_AuxKeyMenu;

	//TODO: Eliminate this when repository is working OK
	GetTagStrings(m_MenuLabelsMap);
}

/**
 *
 * VIRTUAL METHODS
 *
 */

GAuxKeyMenu::~GAuxKeyMenu()
{
}

void GAuxKeyMenu::Initialize()
{
	m_pKBManager = new GAuxKeyManager(this);
	m_pKBManager->Initialize();
}

bool GAuxKeyMenu::DrawMenu( bool blDisablePost /* = false */ )
{
	return GAbsKeyboardMenu::DrawMenu( blDisablePost );
}

void GAuxKeyMenu::HandleSoftKeyEvent(UINT8 u8KeyCode)
{
	bool blok = true;
	const char* pcMethod = "GAuxKeysMenu::HandleSoftKeyEvent() - ";

	LOG(eKeypad, eTrace, "%sEntry with param 0x%02X.\n", pcMethod, UINT(u8KeyCode));

	if(u8KeyCode >= eP1 && u8KeyCode <= eP10) // auxiliary (pump) key code
	{
		UINT8 u8TraslatedCode = 0xFF;
		GKeypadService* pKPService = GKeypadService::GetService();
		blok = pKPService->KeyTranslate(u8KeyCode, u8TraslatedCode);

		if(blok)
		{
			LOG(eKeypad, eDebug, "%sMark P%d key as pressed.\n", pcMethod, u8KeyCode);
			u8KeyCode = u8TraslatedCode;
		}
	}

	if(m_pKBManager->GetMenuStatus() == GAbsKeyboardManager::eShowTest)
	{
		if(blok)
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

int GAuxKeyMenu::GetTagStrings(GSoftKeysWindowTemplate::tStringTagMap & rMap)
{
	return GAbsKeyboardMenu::GetTagStrings(rMap);
}

