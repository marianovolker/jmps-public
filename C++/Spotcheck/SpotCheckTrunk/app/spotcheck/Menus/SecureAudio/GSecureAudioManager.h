/*!
 * © 2013 Gilbarco Inc.
 * Confidential and Proprietary
 *
 * @file GSecureAudioManager.h
 * @author Sayyed Mohammad
 * @date Feb 4 2015
 * @copyright © 2015 Gilbarco Inc. Confidential and Propietary
 */

#ifndef GSecureAudioManager_h
#define GSecureAudioManager_h 1


#include "Common/Util/GSimpleCommand.h"
#include "Common/Util/GTimer.h"
#include "Services/Keypad/GKeypadService.h"
#include "Services/Display/GDisplayServiceStatistics.h"
#include "Menus/Templates/GKeyMatrixWindowFromResource.h"
#include "Menus/Keypad/GAbsKeyboardManager.h"

const int DY_TOUT_SECURE_AUDIO_TEST_DELAY 		=  5000; // 5 Seconds
const int DY_TOUT_SECURE_AUDIO_GET_ANSWER_DELAY  =  60000; // 60 Seconds

class GSecureAudioMenu;
//class GDisplayService;
class GKeypadService;

class GSecureAudioManager : public GAbsKeyboardManager
{
	public:


		GSecureAudioManager(GSecureAudioMenu *);
		virtual ~GSecureAudioManager();

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

		enum eSecStatus
		{
			eStatusInitial,
			eStatusCheckNonSecureTest,
			eStatusSecureTest,
			eStatusSecureAudioScr,
			eStatusError,
			eStatusTestResult,
			eStatusSummary,

			eLastStatus = (eStatusSummary+1)
		};

		void ProcessEventReceived(eEvents e);
		void Initialize();
		void Dispose();
		bool IsSecureAudioTest()
		{
			return true;
		}
		void SetSAResults(enum eSAResult result);
		void ProcessSAResults();

	private:

		GSecureAudioMenu* m_pDYMenu;
		GWindowFromResource *  m_pSecureAudioWindow;
		//bool                            m_blWindowCreated;
		GKeypadService* m_pDYService;
		GTimer* m_pTimer;
		GSimpleCommand<GSecureAudioManager> m_TOutCommand;

		enum eSecStatus         m_SecStatus;
		enum eSAResult          m_eSAResult;
		bool   m_bIsNonSecureTest;

		static const char * mst_arypcStatusLabel	[eLastStatus];
		static const char * mst_arypcEventLabel	[eLastEvent	];
		static const char * m_aryu8SAResultStrings[eSALast + 1];

		void NextState(eEvents e, eSecStatus s);

		bool StopTimer();
		bool StartTimer(UINT16 timeout=DY_TOUT_SECURE_AUDIO_TEST_DELAY);

		void TemporizedDisplayTest();

	public:
		virtual void         NotifyCustomerInputKey(UINT8 u8KeyCode);

		virtual bool         ActivateTest_KeypadAction()     ;
		virtual bool         InTest_SoftKeyAction()          ;
		virtual bool         DeactivateTest_KeypadAction();
		virtual bool         DeactivateTest_SoftKeyAction()  ;
		virtual bool         KeepSoftKeysEnabled()           ;
		virtual bool         isSuitableKeyCode(UINT8 )       ;


};

#endif

