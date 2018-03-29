/**
 * GSoftKeysMenu.cpp
 *
 *  Created on: 08/07/2013
 *      Author: Guillermo Paris
 */

#include "Common/Util/DEBUGOUT.h"
#include "Services/Keypad/GKeypadService.h"
#include "GSoftKeysMenu.h"
#include "Services/Resource/GResourcesService.h"


GSoftKeysMenu::GSoftKeysMenu()
	: m_blWindowCreated(false)
{

	m_pTestWindow = new GKeyMatrixWindowFromResource(GResourcesService::GetService()->GetResourceIdForSoftKeys());
}

GSoftKeysMenu::~GSoftKeysMenu()
{
	if(m_pTestWindow)
		delete m_pTestWindow;
}


bool GSoftKeysMenu::DrawMenu(bool blDisablePost /* = false */)
{
	if(!m_blWindowCreated)
	{
		m_pTestWindow->CreateWindow();
		m_blWindowCreated = true;
	}

	EnableSoftKeys();
	return true;
}

bool GSoftKeysMenu::HandleSPOTMessage(GSPOTMessage* pMsg)
{
	// this menu only responds to soft key events, which come through "HandleSoftKeyEvent(KeyCode)"
	return true;
}

void GSoftKeysMenu::HandleSoftKeyEvent(UINT8 u8KeyCode)
{
	const char* pcMethod = "GSoftKeysMenu::HandleSoftKeyEvent() - ";

	LOG(eKeypad, eTrace, "%sEntry with param 0x%02X.\n", pcMethod, UINT(u8KeyCode));

	char ckey = (u8KeyCode < eR1 ? 'L' : 'R');
	m_pTestWindow->MarkKeyPressed(u8KeyCode);
	GKeypadService::GetService()->NotifySoftKey(u8KeyCode);

	LOG(eKeypad, eDebug, "%sMark %c%d key as pressed.\n",
	    pcMethod, ckey, u8KeyCode - (eL1 - 1) - (u8KeyCode < eR1 ? 0 : 4));

	switch (u8KeyCode)
	{
		case eL4:  // L4 - back to previous menu
			GSystem::Sleep(1000); // show L1 marked for one second
			m_pManager->QueueMenuPop();
		break;

		case eL1:  // all unused
		case eL2:
		case eL3:
		case eR1:
		case eR2:
		case eR3:
		case eR4:
		default:
			EnableSoftKeys();
		break;
	}
}


