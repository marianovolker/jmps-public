#include <cstdio>
#include <cstring>

#include "GAuxKeyManager.h"
#include "GAuxKeyMenu.h"
#include "Services/Keypad/GKeypadService.h"
#include "Services/Resource/GResourcesService.h"


const char* GAuxKeyManager::AUXKEY_MENU_TITLE = "SoftKey Diagnostics";

GAuxKeyManager::GAuxKeyManager(GAuxKeyMenu *pMenu)
	:  GAbsKeyboardManager(pMenu)
{
	m_pTestWindow = new GKeyMatrixWindowFromResource(GResourcesService::GetService()->GetResourceIdForAuxKeys());
}


/**
 *
 * VIRTUAL METHODS
 *
 */

GAuxKeyManager::~GAuxKeyManager()
{
	if(m_pTestWindow)
		delete m_pTestWindow;
}

bool GAuxKeyManager::ActivateTest_KeypadAction()
{
	return true;
}

bool GAuxKeyManager::DeactivateTest_KeypadAction()
{
	return true;
}

bool GAuxKeyManager::InTest_SoftKeyAction()
{
	NotifySoftKey(GetLastKeyPressed());

	return true;
}

bool GAuxKeyManager::DeactivateTest_SoftKeyAction()
{
	if(GetLastKeyPressed() == GKeypadService::eKeyL4)
		return true;
	else
		return false;
}

bool GAuxKeyManager::KeepSoftKeysEnabled()
{
	if(m_pKBMenu)
		return m_pKBMenu->EnableSoftKeys(true); // true: enable auxiliary keys too
	else
		return false;
}

bool GAuxKeyManager::isSuitableKeyCode(UINT8 u8KeyCode)
{
	return GKeypadService::isAuxKeyCode(u8KeyCode);
}


