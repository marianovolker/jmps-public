/*
 * ©2012 Gilbarco Inc.
 * Confidential and Proprietary
 *
 */

#ifndef GKEYPADMENU_H
#define GKEYPADMENU_H 1

#include "GAbsKeyboardMenu.h"


class GKeypadMenu : public GAbsKeyboardMenu
{
	public:
		                 GKeypadMenu(void);
		virtual          ~GKeypadMenu(void);

		virtual void     Initialize();
		virtual bool     DrawMenu( bool blDisablePost = false );

		static  int      GetTagStrings(GSoftKeysWindowTemplate::tStringTagMap & rMap);

	private:
		virtual void     HandleSoftKeyEvent(UINT8 KeyCode);
};

#endif // GKEYPADMENU_H
