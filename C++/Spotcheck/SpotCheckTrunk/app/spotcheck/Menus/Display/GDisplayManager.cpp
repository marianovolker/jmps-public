#include <cstdio>
#include <cstring>

#include "GDisplayManager.h"

#include "Common/Util/DEBUGOUT.h"
#include "Services/Display/GDisplayService.h"
#include "Menus/Display/GDisplayMenu.h"
#include "Menus/Templates/GPopUpWindow.h"

#include <GSystem.h>



const char * GDisplayManager::mst_arypcStatusLabel[]=
{
	"eStatusInitial",
	"eStatusTest_1",
	"eStatusCheckTest_1",
	"eStatusTest_2",
	"eStatusCheckTest_2",
	"eStatusTest_3",
	"eStatusCheckTest_3",
	"eStatusTest_4",
	"eStatusCheckTest_4",
	"eStatusTest_5",
	"eStatusCheckTest_5",
	"eStatusError",
	"eStatusTestResult",
	"eStatusSummary"
};
const char * GDisplayManager::mst_arypcEventLabel[]=
{
	"eEventSoftKeyL1",
	"eEventSoftKeyL2",
	"eEventSoftKeyL3",
	"eEventSoftKeyL4",

	"eEventSoftKeyR1",
	"eEventSoftKeyR2",
	"eEventSoftKeyR3",
	"eEventSoftKeyR4",

	"eEventTimeout",
	"eEventError"
};



GDisplayManager::GDisplayManager(GDisplayMenu *pMenu) :
				m_pDYMenu(pMenu),
				m_pDYService(NULL),
				m_pTimer(NULL),
				m_Status(eStatusInitial),
				m_TOutCommand(this, &GDisplayManager::TemporizedDisplayTest)
{
	m_pTimer = new GTimer(&m_TOutCommand, DY_TOUT_DISPLAY_TEST_DELAY, true, false);
}

GDisplayManager::~GDisplayManager()
{
}

void GDisplayManager::Initialize()
{
	m_pDYService = GDisplayService::GetService();

	if (m_pDYService)
	{
		m_pDYService->AttachManager(this);
	}
}

void GDisplayManager::Dispose()
{
	m_pDYService = GDisplayService::GetService();
	if (m_pDYService)
	{
		m_pDYService->DetachManager();
	}
}

