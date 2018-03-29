/**
 * © 2013 Gilbarco Inc.
 * Confidential and Proprietary
 *
 * @file GSoftKeyMenu.h
 * @author Guillermo Paris
 * @date July 23 2013
 * @copyright © 2013 Gilbarco Inc. Confidential and Propietary
 */

#ifndef GSOFTKEYMENU_H
#define GSOFTKEYMENU_H 1

#include "GAbsKeyboardMenu.h"


class GSoftKeyMenu : public GAbsKeyboardMenu
{
	public:
		                 GSoftKeyMenu(void);
		virtual          ~GSoftKeyMenu(void);

		virtual void     Initialize();
		virtual bool     DrawMenu( bool blDisablePost = false );

		static  int      GetTagStrings(GSoftKeysWindowTemplate::tStringTagMap & rMap);


	private:
		virtual void     HandleSoftKeyEvent(UINT8 KeyCode);
};

#endif // GSOFTKEYMENU_H
