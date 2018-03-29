/*
 * �2012 Gilbarco Inc.
 * Confidential and Proprietary
 *
 */

#ifndef GMainMenu_H
#define GMainMenu_H 1

#include "Menus/GAbsMenu.h"
#include "Menus/Templates/GSoftKeysWindowTemplate.h"

class GMainMenu : public GAbsMenu
{
	public:
		enum eStatus
		{
			eInitialLocked, eUnLocked, eFinishing, eFatalError
		};

	public:
		            GMainMenu();
		            ~GMainMenu();

		       void FatalErrorLock();
		       void UnLockForNormalUse();
		       bool DrawMenu();
		       bool HandleSPOTMessage(GSPOTMessage* pMsg);

		static  int GetTagStrings(eStatus eSt, GSoftKeysWindowTemplate::tStringTagMap & rMap);

	private:
		       void HandleSoftKeyEvent(UINT8 KeyCode);
		       void BuildAndShowExitScreen();

		   eStatus  m_eStatus;
};

#endif