void GDisplayManager::ProcessEventReceived(eEvents e)
{
	switch(m_Status)
	{
		//======================================================================================================
		case eStatusInitial:
		{
			switch(e)
			{
				case eEventSoftKeyL1:
					NextState(eEventSoftKeyL1, eStatusTest_1);
					FirstTest();
					StartTimer();
				break;

				case eEventSoftKeyL4:
					NextState(eEventSoftKeyL4, eStatusInitial);
					StopTimer();
					m_pDYMenu->Quit();
				break;

				case eEventSoftKeyL2:
				case eEventSoftKeyL3:
				case eEventSoftKeyR1:
				case eEventSoftKeyR2:
				case eEventSoftKeyR3:
					LOG(eDisplay, eTrace, "IGNORED EVENT [%s] received in [%s].\n", mst_arypcEventLabel[e], mst_arypcStatusLabel[m_Status]);
				break;

				// Go to the Summary if the Soft R4 is pressed.
				case eEventSoftKeyR4:
					NextState(eEventSoftKeyR4, eStatusSummary);
					StopTimer();
					m_pDYMenu->DrawTestResultMenu();
					StartTimer(DY_TOUT_DISPLAY_GET_ANSWER_DELAY);
				break;

				default:
					LOG(eDisplay, eError, "ERROR: INVALID EVENT [%s] received in [%s].\n", mst_arypcEventLabel[e], mst_arypcStatusLabel[m_Status]);
				break;
			}
		}
		break;
		//======================================================================================================

		//======================================================================================================
		case eStatusTest_1:
		{
			switch(e)
			{
				case eEventError:
					NextState(eEventError, eStatusTest_1);
					m_pDYMenu->DrawErrorMenu("Error with Display, Check log file.");
				break;

				case eEventTimeout:
				{
					NextState(eEventTimeout, eStatusCheckTest_1);

					std::string str = "Did you see all the pixels ";
					str+= GDisplayServiceStatistics::GetTestLabel(GDisplayServiceStatistics::eTest1_White_Color);
					str+="?.";

					DrawCheckTestMenu(str);
					StopTimer();
					StartTimer(DY_TOUT_DISPLAY_GET_ANSWER_DELAY);
				}
				break;

				case eEventSoftKeyL1:
				case eEventSoftKeyL2:
				case eEventSoftKeyL3:
				case eEventSoftKeyR1:
				case eEventSoftKeyR2:
				case eEventSoftKeyR3:
					LOG(eDisplay, eTrace, "IGNORED EVENT [%s] received in [%s].\n", mst_arypcEventLabel[e], mst_arypcStatusLabel[m_Status]);
				break;

				case eEventSoftKeyL4:
					NextState(eEventSoftKeyL4, eStatusInitial);
					StopTimer();
					DestroyAndHidePlainScreen();
					m_pDYMenu->Quit();
				break;

				// Go to the Summary if the Soft R4 is pressed.
				case eEventSoftKeyR4:
					NextState(eEventSoftKeyR4, eStatusSummary);
					StopTimer();
					m_pDYMenu->DrawTestResultMenu();
					DestroyAndHidePlainScreen();
					StartTimer(DY_TOUT_DISPLAY_GET_ANSWER_DELAY);
				break;

				default:
					LOG(eDisplay, eError, "ERROR: INVALID EVENT [%s] received in [%s].\n", mst_arypcEventLabel[e], mst_arypcStatusLabel[m_Status]);
				break;
			}
		}
		break;
		//======================================================================================================

		//======================================================================================================
		case eStatusCheckTest_1:
		{
			switch(e)
			{
				case eEventTimeout:
					NextState(eEventTimeout, eStatusInitial);
					m_pDYMenu->DrawTimeoutMenu("Timeout!. You must select an option.");
					GSystem::Sleep(2000);
					m_pDYMenu->DrawMainMenu();
					StopTimer();
				break;

				// YES Soft Key is pressed.
				case eEventSoftKeyL1:
					NextState(eEventSoftKeyL1, eStatusTest_2);
					StopTimer();
					IncrementPassStatistics(GDisplayServiceStatistics::eTest1_White_Color);
					SecondTest();
					StartTimer();
				break;

				// AGAIN Soft Key is pressed.
				case eEventSoftKeyL2:
					NextState(eEventSoftKeyL2, eStatusTest_1);
					StopTimer();
					FirstTest();
					StartTimer();
				break;

				// NO Soft Key is pressed.
				case eEventSoftKeyL3:
					NextState(eEventSoftKeyL3, eStatusTest_2);
					StopTimer();
					IncrementFailStatistics(GDisplayServiceStatistics::eTest1_White_Color);
					SecondTest();
					StartTimer();
				break;

				case eEventSoftKeyL4:
					NextState(eEventSoftKeyL4, eStatusInitial);
					StopTimer();
					m_pDYMenu->Quit();
				break;

				case eEventSoftKeyR1:
				case eEventSoftKeyR2:
				case eEventSoftKeyR3:
					LOG(eDisplay, eTrace, "IGNORED EVENT [%s] received in [%s].\n", mst_arypcEventLabel[e], mst_arypcStatusLabel[m_Status]);
				break;

				// Go to the Summary if the Soft R4 is pressed.
				case eEventSoftKeyR4:
					NextState(eEventSoftKeyR4, eStatusSummary);
					StopTimer();
					m_pDYMenu->DrawTestResultMenu();
					StartTimer(DY_TOUT_DISPLAY_GET_ANSWER_DELAY);
				break;

				default:
					LOG(eDisplay, eError, "ERROR: INVALID EVENT [%s] received in [%s].\n", mst_arypcEventLabel[e], mst_arypcStatusLabel[m_Status]);
				break;
			}
		}
		break;
		//======================================================================================================

		//======================================================================================================
		case eStatusTest_2:
		{
			switch(e)
			{
				case eEventError:
					NextState(eEventError, eStatusTest_2);
					m_pDYMenu->DrawErrorMenu("Error with Display, Check log file.");
				break;

				case eEventTimeout:
				{
					NextState(eEventTimeout, eStatusCheckTest_2);

					std::string str = "Did you see all the pixels ";
					str+= GDisplayServiceStatistics::GetTestLabel(GDisplayServiceStatistics::eTest2_Black_Color);
					str+="?.";

					DrawCheckTestMenu(str);
					StopTimer();
					StartTimer(DY_TOUT_DISPLAY_GET_ANSWER_DELAY);
				}
				break;

				case eEventSoftKeyL1:
				case eEventSoftKeyL2:
				case eEventSoftKeyL3:
				case eEventSoftKeyR1:
				case eEventSoftKeyR2:
				case eEventSoftKeyR3:
					LOG(eDisplay, eTrace, "IGNORED EVENT [%s] received in [%s].\n", mst_arypcEventLabel[e], mst_arypcStatusLabel[m_Status]);
				break;

				case eEventSoftKeyL4:
					NextState(eEventSoftKeyL4, eStatusInitial);
					StopTimer();
					DestroyAndHidePlainScreen();
					m_pDYMenu->Quit();
				break;

				// Go to the Summary if the Soft R4 is pressed.
				case eEventSoftKeyR4:
					NextState(eEventSoftKeyR4, eStatusSummary);
					StopTimer();
					m_pDYMenu->DrawTestResultMenu();
					DestroyAndHidePlainScreen();
					StartTimer(DY_TOUT_DISPLAY_GET_ANSWER_DELAY);
				break;

				default:
					LOG(eDisplay, eError, "ERROR: INVALID EVENT [%s] received in [%s].\n", mst_arypcEventLabel[e], mst_arypcStatusLabel[m_Status]);
				break;
			}
		}
		break;
		//======================================================================================================

		//======================================================================================================
		case eStatusCheckTest_2:
		{
			switch(e)
			{
				case eEventTimeout:
					NextState(eEventTimeout, eStatusInitial);
					m_pDYMenu->DrawTimeoutMenu("Timeout!. You must select an option.");
					GSystem::Sleep(2000);
					m_pDYMenu->DrawMainMenu();
					StopTimer();
				break;

				// YES Soft Key is pressed.
				case eEventSoftKeyL1:
					NextState(eEventSoftKeyL1, eStatusTest_3);
					StopTimer();
					IncrementPassStatistics(GDisplayServiceStatistics::eTest2_Black_Color);
					ThirdTest();
					StartTimer();
				break;

				// AGAIN Soft Key is pressed.
				case eEventSoftKeyL2:
					NextState(eEventSoftKeyL2, eStatusTest_2);
					StopTimer();
					SecondTest();
					StartTimer();
				break;

				// NO Soft Key is pressed.
				case eEventSoftKeyL3:
					NextState(eEventSoftKeyL3, eStatusTest_3);
					StopTimer();
					IncrementFailStatistics(GDisplayServiceStatistics::eTest2_Black_Color);
					ThirdTest();
					StartTimer();
				break;

				case eEventSoftKeyL4:
					NextState(eEventSoftKeyL4, eStatusInitial);
					StopTimer();
					m_pDYMenu->Quit();
				break;

				case eEventSoftKeyR1:
				case eEventSoftKeyR2:
				case eEventSoftKeyR3:
					LOG(eDisplay, eTrace, "IGNORED EVENT [%s] received in [%s].\n", mst_arypcEventLabel[e], mst_arypcStatusLabel[m_Status]);
				break;

				// Go to the Summary if the Soft R4 is pressed.
				case eEventSoftKeyR4:
					NextState(eEventSoftKeyR4, eStatusSummary);
					StopTimer();
					m_pDYMenu->DrawTestResultMenu();
					StartTimer(DY_TOUT_DISPLAY_GET_ANSWER_DELAY);
				break;

				default:
					LOG(eDisplay, eError, "ERROR: INVALID EVENT [%s] received in [%s].\n", mst_arypcEventLabel[e], mst_arypcStatusLabel[m_Status]);
				break;
			}
		}
		break;
		//======================================================================================================

		//======================================================================================================
		case eStatusTest_3:
		{
			switch(e)
			{
				case eEventError:
					NextState(eEventError, eStatusTest_3);
					m_pDYMenu->DrawErrorMenu("Error with Display, Check log file.");
				break;

				case eEventTimeout:
				{
					NextState(eEventTimeout, eStatusCheckTest_3);

					std::string str = "Did you see all the pixels ";
					str+= GDisplayServiceStatistics::GetTestLabel(GDisplayServiceStatistics::eTest3_Red_Color);
					str+="?.";

					DrawCheckTestMenu(str);
					StopTimer();
					StartTimer(DY_TOUT_DISPLAY_GET_ANSWER_DELAY);
				}
				break;

				case eEventSoftKeyL1:
				case eEventSoftKeyL2:
				case eEventSoftKeyL3:
				case eEventSoftKeyR1:
				case eEventSoftKeyR2:
				case eEventSoftKeyR3:
					LOG(eDisplay, eTrace, "IGNORED EVENT [%s] received in [%s].\n", mst_arypcEventLabel[e], mst_arypcStatusLabel[m_Status]);
				break;

				case eEventSoftKeyL4:
					NextState(eEventSoftKeyL4, eStatusInitial);
					StopTimer();
					DestroyAndHidePlainScreen();
					m_pDYMenu->Quit();
				break;

				// Go to the Summary if the Soft R4 is pressed.
				case eEventSoftKeyR4:
					NextState(eEventSoftKeyR4, eStatusSummary);
					StopTimer();
					m_pDYMenu->DrawTestResultMenu();
					DestroyAndHidePlainScreen();
					StartTimer(DY_TOUT_DISPLAY_GET_ANSWER_DELAY);
				break;

				default:
					LOG(eDisplay, eError, "ERROR: INVALID EVENT [%s] received in [%s].\n", mst_arypcEventLabel[e], mst_arypcStatusLabel[m_Status]);
				break;
			}
		}
		break;
		//======================================================================================================

		//======================================================================================================
		case eStatusCheckTest_3:
		{
			switch(e)
			{
				case eEventTimeout:
					NextState(eEventTimeout, eStatusInitial);
					m_pDYMenu->DrawTimeoutMenu("Timeout!. You must select an option.");
					GSystem::Sleep(2000);
					m_pDYMenu->DrawMainMenu();
					StopTimer();
				break;

				// YES Soft Key is pressed.
				case eEventSoftKeyL1:
					NextState(eEventSoftKeyL1, eStatusTest_4);
					StopTimer();
					IncrementPassStatistics(GDisplayServiceStatistics::eTest3_Red_Color);
					FourthTest();
					StartTimer();
				break;

				// AGAIN Soft Key is pressed.
				case eEventSoftKeyL2:
					NextState(eEventSoftKeyL2, eStatusTest_3);
					StopTimer();
					ThirdTest();
					StartTimer();
				break;

				// NO Soft Key is pressed.
				case eEventSoftKeyL3:
					NextState(eEventSoftKeyL3, eStatusTest_4);
					StopTimer();
					IncrementFailStatistics(GDisplayServiceStatistics::eTest3_Red_Color);
					FourthTest();
					StartTimer();
				break;

				case eEventSoftKeyL4:
					NextState(eEventSoftKeyL4, eStatusInitial);
					StopTimer();
					m_pDYMenu->Quit();
				break;

				case eEventSoftKeyR1:
				case eEventSoftKeyR2:
				case eEventSoftKeyR3:
					LOG(eDisplay, eTrace, "IGNORED EVENT [%s] received in [%s].\n", mst_arypcEventLabel[e], mst_arypcStatusLabel[m_Status]);
				break;

				// Go to the Summary if the Soft R4 is pressed.
				case eEventSoftKeyR4:
					NextState(eEventSoftKeyR4, eStatusSummary);
					StopTimer();
					m_pDYMenu->DrawTestResultMenu();
					StartTimer(DY_TOUT_DISPLAY_GET_ANSWER_DELAY);
				break;

				default:
					LOG(eDisplay, eError, "ERROR: INVALID EVENT [%s] received in [%s].\n", mst_arypcEventLabel[e], mst_arypcStatusLabel[m_Status]);
				break;
			}
		}
		break;
		//======================================================================================================

		//======================================================================================================
		case eStatusTest_4:
		{
			switch(e)
			{
				case eEventError:
					NextState(eEventError, eStatusTest_4);
					m_pDYMenu->DrawErrorMenu("Error with Display, Check log file.");
				break;

				case eEventTimeout:
				{
					NextState(eEventTimeout, eStatusCheckTest_4);

					std::string str = "Did you see all the pixels ";
					str+= GDisplayServiceStatistics::GetTestLabel(GDisplayServiceStatistics::eTest4_Green_Color);
					str+="?.";

					DrawCheckTestMenu(str);
					StopTimer();
					StartTimer(DY_TOUT_DISPLAY_GET_ANSWER_DELAY);
				}
				break;

				case eEventSoftKeyL1:
				case eEventSoftKeyL2:
				case eEventSoftKeyL3:
				case eEventSoftKeyR1:
				case eEventSoftKeyR2:
				case eEventSoftKeyR3:
					LOG(eDisplay, eTrace, "IGNORED EVENT [%s] received in [%s].\n", mst_arypcEventLabel[e], mst_arypcStatusLabel[m_Status]);
				break;

				case eEventSoftKeyL4:
					NextState(eEventSoftKeyL4, eStatusInitial);
					StopTimer();
					DestroyAndHidePlainScreen();
					m_pDYMenu->Quit();
				break;

				// Go to the Summary if the Soft R4 is pressed.
				case eEventSoftKeyR4:
					NextState(eEventSoftKeyR4, eStatusSummary);
					StopTimer();
					m_pDYMenu->DrawTestResultMenu();
					StartTimer(DY_TOUT_DISPLAY_GET_ANSWER_DELAY);
					DestroyAndHidePlainScreen();
				break;

				default:
					LOG(eDisplay, eError, "ERROR: INVALID EVENT [%s] received in [%s].\n", mst_arypcEventLabel[e], mst_arypcStatusLabel[m_Status]);
				break;
			}
		}
		break;
		//======================================================================================================

		//======================================================================================================
		case eStatusCheckTest_4:
		{
			switch(e)
			{
				case eEventTimeout:
					NextState(eEventTimeout, eStatusInitial);
					m_pDYMenu->DrawTimeoutMenu("Timeout!. You must select an option.");
					GSystem::Sleep(2000);
					m_pDYMenu->DrawMainMenu();
					StopTimer();
				break;

				// YES Soft Key is pressed.
				case eEventSoftKeyL1:
					NextState(eEventSoftKeyL1, eStatusTest_5);
					StopTimer();
					IncrementPassStatistics(GDisplayServiceStatistics::eTest4_Green_Color);
					FifthTest();
					StartTimer();
				break;

				// AGAIN Soft Key is pressed.
				case eEventSoftKeyL2:
					NextState(eEventSoftKeyL2, eStatusTest_4);
					StopTimer();
					FourthTest();
					StartTimer();
				break;

				// NO Soft Key is pressed.
				case eEventSoftKeyL3:
					NextState(eEventSoftKeyL3, eStatusTest_5);
					StopTimer();
					IncrementFailStatistics(GDisplayServiceStatistics::eTest4_Green_Color);
					FifthTest();
					StartTimer();
				break;

				case eEventSoftKeyL4:
					NextState(eEventSoftKeyL4, eStatusInitial);
					StopTimer();
					m_pDYMenu->Quit();
				break;

				case eEventSoftKeyR1:
				case eEventSoftKeyR2:
				case eEventSoftKeyR3:
					LOG(eDisplay, eTrace, "IGNORED EVENT [%s] received in [%s].\n", mst_arypcEventLabel[e], mst_arypcStatusLabel[m_Status]);
				break;

				// Go to the Summary if the Soft R4 is pressed.
				case eEventSoftKeyR4:
					NextState(eEventSoftKeyR4, eStatusSummary);
					StopTimer();
					m_pDYMenu->DrawTestResultMenu();
					StartTimer(DY_TOUT_DISPLAY_GET_ANSWER_DELAY);
				break;

				default:
					LOG(eDisplay, eError, "ERROR: INVALID EVENT [%s] received in [%s].\n", mst_arypcEventLabel[e], mst_arypcStatusLabel[m_Status]);
				break;
			}
		}
		break;
		//======================================================================================================

		//======================================================================================================
		case eStatusTest_5:
		{
			switch(e)
			{
				case eEventError:
					NextState(eEventError, eStatusTest_5);
					m_pDYMenu->DrawErrorMenu("Error with Display, Check log file.");
				break;

				case eEventTimeout:
				{
					NextState(eEventTimeout, eStatusCheckTest_5);

					std::string str = "Did you see all the pixels ";
					str+= GDisplayServiceStatistics::GetTestLabel(GDisplayServiceStatistics::eTest5_Blue_Color);
					str+="?.";

					DrawCheckTestMenu(str);
					StopTimer();
					StartTimer(DY_TOUT_DISPLAY_GET_ANSWER_DELAY);
				}
				break;

				case eEventSoftKeyL1:
				case eEventSoftKeyL2:
				case eEventSoftKeyL3:
				case eEventSoftKeyR1:
				case eEventSoftKeyR2:
				case eEventSoftKeyR3:
					LOG(eDisplay, eTrace, "IGNORED EVENT [%s] received in [%s].\n", mst_arypcEventLabel[e], mst_arypcStatusLabel[m_Status]);
				break;

				case eEventSoftKeyL4:
					NextState(eEventSoftKeyL4, eStatusInitial);
					StopTimer();
					DestroyAndHidePlainScreen();
					m_pDYMenu->Quit();
				break;

				// Go to the Summary if the Soft R4 is pressed.
				case eEventSoftKeyR4:
					NextState(eEventSoftKeyR4, eStatusSummary);
					StopTimer();
					m_pDYMenu->DrawTestResultMenu();
					DestroyAndHidePlainScreen();
					StartTimer(DY_TOUT_DISPLAY_GET_ANSWER_DELAY);
				break;

				default:
					LOG(eDisplay, eError, "ERROR: INVALID EVENT [%s] received in [%s].\n", mst_arypcEventLabel[e], mst_arypcStatusLabel[m_Status]);
				break;
			}
		}
		break;
		//======================================================================================================

		//======================================================================================================
		case eStatusCheckTest_5:
		{
			switch(e)
			{
				case eEventTimeout:
					NextState(eEventTimeout, eStatusInitial);
					m_pDYMenu->DrawTimeoutMenu("Timeout!. You must select an option.");
					GSystem::Sleep(2000);
					m_pDYMenu->DrawMainMenu();
					StopTimer();
				break;

				// YES Soft Key is pressed.
				case eEventSoftKeyL1:
					NextState(eEventSoftKeyL1, eStatusSummary);
					StopTimer();
					IncrementPassStatistics(GDisplayServiceStatistics::eTest5_Blue_Color);
					m_pDYMenu->DrawTestResultMenu();
					StartTimer(DY_TOUT_DISPLAY_GET_ANSWER_DELAY);
				break;

				// AGAIN Soft Key is pressed.
				case eEventSoftKeyL2:
					NextState(eEventSoftKeyL2, eStatusTest_5);
					StopTimer();
					FifthTest();
					StartTimer();
				break;

				// NO Soft Key is pressed.
				case eEventSoftKeyL3:
					NextState(eEventSoftKeyL3, eStatusSummary);
					StopTimer();
					IncrementFailStatistics(GDisplayServiceStatistics::eTest5_Blue_Color);
					m_pDYMenu->DrawTestResultMenu();
					StartTimer(DY_TOUT_DISPLAY_GET_ANSWER_DELAY);
				break;

				case eEventSoftKeyL4:
					NextState(eEventSoftKeyL4, eStatusInitial);
					StopTimer();
					m_pDYMenu->Quit();
				break;

				case eEventSoftKeyR1:
				case eEventSoftKeyR2:
				case eEventSoftKeyR3:
					LOG(eDisplay, eTrace, "IGNORED EVENT [%s] received in [%s].\n", mst_arypcEventLabel[e], mst_arypcStatusLabel[m_Status]);
				break;

				// Go to the Summary if the Soft R4 is pressed.
				case eEventSoftKeyR4:
					NextState(eEventSoftKeyR4, eStatusSummary);
					StopTimer();
					m_pDYMenu->DrawTestResultMenu();
					StartTimer(DY_TOUT_DISPLAY_GET_ANSWER_DELAY);
				break;

				default:
					LOG(eDisplay, eError, "ERROR: INVALID EVENT [%s] received in [%s].\n", mst_arypcEventLabel[e], mst_arypcStatusLabel[m_Status]);
				break;
			}
		}
		break;
		//======================================================================================================

		//======================================================================================================
		case eStatusSummary:
		{
			switch(e)
			{
				case eEventError:
					NextState(eEventError, eStatusSummary);
					m_pDYMenu->DrawErrorMenu("Error with Display, Check log file.");
				break;

				case eEventTimeout:
					NextState(eEventTimeout, eStatusInitial);
					m_pDYMenu->ClearScreen();
					m_pDYMenu->DrawMainMenu();
					StopTimer();
				break;

				case eEventSoftKeyL4:
					NextState(eEventSoftKeyL4, eStatusInitial);
					StopTimer();
					m_pDYMenu->ClearScreen();
					m_pDYMenu->DrawMainMenu();
				break;

				case eEventSoftKeyL1:
				case eEventSoftKeyL2:
				case eEventSoftKeyL3:
				case eEventSoftKeyR1:
				case eEventSoftKeyR2:
				case eEventSoftKeyR3:
				case eEventSoftKeyR4:
					LOG(eDisplay, eTrace, "IGNORED EVENT [%s] received in [%s].\n", mst_arypcEventLabel[e], mst_arypcStatusLabel[m_Status]);
				break;

				default:
					LOG(eDisplay, eError, "ERROR: INVALID EVENT [%s] received in [%s].\n", mst_arypcEventLabel[e], mst_arypcStatusLabel[m_Status]);
				break;
			}
		}
		break;
		//======================================================================================================

		//======================================================================================================
		case eStatusError:
		{
			switch(e)
			{
				default:
					LOG(eDisplay, eError, "ERROR: event [%s] received in eStatusError. The Display device is in unrecoverable state.\n", mst_arypcEventLabel[e]);
					NextState(e, eStatusError);
				break;
			}
		}
		break;
		//======================================================================================================
	}
}

