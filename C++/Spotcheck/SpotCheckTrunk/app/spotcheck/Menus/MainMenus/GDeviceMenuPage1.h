/*
 * ©2012 Gilbarco Inc.
 * Confidential and Proprietary
 *
 */

#ifndef GDeviceMenuPage1_H
#define GDeviceMenuPage1_H 1

#include "Menus/GAbsMenu.h"


class GDeviceMenuPage1 : public GAbsMenu
{
	public:
		              GDeviceMenuPage1();
		virtual       ~GDeviceMenuPage1();

		virtual bool  DrawMenu();
		virtual bool  HandleSPOTMessage(GSPOTMessage* pMsg);

		static  int   GetTagStrings(GSoftKeysWindowTemplate::tStringTagMap & rMap);

	private:
		virtual void  HandleSoftKeyEvent(UINT8 KeyCode);
};

#endif
