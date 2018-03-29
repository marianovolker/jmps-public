/**
 * © 2013 Gilbarco Inc.
 * Confidential and Proprietary
 *
 * @file GCardReaderService.h
 * @author Volker Mariano
 * @date Aug 27 2013
 * @copyright © 2013 Gilbarco Inc. Confidential and Propietary
 */

#ifndef GCardReaderService_h
#define GCardReaderService_h 1

#include "SPOT/SPOTProtocolInterface/GSPOTSession.h"
#include "GCardReaderStateRuleProcessor.h"

class GCardReaderService
{
	public:

					 inline static GCardReaderService* GetService();
					 inline static GCardReaderService* CreateInstance(GSPOTSession* pSession);
									inline static void DestroyInstance();

										   static bool isCardReaderReply(GSPOTMessage* pMsg, bool blOnlySubCmd = false);

												  bool ProcessCardReaderReply(GSPOTMessage* pMsg);
												  bool ProcessSystemStatusNotification(UINT8 u8Status, UINT8 u8StatusEx, UINT8 u8Error);

												  bool SendEnableCmdToSPOT();
												  bool SendDisableCmdToSPOT();

		inline const GCardReaderStateRuleProcessor & GetRulesProcessor() const {return m_oRulesProcessor;};


	private:
													   GCardReaderService(GSPOTSession* pSession);
											   virtual ~GCardReaderService();


							static GCardReaderService* mst_pSoleInstance;
										 static GMutex m_createInstanceMutex;
									static const UINT8 mst_aryu8EnableCRCmd			[6];
									static const UINT8 mst_aryu8DisableCRCmd		[6];

										 GSPOTSession* m_pSession;
												GMutex m_instanceMutex;

					   GCardReaderStateRuleProcessor m_oRulesProcessor;
};

inline GCardReaderService* GCardReaderService::GetService()
{
	GMutexLock ml(m_createInstanceMutex);

	return mst_pSoleInstance;
}

inline GCardReaderService* GCardReaderService::CreateInstance(GSPOTSession* pSession)
{
	GMutexLock ml(m_createInstanceMutex);

	if (mst_pSoleInstance == NULL)
	{
		mst_pSoleInstance = new GCardReaderService(pSession);
	}

	return mst_pSoleInstance;
}

inline void GCardReaderService::DestroyInstance()
{
	GMutexLock ml(m_createInstanceMutex);

	if (mst_pSoleInstance != NULL)
	{
		delete mst_pSoleInstance;
		mst_pSoleInstance = NULL;
	}
}

#endif // GCardReaderService_h
