/*
 * © 2012 Gilbarco Inc.
 * Confidential and Proprietary
 *
 */

#ifndef GMessageManager_h
#define GMessageManager_h 1

#include <GMutex.h>
#include <stack>

#include "SPOT/SPOTProtocolInterface/GSPOTSession.h"

class GMessageManager : public GAbsSPOTSessionListener
{
	public:
		static GMessageManager* GetSoleInstance(void);
		            static void DestroySoleInstance(void);

		            static bool isExitApplicationRequested();
		            static void RequestExitApplication();

		                   bool SendCommandToSPOT(GSPOTMessage* pMessage);
		                   void ProcessMessage(GSPOTMessage* pMessage);
		                   void ProcessTimeoutEvent () {};
		                   void NotifyError(GSpotDefs::eSPOTTransportLayerError eSessionError);

	protected:
		                        GMessageManager();
		                virtual ~GMessageManager();

	private:
		            static bool isPinpadSystemStatusNotification(GSPOTMessage* pMessage);
		            static bool isServiceMessage(GSPOTMessage* pMessage);
		            static bool ProcessSystemStatusNotification(GSPOTMessage* pMessage);
		            static bool ProcessServiceMessage(GSPOTMessage* pMessage);
		            static void ProcessOPTHardwareConfigMessageReply(GSPOTMessage* pMessage);
		            static void ProcessOPTStatusInfoRequestMessageReply(GSPOTMessage* pMessage);

		//          These are not needed in this release.
		//          static bool isDiagnosticInfoNotification(GSPOTMessage* pMessage);
		//          static bool ProcessDiagnosticInfoNotification(GSPOTMessage* pMsg);

		   static volatile bool mst_blExitApplication;
		static GMessageManager* mst_pSoleInstance;
		          static GMutex m_createInstanceMutex;
		          static GMutex m_InstanceMutex;
		          GSPOTSession* m_pSession;
};

#endif
