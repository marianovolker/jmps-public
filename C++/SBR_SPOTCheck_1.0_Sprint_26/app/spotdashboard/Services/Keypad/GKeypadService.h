/**
 * © 2013 Gilbarco Inc.
 * Confidential and Proprietary
 *
 * @file GKeyPadService.h
 * @author Mariano volker
 * @date Aug 27 2013
 * @copyright © 2013 Gilbarco Inc. Confidential and Proprietary
 */

#ifndef GKeypadService_h
#define GKeypadService_h 1

#include "SPOT/SPOTProtocolInterface/GSPOTSession.h"
#include "GKeypadStateRuleProcessor.h"

class GKeypadService
{
	public:

					 inline static GKeypadService* GetService();
					 inline static GKeypadService* CreateInstance(GSPOTSession* pSession);
							    inline static void DestroyInstance();

									   static bool isKeypadReply(GSPOTMessage* pMsg, bool blOnlySubCmd = false);

											  bool ProcessKeypadReply(GSPOTMessage* pMsg);
											  bool ProcessSystemStatusNotification(UINT8 u8Status, UINT8 u8StatusEx, UINT8 u8Error);

											  bool SendEnableCmdToSPOT();
											  bool SendDisableCmdToSPOT();

		inline const GKeypadStateRuleProcessor & GetRulesProcessor() const {return m_oRulesProcessor;};

	private:

												   GKeypadService(GSPOTSession* pSession);
										   virtual ~GKeypadService();

						    static GKeypadService* mst_pSoleInstance;
									 static GMutex mst_createInstanceMutex;
								static const UINT8 mst_aryu8EnableKBCmd [20];
								static const UINT8 mst_aryu8DisableKBCmd[6];

									 GSPOTSession* m_pSession;
										    GMutex m_instanceMutex;

					   GKeypadStateRuleProcessor m_oRulesProcessor;
};

inline GKeypadService* GKeypadService::GetService()
{
	GMutexLock ml(mst_createInstanceMutex);

	return mst_pSoleInstance;
}

inline GKeypadService* GKeypadService::CreateInstance(GSPOTSession* pSession)
{
	GMutexLock ml(mst_createInstanceMutex);

	if (mst_pSoleInstance == NULL)
	{
		mst_pSoleInstance = new GKeypadService(pSession);
	}

	return mst_pSoleInstance;
}

inline void GKeypadService::DestroyInstance()
{
	GMutexLock ml(mst_createInstanceMutex);

	if (mst_pSoleInstance != NULL)
	{
		delete mst_pSoleInstance;
		mst_pSoleInstance = NULL;
	}
}

#endif
