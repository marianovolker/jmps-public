/*
 * ©2012 Gilbarco Inc.
 * Confidential and Proprietary
 *
 */

#ifndef GDeviceMenuPage2_H
#define GDeviceMenuPage2_H 1

#include "Menus/GAbsMenu.h"


class GDeviceMenuPage2 : public GAbsMenu
{
	public:
		              GDeviceMenuPage2();
		virtual       ~GDeviceMenuPage2();

		virtual bool  DrawMenu();
		virtual bool  HandleSPOTMessage(GSPOTMessage* pMsg);

		static  int   GetTagStrings(GSoftKeysWindowTemplate::tStringTagMap & rMap);

	private:
		virtual void  HandleSoftKeyEvent(UINT8 KeyCode);
};

#endif
