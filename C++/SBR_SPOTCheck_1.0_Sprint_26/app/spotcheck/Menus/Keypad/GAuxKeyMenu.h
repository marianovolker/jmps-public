/**
 * © 2013 Gilbarco Inc.
 * Confidential and Proprietary
 *
 * @file GAuxKeyMenu.h
 * @author Guillermo Paris
 * @date July 24 2013
 * @copyright © 2013 Gilbarco Inc. Confidential and Propietary
 */

#ifndef GAUXKEYMENU_H
#define GAUXKEYMENU_H 1

#include "GAbsKeyboardMenu.h"


class GAuxKeyMenu : public GAbsKeyboardMenu
{
	public:
		                 GAuxKeyMenu(void);
		virtual          ~GAuxKeyMenu(void);

		virtual void     Initialize();
		virtual bool     DrawMenu( bool blDisablePost = false );

		static  int      GetTagStrings(GSoftKeysWindowTemplate::tStringTagMap & rMap);


	private:
		virtual void     HandleSoftKeyEvent(UINT8 KeyCode);
};

#endif // GAUXKEYMENU_H
