#include <cstdio>
#include <cstring>

#include "GSecureAudioManager.h"

#include "Common/Util/DEBUGOUT.h"

#include "GSecureAudioMenu.h"
#include "Menus/Templates/GPopUpWindow.h"
#include "Menus/Keypad/GKeypadMenu.h"
#include "Services/Resource/GResourcesService.h"
#include <GSystem.h>

const char * GSecureAudioManager::mst_arypcStatusLabel[]=
{
		"eStatusInitial",
		"eStatusCheckNonSecureTest",
		"eStatusSecureTest",
		"eStatusSecureAudioScr",
		"eStatusError",
		"eStatusTestResult",
		"eStatusSummary"
};
const char * GSecureAudioManager::mst_arypcEventLabel[]=
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

const char* GSecureAudioManager::m_aryu8SAResultStrings[] =
{
  "Secure Audio: No Test",
  "No Audio in NonSecure Mode",
  "Secure Audio: Disabled",
  "Secure Audio: Enabled"
};

GSecureAudioManager::GSecureAudioManager(GSecureAudioMenu *pMenu) :GAbsKeyboardManager(NULL),
				m_pDYMenu(pMenu),
				m_pDYService(NULL),
				m_pTimer(NULL),
				m_SecStatus(eStatusInitial),
				m_bIsNonSecureTest(false),
				m_TOutCommand(this, &GSecureAudioManager::TemporizedDisplayTest)
{
	m_pTimer = new GTimer(&m_TOutCommand, DY_TOUT_SECURE_AUDIO_TEST_DELAY, true, false);
	UINT8 u8ResId = GResourcesService::GetService()->GetResourceIdForSecureAudio();
	m_pSecureAudioWindow = new GWindowFromResource(u8ResId);
}

GSecureAudioManager::~GSecureAudioManager()
{
	delete m_pSecureAudioWindow;
}

void GSecureAudioManager::Initialize()
{
	m_pDYService = GKeypadService::GetService();

	if (m_pDYService)
	{
		m_pDYService->AttachManager(this);
	}
}

void GSecureAudioManager::Dispose()
{
	m_pDYService = GKeypadService::GetService();
	if (m_pDYService)
	{
		m_pDYService->DetachManager();
	}
}

void GSecureAudioManager::SetSAResults(enum eSAResult result)
{
	m_pDYService = GKeypadService::GetService();
	m_pDYService->SetSAResults(result);
}

void GSecureAudioManager::ProcessSAResults()
{
	switch(m_pDYService->GetNonSecStatusResults())
	{
		case eSATStatusInitial:
		{
			m_pDYMenu->DrawTestResultMenu(m_aryu8SAResultStrings[eSAInitial],"");
			SetSAResults(eSAInitial);
			break;
		}
//========================================================================================================
		case eSATStatusYes:
		{
			switch(m_pDYService->GetSecStatusResults())
			{
				case eSATStatusInitial:
				{
					SetSAResults(eSAInitial);
					m_pDYMenu->DrawTestResultMenu(m_aryu8SAResultStrings[eSAInitial], "");
					break;
				}

				case eSATStatusYes:
				{
					SetSAResults(eSADisabled);
					m_pDYMenu->DrawTestResultMenu("Feature tested successfully", m_aryu8SAResultStrings[eSADisabled]);
					break;
				}

				case eSATStatusNo:
				{
					SetSAResults(eSAEnabled);
					m_pDYMenu->DrawTestResultMenu("Feature tested successfully", m_aryu8SAResultStrings[eSAEnabled]);
					break;
				}

			}

			break;
		}
//=========================================================================================================
		case eSATStatusNo:
		{
			SetSAResults(eSANoTest);
			m_pDYMenu->DrawTestResultMenu("Could not test feature", m_aryu8SAResultStrings[eSANoTest]);
			break;
		}
	}
}


