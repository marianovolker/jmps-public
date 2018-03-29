/*
 * © 2012 Gilbarco Inc.
 * Confidential and Proprietary
 *
 */

#ifndef GMenuManager_h
#define GMenuManager_h 1 

#include <GMutex.h>
#include <stack>

#include "SPOT/SPOTProtocolInterface/GSPOTSession.h"

class GMainMenu;
class GAbsMenu;

class GMenuManager : public GAbsSPOTSessionListener
{
	public:
		static GMenuManager* GetSoleInstance(void);
		static void DestroySoleInstance(void);

		/**
		 * @note This method creates the main screen, holding all the menus, and the main menu.
		 */
		 GMainMenu* Initialize();

		static bool isExitApplicationRequested();
		static void RequestExitApplication();
		       void ProcessInactiveTimeout(UINT32 tLeft);


		       void PushMenu(GAbsMenu* menu);
		       void PopMenu();
		       bool SendCommandToSPOT(GSPOTMessage* pMessage);
		       void QueueMenuPop();
		       void ProcessMessage(GSPOTMessage* pMessage);
		       void ProcessTimeoutEvent();
		       void NotifyError(GSpotDefs::eSPOTTransportLayerError eSessionError);


	protected:
		            GMenuManager();
		            ~GMenuManager();
		            // copy constructor
		            GMenuManager(const GMenuManager& right);
		            // assignment operator
		            GMenuManager& operator=(const GMenuManager& right);


	private:
		       void InitializeScreen();
		static bool isPinpadSystemStatusNotification(GSPOTMessage* pMessage);
		static bool isDiagnosticInfoNotification(GSPOTMessage* pMessage);
		static bool isServiceMessage(GSPOTMessage* pMessage);
		static bool isSecureMediaConnectorMessage(GSPOTMessage* pMessage);
		static bool ProcessSystemStatusNotification(GSPOTMessage* pMessage);
		static bool ProcessDiagnosticInfoNotification(GSPOTMessage* pMsg);
		static bool ProcessServiceMessage(GSPOTMessage* pMessage);
		static void ProcessOPTHardwareConfigMessageReply(GSPOTMessage* pMessage);
		static void ProcessOPTStatusInfoRequestMessageReply(GSPOTMessage* pMessage);

		static volatile bool  mst_blExitApplication;
		static GMenuManager*  mst_pSoleInstance;
        static GMutex m_createInstanceMutex;

		std::stack<GAbsMenu*> m_MenuStack;
		GSPOTSession*         m_pSession;
		GMutex                m_PopMutex;
		GMutex				  m_InstanceMutex;
		bool                  m_bShouldPopMenu;

};

#endif
