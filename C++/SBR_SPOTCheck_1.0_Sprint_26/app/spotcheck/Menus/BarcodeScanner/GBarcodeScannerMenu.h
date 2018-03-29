/*
 * ©2012 Gilbarco Inc.
 * Confidential and Proprietary
 *
 */

#ifndef GBarcodeScannerMenu_H
#define GBarcodeScannerMenu_H 1

#include "Menus/GAbsMenu.h"
#include "GBarcodeScannerManager.h"

class GBarcodeScannerManager;

class GBarcodeScannerMenu: public GAbsMenu
{
	public:

		                 GBarcodeScannerMenu(void);
		virtual          ~GBarcodeScannerMenu(void);

		virtual bool     DrawMenu();
		virtual bool     HandleSPOTMessage(GSPOTMessage* pMsg);
		void             Quit();
		void             ClearScreen();
		void             Initialize();
		void             ShowResultsScreen(char* pcResult);
		void             ShowBarcodeSamplePrintingScreen();
		void             ShowStatsScreen();

		static  int      GetTagStrings(GSoftKeysWindowTemplate::tStringTagMap & rMap);

		static const int BCS_MAX_LINE_LEN;

	private:
		virtual void     HandleSoftKeyEvent(UINT8 KeyCode);

		GBarcodeScannerManager * m_pBCSManager;
};

#endif
