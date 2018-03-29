#include <cstdio>
#include <cstring>

#include "GAuxKeyManager.h"
#include "GAuxKeyMenu.h"
#include "Services/Keypad/GKeypadService.h"
#include "Services/Resource/GResourcesService.h"
#include "SPOT/SystemInformation/GGlobalSystemInformation.h"


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
	if(GetLastKeyPressed() != GKeypadService::eKeyL4)
	{   // this if really weird (!=L4) but included for safety
		m_u8ExitKeyCounter = 0;
		return true;
	}

	if(!GGlobalSystemInformation::GetInstance()->isVGDUnit())
	{
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

bool GAuxKeyManager::KeepSoftKeysEnabled()
{
	//if(!GGlobalSystemInformation::GetInstance()->isVGDUnit())
	{
		if(m_pKBMenu)
		{
			return m_pKBMenu->EnableSoftKeys(true); // true: enable auxiliary keys too
		}
		else
		{
			return false;
		}
	}
/*	else
	{
		if(m_pKBMenu)
		{
			return m_pKBMenu->EnableSoftKeys(false);// false: disable auxiliary keys
		}
		else
		{
			return false;
		}
	}*/

}

bool GAuxKeyManager::isSuitableKeyCode(UINT8 u8KeyCode)
{
	bool bIsVGD = GGlobalSystemInformation::GetInstance()->isVGDUnit();

	if(bIsVGD)
	{
		return (GKeypadService::isADASoftKeyCode(u8KeyCode) || GKeypadService::isAuxKeyCode(u8KeyCode)) ;
	}
	return GKeypadService::isAuxKeyCode(u8KeyCode);
}


void GAuxKeyManager::MarkForStatistics()
{
	if( !m_blWindowCreated || m_pTestWindow == NULL)
		return; // no graphics available

	UINT  uQty = 0;
	UINT8 u8KeyFrecuency = 0;
	UINT8 aryu8KeyCodes[GKeypadService::eMaxKeys];

	memset(aryu8KeyCodes, 0, GKeypadService::eMaxKeys);

	for(int i = 0; i < GKeypadService::eMaxKeys; i++)
	{
		if( isSuitableKeyCode(UINT8(i)) )
		{
			u8KeyFrecuency = GetKeyStatistics( UINT8(i) );
			if( u8KeyFrecuency == 0 || u8KeyFrecuency == 0xFF )
				continue;
			if(GKeypadService::isADASoftKeyCode(UINT8(i)))
			{
				aryu8KeyCodes[uQty++] = UINT8(i - GKeypadService::eKeyL4);
			}
			else
			{
				aryu8KeyCodes[uQty++] = UINT8(i);
			}
		}
	}

	m_pTestWindow->MarkForStatistics(uQty, aryu8KeyCodes, true, true);
}


void GAuxKeyManager::NotifySoftKey(UINT8 u8KeyCode)
{
	bool bIsVGD = GGlobalSystemInformation::GetInstance()->isVGDUnit();
	if(bIsVGD && GKeypadService::isSoftKeyCode(u8KeyCode))
	{
		GAbsKeyboardManager::NotifySoftKey(u8KeyCode + (UINT8)GKeypadService::eKeyL4);
	}
	else
	{
		GAbsKeyboardManager::NotifySoftKey(u8KeyCode);
	}
}
