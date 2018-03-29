/**
 * © 2013 Gilbarco Inc.
 * Confidential and Proprietary
 *
 * @file GKeyboardSelectorMenu.h
 * @author Guillermo Paris
 * @date Jul 04 2013
 * @copyright © 2013 Gilbarco Inc. Confidential and Proprietary
 */

#ifndef GKEYBOARDSELECTORMENU_H_
#define GKEYBOARDSELECTORMENU_H_

#include "Menus/GAbsMenu.h"

class GKeyboardSelectorMenu: public GAbsMenu
{
public:
	             GKeyboardSelectorMenu();
	virtual      ~GKeyboardSelectorMenu();

	virtual bool DrawMenu( bool blDisablePost = false );
	virtual bool HandleSPOTMessage(GSPOTMessage* pMsg);

 private:
	virtual void HandleSoftKeyEvent(UINT8 KeyCode);
};

#endif /* GKEYBOARDSELECTORMENU_H_ */