void GDisplayManager::NextState(eEvents e, eStatus s)
{
	LOG( eDisplay, eDebug, "[current_status[%s] ==>> event[%s] ==>> next_status[%s]]\n", mst_arypcStatusLabel[m_Status], mst_arypcEventLabel[e], mst_arypcStatusLabel[s] );
	m_Status = s;
}

bool GDisplayManager::StopTimer()
{
	if( m_pTimer == NULL )
	{
		return false;
	}

	return m_pTimer->Stop();
}

bool GDisplayManager::StartTimer(UINT16 timeout)
{
	if( m_pTimer == NULL )
	{
		return false;
	}

	m_pTimer->SetInterval(timeout);

	return m_pTimer->Start();
}

bool GDisplayManager::DrawCheckTestMenu(std::string str)
{
	m_pDYMenu->DrawCheckTestMenu(str);

	return DestroyAndHidePlainScreen();
}

void GDisplayManager::IncrementPassStatistics(GDisplayServiceStatistics::eTests t)
{
	m_pDYService->GetStatistics().IncrementStatistics(GDisplayServiceStatistics::ePass, t);
}

void GDisplayManager::IncrementFailStatistics(GDisplayServiceStatistics::eTests t)
{
	m_pDYService->GetStatistics().IncrementStatistics(GDisplayServiceStatistics::eFail, t);
}