void GSecureAudioManager::ProcessEventReceived(eEvents e)
{
	GDisplayService* pDYService = GDisplayService::GetService();
	switch(m_SecStatus)
	{
		//======================================================================================================
		case eStatusInitial:
		{
			switch(e)
			{
				case eEventSoftKeyL1:
					NextState(eEventSoftKeyL1, eStatusCheckNonSecureTest);
					m_pDYService->SetNonSecStatusResults(eSATStatusInitial);
					m_pDYService->SetSecStatusResults(eSATStatusInitial);
					m_pDYMenu->DrawNonSecureCheckTestMenu("Do you hear the audio from the speakers?");
					//StartTimer();
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
					LOG(eDisplay, eTrace, "IGNORED EVENT [%s] received in [%s].\n", mst_arypcEventLabel[e], mst_arypcStatusLabel[m_SecStatus]);
				break;

				// Go to the Summary if the Soft R4 is pressed.
				case eEventSoftKeyR4:
					NextState(eEventSoftKeyR4, eStatusSummary);
					StopTimer();
					ProcessSAResults();
					StartTimer(DY_TOUT_SECURE_AUDIO_GET_ANSWER_DELAY);
				break;

				default:
					LOG(eDisplay, eError, "ERROR: INVALID EVENT [%s] received in [%s].\n", mst_arypcEventLabel[e], mst_arypcStatusLabel[m_SecStatus]);
				break;
			}
		}
		break;
		//======================================================================================================

		//======================================================================================================

		case eStatusCheckNonSecureTest:
		{
			switch(e)
			{
				case eEventSoftKeyL1 :
					NextState(eEventSoftKeyL1, eStatusSecureTest);
					m_pDYService->SetNonSecStatusResults(eSATStatusYes);
					m_pDYMenu->DrawSecureTestMenu(std::string("Press Start to go to Secure input session"));
					break;
				case eEventSoftKeyL2 :
					m_pDYMenu->EnableSoftKeys();
					break;

				case eEventSoftKeyL3 :
					NextState(eEventSoftKeyL2, eStatusSummary);
					m_pDYService->SetNonSecStatusResults(eSATStatusNo);

					ProcessSAResults();
					break;

				case eEventSoftKeyL4:
					NextState(eEventSoftKeyL4, eStatusInitial);
					StopTimer();
					m_pDYMenu->DrawMainMenu();
				break;


				default:
					m_pDYMenu->EnableSoftKeys();
					break;

			}
			break;
		}

		//======================================================================================================

		case eStatusSecureTest:
		{
			switch(e)
			{
				case eEventSoftKeyL1:

					NextState(eEventSoftKeyL1, eStatusSecureAudioScr);

					if(m_blWindowCreated)
						m_pSecureAudioWindow->DestroyWindow();

					m_pSecureAudioWindow->CreateWindow(); // re-create a clean matrix for test
					m_blWindowCreated = true;
					// hiding previous window before beginning the test

					//if(pSysInfo->isVGDUnit())
					{
						pDYService->HideWindowTag(eWinId_MainMenu,0);
					}
					m_pDYService->EnableRequestForSecAud();

					//pKMenu->Initialize();
					//pKMenu->DrawMenu();
					//m_pDYMenu->PushMenu(pKMenu);

					//StartTimer();
				break;

				case eEventSoftKeyL4:
					NextState(eEventSoftKeyL4, eStatusCheckNonSecureTest);
					StopTimer();
					m_pDYMenu->DrawNonSecureCheckTestMenu("Do you hear the audio from the speakers?");
				break;

				case eEventSoftKeyL2:
				case eEventSoftKeyL3:
				case eEventSoftKeyR1:
				case eEventSoftKeyR2:
				case eEventSoftKeyR3:
					LOG(eDisplay, eTrace, "IGNORED EVENT [%s] received in [%s].\n", mst_arypcEventLabel[e], mst_arypcStatusLabel[m_SecStatus]);
				break;

				default:
					LOG(eDisplay, eError, "ERROR: INVALID EVENT [%s] received in [%s].\n", mst_arypcEventLabel[e], mst_arypcStatusLabel[m_SecStatus]);
				break;
			}
			break;
		}
	//======================================================================================================

		case eStatusSecureAudioScr:
			{
				bool blAprove;
				switch(e)
				{
					case eEventSoftKeyL1 :
						NextState(eEventSoftKeyL1, eStatusSummary);
						m_pDYService->SetSecStatusResults(eSATStatusYes);
						ProcessSAResults();
						pDYService->ShowWindowTag(eWinId_MainMenu,0);
						blAprove = m_pDYService->DisableRequest();
						if(blAprove)
						{
							// marked matrix is not useful for another test or statistic
							//m_pSecureAudioWindow->DestroyWindow();
							m_pSecureAudioWindow->DestroyWindow();
							m_blWindowCreated = false;
						}

						break;

/*					case eEventSoftKeyL2 :
						NextState(eEventSoftKeyL2, eStatusSecureTest);
						GSecureAudioManager::m_bSecureTestPassed=false;
						m_pDYMenu->DrawSecureTestMenu(std::string("1) Press Start go to Secure input session"));
						break;*/

					case eEventSoftKeyL3 :
						NextState(eEventSoftKeyL2, eStatusSummary);
						blAprove = m_pDYService->DisableRequest();
						m_pDYService->SetSecStatusResults(eSATStatusNo);
						ProcessSAResults();
						if(blAprove)
						{
							// marked matrix is not useful for another test or statistic
							//m_pSecureAudioWindow->DestroyWindow();
							m_pSecureAudioWindow->DestroyWindow();
							m_blWindowCreated = false;
							pDYService->ShowWindowTag(eWinId_MainMenu,0);
						}
						break;

					case eEventSoftKeyL4:
						{
							NextState(eEventSoftKeyL4,eStatusSecureTest );
							blAprove = m_pDYService->DisableRequest();
							if(blAprove)
							{
								// marked matrix is not useful for another test or statistic
								//m_pSecureAudioWindow->DestroyWindow();
								m_pSecureAudioWindow->DestroyWindow();
								m_blWindowCreated = false;
								pDYService->ShowWindowTag(eWinId_MainMenu,0);
							}
							else // exit key combination has not met yet
							{
								m_pDYMenu->EnableSoftKeys();
								return; // return immediately, status is not changed
							}
							break;
						}
					default:
							m_pDYMenu->EnableSoftKeys();
							break;
				}
				break;
			}
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
					LOG(eDisplay, eTrace, "IGNORED EVENT [%s] received in [%s].\n", mst_arypcEventLabel[e], mst_arypcStatusLabel[m_SecStatus]);
				break;

				default:
					LOG(eDisplay, eError, "ERROR: INVALID EVENT [%s] received in [%s].\n", mst_arypcEventLabel[e], mst_arypcStatusLabel[m_SecStatus]);
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

void GSecureAudioManager::NextState(eEvents e, eSecStatus s)
{
	LOG( eDisplay, eDebug, "[current_status[%s] ==>> event[%s] ==>> next_status[%s]]\n", mst_arypcStatusLabel[m_SecStatus], mst_arypcEventLabel[e], mst_arypcStatusLabel[s] );
	m_SecStatus = s;
}

bool GSecureAudioManager::StopTimer()
{
	if( m_pTimer == NULL )
	{
		return false;
	}

	return m_pTimer->Stop();
}

bool GSecureAudioManager::StartTimer(UINT16 timeout)
{
	if( m_pTimer == NULL )
	{
		return false;
	}

	m_pTimer->SetInterval(timeout);

	return m_pTimer->Start();
}

void GSecureAudioManager::TemporizedDisplayTest()
{
	ProcessEventReceived(eEventTimeout);
}

void GSecureAudioManager::NotifyCustomerInputKey(UINT8 u8KeyCode)
{
	if (m_SecStatus == eStatusSecureAudioScr) // only if we are in test window, the key pressed event is used.
		{
			if(m_pKPService)
				m_pKPService->NotifySoftKey(u8KeyCode);

			if( m_blWindowCreated && m_pTestWindow )
			{
				m_pTestWindow->MarkKeyPressed(u8KeyCode);
			}
		}
}

bool GSecureAudioManager::ActivateTest_KeypadAction()
{
	return true;
}

bool GSecureAudioManager::DeactivateTest_KeypadAction()
{
	return true;
}

bool GSecureAudioManager::InTest_SoftKeyAction()
{
	return true;
}

bool GSecureAudioManager::DeactivateTest_SoftKeyAction()
{
	return true;
}

bool GSecureAudioManager::isSuitableKeyCode(UINT8)
{
	return true;
}

bool GSecureAudioManager::KeepSoftKeysEnabled()
{
	return true;
}

