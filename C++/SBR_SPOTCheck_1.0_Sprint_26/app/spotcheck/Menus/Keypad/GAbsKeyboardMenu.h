/**
 * © 2013 Gilbarco Inc.
 * Confidential and Proprietary
 *
 * @file GAbsKeyboardMenu.h
 * @author Guillermo Paris
 * @date July 22 2013
 * @copyright © 2013 Gilbarco Inc. Confidential and Propietary
 */

#ifndef GAbsKeyboardMenu_H
#define GAbsKeyboardMenu_H 1

#include "Menus/GAbsMenu.h"
#include "Menus/GMenuManager.h"

class GAbsKeyboardManager;

class GAbsKeyboardMenu : public GAbsMenu
{
	public:
		                 GAbsKeyboardMenu(void);
		virtual          ~GAbsKeyboardMenu(void);

		virtual void     Initialize() = 0;
		inline void      QueueMenuPop();

		virtual bool     DrawMenu( bool blDisablePost = false );
		        bool     HandleSPOTMessage(GSPOTMessage* pMsg);

		static  int      GetTagStrings(GSoftKeysWindowTemplate::tStringTagMap & rMap);

	protected:

		virtual void     HandleSoftKeyEvent(UINT8 KeyCode);

		GAbsKeyboardManager * m_pKBManager;
};

inline void GAbsKeyboardMenu::QueueMenuPop()
{
	m_pManager->QueueMenuPop();
}

#endif // GAbsKeyboardMenu_H
