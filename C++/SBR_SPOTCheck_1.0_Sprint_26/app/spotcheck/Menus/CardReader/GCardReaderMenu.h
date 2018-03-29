/*!
 * © 2012, 2013 Gilbarco Inc.
 * Confidential and Proprietary
 *
 * @file GCardReaderMenu.h
 * @author Guillermo Paris
 * @date Dec 28 2012
 * @copyright © 2012, 2013 Gilbarco Inc. Confidential and Propietary
 */

#ifndef GCardReaderMenu_H
#define GCardReaderMenu_H 1

#include "Menus/GAbsMenu.h"

class GCardReaderManager;

const int MAX_LINE_LEN = 33;

class GCardReaderMenu: public GAbsMenu
{
	public:
		GCardReaderMenu(void);
		virtual ~GCardReaderMenu(void);
		void Initialize();

		void ShowCardInsertNotification();
		void ShowChipCardDetected();
		void ShowCardRemoveNotification(const char* pcLabel1,const char* pcLabel2);
		void ShowDetailsScreen(const char* pcLabel1, const char* pcLabel2, const char* pcLabel3, const char* pcLabel4);
		void ShowStatsScreen();

		virtual bool DrawMenu();
		virtual bool HandleSPOTMessage(GSPOTMessage* pMsg);
		virtual void ClearScreen();

		static  int  GetTagStrings(GSoftKeysWindowTemplate::tStringTagMap & rMap);

	private:
		virtual void HandleSoftKeyEvent(UINT8 KeyCode);

		GCardReaderManager * m_pCRManager;

};

#endif // GGCardReaderMenu_H