void GDisplayManager::FirstTest()
{
	if( BuildAndShowPlainScreen(255,255,255) )
	{
		LOG(eDisplay, eDebug, "STEP 1 started successfully in display test.\n");
	}
	else
	{
		ProcessEventReceived(eEventError);

		LOG(eDisplay, eError, "Could not start STEP_1 test.\n");
	}

	return;
}

void GDisplayManager::SecondTest()
{
	if( BuildAndShowPlainScreen(0,0,0) )
	{
		LOG(eDisplay, eDebug, "STEP 2 started successfully in display test.\n");
	}
	else
	{
		ProcessEventReceived(eEventError);

		LOG(eDisplay, eError, "Could not start STEP_2 test.\n");
	}

	return;
}

void GDisplayManager::ThirdTest()
{
	if( BuildAndShowPlainScreen(255,0,0) )
	{
		LOG(eDisplay, eDebug, "STEP 3 started successfully in display test.\n");
	}
	else
	{
		ProcessEventReceived(eEventError);

		LOG(eDisplay, eError, "Could not start STEP_3 test.\n");
	}

	return;
}

void GDisplayManager::FourthTest()
{
	if( BuildAndShowPlainScreen(0,255,0) )
	{
		LOG(eDisplay, eDebug, "STEP 4 started successfully in display test.\n");
	}
	else
	{
		ProcessEventReceived(eEventError);

		LOG(eDisplay, eError, "Could not start STEP_4 test.\n");
	}

	return;
}

