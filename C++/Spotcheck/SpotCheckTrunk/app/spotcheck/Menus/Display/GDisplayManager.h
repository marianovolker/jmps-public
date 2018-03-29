/*!
 * © 2013 Gilbarco Inc.
 * Confidential and Proprietary
 *
 * @file GDisplayManager.h
 * @author Mariano Volker
 * @date June 3 2013
 * @copyright © 2013 Gilbarco Inc. Confidential and Propietary
 */

#ifndef GDisplayManager_h
#define GDisplayManager_h 1

#include "Common/Util/GSimpleCommand.h"
#include "Common/Util/GTimer.h"
#include "Services/Display/GDisplayServiceStatistics.h"

const int DY_TOUT_DISPLAY_TEST_DELAY		=  2000; // 2 Seconds
const int DY_TOUT_DISPLAY_GET_ANSWER_DELAY  =  60000; // 60 Seconds

class GDisplayMenu;
class GDisplayService;

class GDisplayManager
{
	public:
		GDisplayManager(GDisplayMenu *);
		virtual ~GDisplayManager();

		enum eEvents
		{
			eEventSoftKeyL1 = 0,
			eEventSoftKeyL2    ,
			eEventSoftKeyL3    ,
			eEventSoftKeyL4    ,

			eEventSoftKeyR1    ,
			eEventSoftKeyR2    ,
			eEventSoftKeyR3    ,
			eEventSoftKeyR4    ,

			eEventTimeout      ,
			eEventError        ,

			eLastEvent = (eEventError+1)
		};

		enum eStatus
		{
			eStatusInitial,
			eStatusTest_1,
			eStatusCheckTest_1,
			eStatusTest_2,
			eStatusCheckTest_2,
			eStatusTest_3,
			eStatusCheckTest_3,
			eStatusTest_4,
			eStatusCheckTest_4,
			eStatusTest_5,
			eStatusCheckTest_5,
			eStatusError,
			eStatusTestResult,
			eStatusSummary,

			eLastStatus = (eStatusSummary+1)
		};

		                      void ProcessEventReceived(eEvents e);
					          void Initialize();
					          void Dispose();

	private:

		             GDisplayMenu* m_pDYMenu;
		          GDisplayService* m_pDYService;
		                   GTimer* m_pTimer;
   GSimpleCommand<GDisplayManager> m_TOutCommand;
		      enum eStatus         m_Status;

		       static const char * mst_arypcStatusLabel	[eLastStatus];
		       static const char * mst_arypcEventLabel	[eLastEvent	];

		                      void NextState(eEvents e, eStatus s);

		                      bool StopTimer();
		                      bool StartTimer(UINT16 timeout=DY_TOUT_DISPLAY_TEST_DELAY);

                              void FirstTest();
                              void SecondTest();
                              void ThirdTest();
                              void FourthTest();
                              void FifthTest();

                              void TemporizedDisplayTest();
                              bool BuildAndShowPlainScreen(UINT8 u8Red, UINT8 u8Green, UINT8 u8Blue);
                              bool DestroyAndHidePlainScreen();
                              bool DrawCheckTestMenu(std::string s);
                              void IncrementPassStatistics(GDisplayServiceStatistics::eTests t);
                              void IncrementFailStatistics(GDisplayServiceStatistics::eTests t);

};

#endif

