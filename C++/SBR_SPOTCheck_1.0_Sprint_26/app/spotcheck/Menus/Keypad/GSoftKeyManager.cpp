#include <cstdio>
#include <cstring>

#include "GSoftKeyManager.h"
#include "GSoftKeyMenu.h"
#include "Services/Keypad/GKeypadService.h"
#include "Services/Resource/GResourcesService.h"

const char* GSoftKeyManager::SOFTKEY_MENU_TITLE = "SoftKey Diagnostics";

GSoftKeyManager::GSoftKeyManager(GSoftKeyMenu *pMenu)
	:  GAbsKeyboardManager(pMenu),
	   m_u8ExitKeyCounter(0)
{

	m_pTestWindow = new GKeyMatrixWindowFromResource(GResourcesService::GetService()->GetResourceIdForSoftKeys());
}


/**
 *
 * VIRTUAL METHODS
 *
 */

GSoftKeyManager::~GSoftKeyManager()
{
	if(m_pTestWindow)
		delete m_pTestWindow;
}

bool GSoftKeyManager::ActivateTest_KeypadAction()
{
	return true;
}

bool GSoftKeyManager::DeactivateTest_KeypadAction()
{
	return true;
}

bool GSoftKeyManager::InTest_SoftKeyAction()
{
	NotifySoftKey(GetLastKeyPressed());

	return true;
}

bool GSoftKeyManager::DeactivateTest_SoftKeyAction()
{
	//GSystem::Sleep(1000); // show L4 marked for one second
	// return true;

	if(GetLastKeyPressed() != GKeypadService::eKeyL4)
	{   // this if really weird (!=L4) but included for safety
		m_u8ExitKeyCounter = 0;
		return true;
	}

	if(m_u8ExitKeyCounter > 0)
	{
		m_u8ExitKeyCounter = 0;
		return true;
	}
	else // exit count not reached yet
	{
		m_u8ExitKeyCounter++;
		NotifySoftKey(GetLastKeyPressed());
		return false;
	}
}

bool GSoftKeyManager::KeepSoftKeysEnabled()
{
	if(m_pKBMenu)
		return m_pKBMenu->EnableSoftKeys();
	else
		return false;
}

bool GSoftKeyManager::isSuitableKeyCode(UINT8 u8KeyCode)
{
	return GKeypadService::isSoftKeyCode(u8KeyCode);
}


void GSoftKeyManager::MarkForStatistics()
{
	if( !isWindowCreated() || m_pTestWindow == NULL)
		return; // no graphics available

	const int CiMaxKeys = 1 + GKeypadService::eKeyR4 - GKeypadService::eKeyL1;
	UINT  uQty = 0;
	UINT8 u8KeyFrecuency = 0;
	UINT8 aryu8KeyCodes[CiMaxKeys];

	memset(aryu8KeyCodes, 0, sizeof(aryu8KeyCodes));

	for(int i = GKeypadService::eKeyL1; i <= GKeypadService::eKeyR4; i++)
	{
		u8KeyFrecuency = GetKeyStatistics( UINT8(i) );
		if( u8KeyFrecuency == 0 || u8KeyFrecuency == 0xFF )
			continue;

		aryu8KeyCodes[uQty++] = UINT8(i);
	}

	m_pTestWindow->MarkForStatistics(uQty, aryu8KeyCodes, true, true);
}

