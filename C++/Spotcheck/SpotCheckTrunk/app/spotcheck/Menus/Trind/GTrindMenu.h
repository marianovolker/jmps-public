/*!
 * © 2015, 2016 Gilbarco Inc.
 * Confidential and Proprietary
 *
 * @file GTrindMenu.h
 * @author Sayyed Mohammad
 * @date Feb 10 2015
 * @copyright © 2015, 2016 Gilbarco Inc. Confidential and Propietary
 */

#ifndef GTrindMenu_H
#define GTrindMenu_H 1

#include "Menus/GAbsMenu.h"

class GTrindManager;

const int MAX_LINE_LEN2 = 33;



class GTrindMenu: public GAbsMenu
{
	public:

	enum eTagResult
	{
		eTrPass,
		eTrFail,
	};

	enum eTRLEDResult  //Used for Trind LED Results
	{
		eTRInitial,
		eTRPass,
		eTRFail,

		eTRLast = eTRFail
	};

		GTrindMenu(void);
		virtual ~GTrindMenu(void);
		void Initialize();

		void ShowTagRead();
		void IncrementTagCount(eTagResult result);
		void SetLEDTestResult(eTRLEDResult);
		void ShowStatsScreen();
		bool DrawTrindLEDCheckTestMenu(std::string text);

		virtual bool DrawMenu();
		virtual bool HandleSPOTMessage(GSPOTMessage* pMsg);
		virtual void ClearScreen();

		static  int  GetTagStrings(GSoftKeysWindowTemplate::tStringTagMap & rMap);
		static const char* m_arpcTRLEDLabels[eTRLast +1 ];

	private:
		virtual void HandleSoftKeyEvent(UINT8 KeyCode);

		GTrindManager *   m_pTRManager;
		int               m_TagPassCount;
		int               m_TagFailCount;
		enum eTRLEDResult m_eTRLEDResult;

};

#endif // GGTrindMenu_H
