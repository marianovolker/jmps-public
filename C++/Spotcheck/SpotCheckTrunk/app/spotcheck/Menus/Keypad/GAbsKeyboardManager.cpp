#include <cstdio>
#include <cstring>

#include "Common/Util/DEBUGOUT.h"
#include "GAbsKeyboardManager.h"
#include "Menus/Keypad/GAbsKeyboardMenu.h"
#include "Services/Keypad/GKeypadService.h"
#include "SPOT/SystemInformation/GGlobalSystemInformation.h"


GAbsKeyboardManager::GAbsKeyboardManager(GAbsKeyboardMenu *pMenu)
	:  m_u8LastKeyPressed(0xFF), m_blWindowCreated(false), m_Status(eShowInitial),
	   m_pTestWindow(NULL), m_pKBMenu(pMenu), m_pKPService(NULL)
{
}

GAbsKeyboardManager::~GAbsKeyboardManager()
{
}

// called after construction
void GAbsKeyboardManager::Initialize()
{
	m_pKPService = GKeypadService::GetService();
	if(m_pKPService)
	{
		m_pKPService->AttachManager(this);
	}
}

// called before destruction
void GAbsKeyboardManager::Dispose()
{
	GKeypadService* pService = GKeypadService::GetService();
	pService->DetachManager();
}


UINT8 GAbsKeyboardManager::GetKeyStatistics(UINT8 u8Key)
{
	if(m_pKPService == NULL)
		return 0xFF;

	return m_pKPService->GetKeyStatistics(u8Key);
}

// check possible state changes
void GAbsKeyboardManager::ProposeChangeStateTo(eStatus eProposedStatus)
{
	// Possible states
	// Device menu  -> eShowInitial
	// eShowInitial -> Device menu

	// eShowInitial -> eShowTest
	// eShowTest    -> eShowInitial

	// eShowInitial -> eShowStats
	// eShowStats   -> eShowInitial

	GGlobalSystemInformation* pSysInfo = GGlobalSystemInformation::GetInstance();
	switch (eProposedStatus)
	{
		case eShowTest:
			if( GetMenuStatus() == eShowInitial )
			{
				SetStatus(eShowTest);

				if(m_blWindowCreated)
					m_pTestWindow->DestroyWindow();

				m_pTestWindow->CreateWindow(); // re-create a clean matrix for test
				m_blWindowCreated = true;
				// hiding previous window before beginning the test


				if(pSysInfo->isVGDUnit())
				{
					GDisplayService* pDYService = GDisplayService::GetService();
					pDYService->HideWindowTag(eWinId_MainMenu,0);
				}

				ActivateTest_KeypadAction();
			}
			else if( GetMenuStatus() == eShowTest )
				InTest_SoftKeyAction();

			KeepSoftKeysEnabled();
			break;

		case eShowStats:
			if( GetMenuStatus() == eShowInitial )
			{
				SetStatus(eShowStats);

				if(m_blWindowCreated)
				{
					m_pTestWindow->ShowWindow();
				}
				else
				{
					m_pTestWindow->CreateWindow();
					m_blWindowCreated = true;
					MarkForStatistics();
				}
			}

			if(m_pKBMenu)
				m_pKBMenu->EnableSoftKeys();
			break;

		case eShowInitial:
			if( GetMenuStatus() == eShowInitial )
			{
				// already in initial state, quitting menu.
				DeactivateTest_KeypadAction();
				if(m_pKBMenu)
					m_pKBMenu->QueueMenuPop();
			}
			//back to "white" initial menu, in this case "keypad menu"
			else
			{
				if(GetMenuStatus() == eShowTest)
				{
					bool blAprove = DeactivateTest_KeypadAction();

					blAprove = DeactivateTest_SoftKeyAction() && blAprove;
					if(blAprove)
					{
						// marked matrix is not useful for another test or statistic
						m_pTestWindow->DestroyWindow();
						m_blWindowCreated = false;
					}
					else // exit key combination has not met yet
					{
						KeepSoftKeysEnabled();
						return; // return immediately, status is not changed
					}
				}
				else // current state: statistics state
				{
					m_pTestWindow->HideWindow();
				}

				SetStatus(eShowInitial);
				if(m_pKBMenu)
				{
					if(pSysInfo->isVGDUnit())
					{
						GDisplayService* pDYService = GDisplayService::GetService();
						pDYService->ShowWindowTag(eWinId_MainMenu,0);
					}
					m_pKBMenu->DrawMenu();
				}
			}
			break;
	}
}

void GAbsKeyboardManager::MarkForStatistics()
{
	if( !m_blWindowCreated || m_pTestWindow == NULL)
		return; // no graphics available

	UINT  uQty = 0;
	UINT8 u8KeyFrecuency = 0;
	UINT8 aryu8KeyCodes[GKeypadService::eMaxKeys];

	memset(aryu8KeyCodes, 0, GKeypadService::eMaxKeys);

	for(int i = 0; i < GKeypadService::eMaxKeys; i++)
	{
		if( isSuitableKeyCode(UINT8(i)) )
		{
			u8KeyFrecuency = GetKeyStatistics( UINT8(i) );
			if( u8KeyFrecuency == 0 || u8KeyFrecuency == 0xFF )
				continue;

			aryu8KeyCodes[uQty++] = UINT8(i);
		}
	}

	m_pTestWindow->MarkForStatistics(uQty, aryu8KeyCodes, true);
}


/**
 * 
 * VIRTUAL METHODS
 * 
 */

//show soft-key as a key pressed
void GAbsKeyboardManager::NotifyCustomerInputKey(UINT8 u8Keystroke)
{
	if (m_Status == eShowTest) // only if we are in test window, the key pressed event is used.
	{
		m_u8LastKeyPressed = u8Keystroke;
		//show the key on screen

		if( m_blWindowCreated && m_pTestWindow )
		{
			m_pTestWindow->MarkKeyPressed(u8Keystroke);
		}
	}
}

void GAbsKeyboardManager::NotifySoftKey(UINT8 u8KeyCode)
{
	if (m_Status == eShowTest) // only if we are in test window, the key pressed event is used.
	{
		if(m_pKPService)
			m_pKPService->NotifySoftKey(u8KeyCode);

		if( m_blWindowCreated && m_pTestWindow )
		{
			if(GKeypadService::isADASoftKeyCode(u8KeyCode))
			{
				m_pTestWindow->MarkKeyPressed(u8KeyCode - UINT8(GKeypadService::eKeyL4));
			}
			else
			{
				m_pTestWindow->MarkKeyPressed(u8KeyCode);

			}
		}
	}
}


