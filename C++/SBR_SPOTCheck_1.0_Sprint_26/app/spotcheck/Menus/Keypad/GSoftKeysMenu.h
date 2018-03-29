/*
 * GSoftKeysMenu.h
 *
 *  Created on: 08/07/2013
 *      Author: hstools
 */

#ifndef GSOFTKEYSMENU_H_
#define GSOFTKEYSMENU_H_

#include "Menus/GAbsMenu.h"
#include "Menus/Templates/GKeyMatrixWindowFromResource.h"

class GSoftKeysMenu: public GAbsMenu
{
public:
	GSoftKeysMenu();
	virtual ~GSoftKeysMenu();

	virtual bool DrawMenu( bool blDisablePost = false );
	virtual bool HandleSPOTMessage(GSPOTMessage* pMsg);

 private:
	virtual void HandleSoftKeyEvent(UINT8 KeyCode);

	bool                            m_blWindowCreated;
	GKeyMatrixWindowFromResource *  m_pTestWindow;
};

#endif /* GSOFTKEYSMENU_H_ */
