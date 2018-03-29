/*!
 * © 2013 Gilbarco Inc.
 * Confidential and Proprietary
 *
 * @file GKeypadManager.h
 * @author Sebastian Lipchak
 * @date March 12 2013
 * @copyright © 2013 Gilbarco Inc. Confidential and Propietary
 */

#ifndef GKEYPADMANAGER_H
#define GKEYPADMANAGER_H 1

#include <GTypes.h>
#include "Menus/Templates/GKeyMatrixWindowFromResource.h"
#include "GAbsKeyboardManager.h"

class GKeypadMenu;

class GKeypadManager : public GAbsKeyboardManager
{
	public:

		                            GKeypadManager(GKeypadMenu *);
		virtual                     ~GKeypadManager();

		bool                        EnableDevice();
		bool                        DisableDevice();

		inline static const char*   GetMenuTitle();


	protected:

		       virtual bool         ActivateTest_KeypadAction();
		       virtual bool         DeactivateTest_KeypadAction();
		       virtual bool         InTest_SoftKeyAction();
		       virtual bool         DeactivateTest_SoftKeyAction();
		       virtual bool         KeepSoftKeysEnabled();
		       virtual bool         isSuitableKeyCode(UINT8);

	private:

		static const char*  KEYPAD_MENU_TITLE;
};


inline const char* GKeypadManager::GetMenuTitle()
{
	return KEYPAD_MENU_TITLE;
}


#endif // GKEYPADMANAGER_H