void GDisplayManager::FifthTest()
{
	if( BuildAndShowPlainScreen(0,0,255) )
	{
		LOG(eDisplay, eDebug, "STEP 5 started successfully in display test.\n");
	}
	else
	{
		ProcessEventReceived(eEventError);

		LOG(eDisplay, eError, "Could not start STEP_5 test.\n");
	}

	return;
}

void GDisplayManager::TemporizedDisplayTest()
{
	ProcessEventReceived(eEventTimeout);
}

bool GDisplayManager::BuildAndShowPlainScreen(UINT8 u8Red, UINT8 u8Green, UINT8 u8Blue)
{
	GPopUpWindow popupWindow;
	popupWindow.SetBackgroundColor(u8Red,u8Green,u8Blue);

	popupWindow.SetWindowWidthAndHeight(m_pDYMenu->GetWidth(), m_pDYMenu->GetHeight());
	GSPOTMessage* createMessage = popupWindow.GetCreateMessage();
	if( !m_pDYService->SendCmdToSPOT(createMessage) )
	{
		LOG(eDisplay, eError, "Could not send create window message to SPOT!.\n");
		delete createMessage;
		return false;
	}
	delete createMessage;

	GSPOTMessage* hideMessage = m_pDYMenu->GetHideMessage();
	if (!m_pDYService->SendCmdToSPOT(hideMessage))
	{
		LOG(eDisplay, eError, "Could not send hide window message to to the primary screen SPOT!.\n");
		delete hideMessage;
		return false;
	}
	delete hideMessage;
	return true;
}

bool GDisplayManager::DestroyAndHidePlainScreen()
{
	GPopUpWindow popupWindow;

	GSPOTMessage* showMessage = m_pDYMenu->GetShowMessage();

	if (!m_pDYService->SendCmdToSPOT(showMessage))
	{
		LOG(eDisplay, eError, "Could not send show window message to to the primary screen SPOT!.\n");
		delete showMessage;
		return false;
	}
	delete showMessage;

	GSPOTMessage* hideMessage = popupWindow.GetHideMessage();

	if( !m_pDYService->SendCmdToSPOT(hideMessage))
	{
		LOG(eDisplay, eError, "Could not send hide window message to SPOT!.\n");
		delete hideMessage;
		return false;
	}
	delete hideMessage;

	GSPOTMessage* destroyMessage = popupWindow.GetDestroyMessage();
	if( !m_pDYService->SendCmdToSPOT(destroyMessage))
	{
		LOG(eDisplay, eError, "Could not send destroy window message to SPOT!.\n");
		delete destroyMessage;
		return false;
	}

	delete destroyMessage;

	return true;
}
