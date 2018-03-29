/**
 * GKeyboardSelectorMenu.cpp
 *
 *  Created on: 04/07/2013
 *      Author: Guillermo Paris
 */

#include "GKeypadMenu.h"
#include "GSoftKeyMenu.h"
#include "GAuxKeyMenu.h"
#include "GKeyboardSelectorMenu.h"
#include "Services/Resource/GResourcesService.h"
#include "SPOT/SystemInformation/GGlobalSystemInformation.h"


GKeyboardSelectorMenu::GKeyboardSelectorMenu()
{
		m_MenuLabelsMap[GSoftKeysWindowTemplate::eSOFT_L1] = std::string(" Main\n Keypad");
		m_MenuLabelsMap[GSoftKeysWindowTemplate::eSOFT_L2] = std::string(" Soft\n Keys");
		if(!GGlobalSystemInformation::GetInstance()->isVGDUnit())
		{
			m_MenuLabelsMap[GSoftKeysWindowTemplate::eSOFT_L3] = std::string(" Auxiliary\n Keys");
		}
		else
		{
			m_MenuLabelsMap[GSoftKeysWindowTemplate::eSOFT_L3] = std::string(" ADA\n Keypad");
		}
		m_MenuLabelsMap[GSoftKeysWindowTemplate::eSOFT_L4] = std::string(" Previous\n Menu");

		m_MenuLabelsMap[GSoftKeysWindowTemplate::eSEGMENT_1] = "Keyboard test selector.";
}

GKeyboardSelectorMenu::~GKeyboardSelectorMenu()
{
}


bool GKeyboardSelectorMenu::DrawMenu( bool blDisablePost /* = false */)
{
	return GAbsMenu::DrawMenu( blDisablePost );
}

bool GKeyboardSelectorMenu::HandleSPOTMessage(GSPOTMessage* pMsg)
{
   // this menu only responds to soft key events, which come through "HandleSoftKeyEvent(KeyCode)"
   return true;
}

void GKeyboardSelectorMenu::HandleSoftKeyEvent(UINT8 KeyCode)
{
	switch (KeyCode)
	{
		case eL1:  // keypad menu
		{
			GKeypadMenu* pMenu = new GKeypadMenu();
			pMenu->Initialize();
			m_pManager->PushMenu(pMenu);
		}
		break;

		case eL2:  // soft keys menu
		{
			GSoftKeyMenu* pMenu = new GSoftKeyMenu();
			pMenu->Initialize();
			m_pManager->PushMenu(pMenu);
		}
		break;


		case eL3:
			{
				GAuxKeyMenu* pMenu = new GAuxKeyMenu();
				pMenu->Initialize();
				m_pManager->PushMenu(pMenu);
			}


		break;

		case eL4:  // L4 - back to previous menu
			m_pManager->QueueMenuPop();
		break;

		case eR1: // unused for now
		case eR2:
		case eR3:
		case eR4:
		default:
			EnableSoftKeys();
		break;
	}
}

