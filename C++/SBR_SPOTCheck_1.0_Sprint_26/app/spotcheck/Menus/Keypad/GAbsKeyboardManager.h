/**
 * © 2013 Gilbarco Inc.
 * Confidential and Proprietary
 *
 * @file GAbsKeyboardManager.h
 * @author Guillermo Paris
 * @date July 22 2013
 * @copyright © 2013 Gilbarco Inc. Confidential and Propietary
 */

#ifndef GAbsKeyboardManager_h
#define GAbsKeyboardManager_h 1

#include <GTypes.h>
#include "Menus/Templates/GKeyMatrixWindowFromResource.h"

class GAbsKeyboardMenu;
class GKeypadService;

class GAbsKeyboardManager
{
	public:

		        GAbsKeyboardManager(GAbsKeyboardMenu *);
		virtual ~GAbsKeyboardManager();

		void    Initialize();
		void    Dispose();

		enum eStatus
		{
			eShowInitial,	//show initial window
			eShowTest,		//show test window with keystrokes
			eShowStats		//show stats menu with
		};

		virtual void         NotifyCustomerInputKey(UINT8 KeyCode);
		virtual void         NotifySoftKey(UINT8 KeyCode);

		inline enum eStatus  GetMenuStatus();
		UINT8                GetKeyStatistics(UINT8 key);
		inline UINT8         GetLastKeyPressed();
		inline void          SetLastKeyPressed(UINT8 u8key);
		void                 ProposeChangeStateTo(eStatus eProposedStatus);


	protected:
		              /* PURE VIRTUAL FOR DESCENDANT USE ONLY */
		virtual bool         ActivateTest_KeypadAction()     =    0;
		virtual bool         DeactivateTest_KeypadAction()   =    0;
		virtual bool         InTest_SoftKeyAction()          =    0;
		virtual bool         DeactivateTest_SoftKeyAction()  =    0;
		virtual bool         KeepSoftKeysEnabled()           =    0;
		virtual bool         isSuitableKeyCode(UINT8 )       =    0;
		virtual void         MarkForStatistics();

		inline bool          isWindowCreated() { return m_blWindowCreated; }

		GKeyMatrixWindowFromResource *  m_pTestWindow;
		GAbsKeyboardMenu*               m_pKBMenu;
		GKeypadService*                 m_pKPService;


	private:

		inline  void                    SetStatus(eStatus eWindowStatus);

		UINT8                           m_u8LastKeyPressed;
		bool                            m_blWindowCreated;
		enum eStatus                    m_Status;
};

inline enum GAbsKeyboardManager::eStatus GAbsKeyboardManager::GetMenuStatus()
{
	return m_Status;
}

inline void GAbsKeyboardManager::SetStatus(eStatus eWindowStatus)
{
	m_Status = eWindowStatus;
}

inline UINT8 GAbsKeyboardManager::GetLastKeyPressed()
{
	return m_u8LastKeyPressed;
}

inline void GAbsKeyboardManager::SetLastKeyPressed(UINT8 u8key)
{
	m_u8LastKeyPressed = u8key;
}


#endif // GAbsKeyboardManager
