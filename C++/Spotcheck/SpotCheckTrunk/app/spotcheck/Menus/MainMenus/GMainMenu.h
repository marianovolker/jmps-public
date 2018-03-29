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
			eInitialLocked, eUnLocked, eFinishing, eResourceFatalError
		};

	public:
		            GMainMenu();
		            ~GMainMenu();

		       void ResourceFatalErrorLock();
		       void UnLockForNormalUse();
		       bool DrawMenu(bool blNotstackAction = false);

		       bool HandleSPOTMessage(GSPOTMessage* pMsg);

		static  int GetTagStrings(eStatus eStatus, GSoftKeysWindowTemplate::tStringTagMap & rMap);


	private:
		       void HandleSoftKeyEvent(UINT8 KeyCode);
		       bool HandleInactiveTimeEvent(UINT32 tLeft);

		       void BuildAndShowExitScreen();

		   eStatus  m_eStatus;
};

#endif
