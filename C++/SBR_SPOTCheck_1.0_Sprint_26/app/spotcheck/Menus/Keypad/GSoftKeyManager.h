/**
 * © 2013 Gilbarco Inc.
 * Confidential and Proprietary
 *
 * @file GSoftKeyManager.h
 * @author Guillermo Paris
 * @date July 23 2013
 * @copyright © 2013 Gilbarco Inc. Confidential and Propietary
 */

#ifndef GSOFTKEYMANAGER_h
#define GSOFTKEYMANAGER_h 1

#include <GTypes.h>
#include "Menus/Templates/GKeyMatrixWindowFromResource.h"
#include "GAbsKeyboardManager.h"

class GSoftKeyMenu;

class GSoftKeyManager : public GAbsKeyboardManager
{
	public:

		                            GSoftKeyManager(GSoftKeyMenu *);
		virtual                     ~GSoftKeyManager();

		inline static const char*   GetMenuTitle();


	protected:

		       virtual bool         ActivateTest_KeypadAction();
		       virtual bool         DeactivateTest_KeypadAction();
		       virtual bool         InTest_SoftKeyAction();
		       virtual bool         DeactivateTest_SoftKeyAction();
		       virtual bool         KeepSoftKeysEnabled();
		       virtual bool         isSuitableKeyCode(UINT8);
		       virtual void         MarkForStatistics();

	private:

		static const char*  SOFTKEY_MENU_TITLE;

		UINT8               m_u8ExitKeyCounter; // counter for L4 (exit soft-key)
};


inline const char* GSoftKeyManager::GetMenuTitle()
{
	return SOFTKEY_MENU_TITLE;
}


#endif // GSOFTKEYMANAGER
