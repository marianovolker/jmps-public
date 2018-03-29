/*
 * �2012 Gilbarco Inc.
 * Confidential and Proprietary
 *
 */

#ifndef GContactlessReaderMenu_H
#define GContactlessReaderMenu_H 1

#include "Menus/GAbsMenu.h"
#include "GContactlessReaderManager.h"

class GContactlessReaderManager;


class GContactlessReaderMenu: public GAbsMenu
{
public:

	GContactlessReaderMenu(void);
	virtual ~GContactlessReaderMenu(void);

	virtual bool     DrawMenu();
	virtual bool     HandleSPOTMessage(GSPOTMessage* pMsg);
	void             Quit();
	void             ClearScreen();
	void             Initialize();
	void             ShowResultsScreen(char* pcResult);
	void             ShowStatsScreen();

	static  int      GetTagStrings(GSoftKeysWindowTemplate::tStringTagMap & rMap);

	static const int CLESS_MAX_LINE_LEN;


private:
	virtual void      HandleSoftKeyEvent(UINT8 KeyCode);

	GContactlessReaderManager * m_pCLRManager;
};

#endif // GGContactlessReaderMenu_H
