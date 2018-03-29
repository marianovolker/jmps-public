#include <cstdio>
#include <cstring>

#include "Common/Util/DEBUGOUT.h"
#include "GKeypadManager.h"
#include "Menus/Keypad/GKeypadMenu.h"
#include "Services/Keypad/GKeypadService.h"
#include "SPOT/SystemInformation/GGlobalSystemInformation.h"
#include "Services/Resource/GResourcesService.h"


const char* GKeypadManager::KEYPAD_MENU_TITLE = "Keypad Diagnostics";

bool GKeypadManager::EnableDevice()
{
	if(m_pKPService == NULL)
		return false;

	return m_pKPService->EnableRequest();
}

bool GKeypadManager::DisableDevice()
{
	if(m_pKPService == NULL)
		return false;

	return m_pKPService->DisableRequest();
}

GKeypadManager::GKeypadManager(GKeypadMenu *pMenu)
	:  GAbsKeyboardManager(pMenu)
{

	UINT8 u8ResId = GResourcesService::GetService()->GetResourceIdForKeyMatrix();
	GGlobalSystemInformation* pSysInfo = GGlobalSystemInformation::GetInstance();

	m_pTestWindow = new GKeyMatrixWindowFromResource(u8ResId);
}


/**
 *
 * VIRTUAL METHODS
 *
 */

GKeypadManager::~GKeypadManager()
{
	if(m_pTestWindow)
		delete m_pTestWindow;
}

bool GKeypadManager::ActivateTest_KeypadAction()
{
	return EnableDevice();
}

bool GKeypadManager::DeactivateTest_KeypadAction()
{
	return DisableDevice();
}

bool GKeypadManager::InTest_SoftKeyAction()
{
	return true;
}

bool GKeypadManager::DeactivateTest_SoftKeyAction()
{
	return true;
}

bool GKeypadManager::KeepSoftKeysEnabled()
{
	if(m_pKBMenu)
		return m_pKBMenu->EnableSoftKeys();
	else
		return false;
}

inline bool GKeypadManager::isSuitableKeyCode(UINT8 u8KeyCode)
{
	return GKeypadService::isKeypadKeyCode(u8KeyCode);
}


