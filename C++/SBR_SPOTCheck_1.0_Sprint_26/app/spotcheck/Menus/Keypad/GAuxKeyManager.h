/**
 * © 2013 Gilbarco Inc.
 * Confidential and Proprietary
 *
 * @file GAuxKeyManager.h
 * @author Guillermo Paris
 * @date July 24 2013
 * @copyright © 2013 Gilbarco Inc. Confidential and Propietary
 */

#ifndef GAUXKEYMANAGER_h
#define GAUXKEYMANAGER_h 1

#include <GTypes.h>
#include "Menus/Templates/GKeyMatrixWindowFromResource.h"
#include "GAbsKeyboardManager.h"

class GAuxKeyMenu;

class GAuxKeyManager : public GAbsKeyboardManager
{
	public:

		                            GAuxKeyManager(GAuxKeyMenu *);
		virtual                     ~GAuxKeyManager();

		inline static const char*   GetMenuTitle();


	protected:

		       virtual bool         ActivateTest_KeypadAction();
		       virtual bool         DeactivateTest_KeypadAction();
		       virtual bool         InTest_SoftKeyAction();
		       virtual bool         DeactivateTest_SoftKeyAction();
		       virtual bool         KeepSoftKeysEnabled();
		       virtual bool         isSuitableKeyCode(UINT8);

	private:

		static const char*  AUXKEY_MENU_TITLE;
};


inline const char* GAuxKeyManager::GetMenuTitle()
{
	return AUXKEY_MENU_TITLE;
}


#endif // GAUXKEYMANAGER
