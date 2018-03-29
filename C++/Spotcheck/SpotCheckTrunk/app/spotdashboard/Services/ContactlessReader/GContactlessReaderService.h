/*!
 * © 2013 Gilbarco Inc.
 * Confidential and Proprietary
 *
 * @file GContactlessReaderService.h
 * @author Mariano Volker
 * @date Aug 27 2013
 * @copyright © 2013 Gilbarco Inc. Confidential and Propietary
 */

#ifndef GContactlessReaderService_h
#define GContactlessReaderService_h 1

#include "SPOT/SPOTProtocolInterface/GSPOTSession.h"

class GContactlessReaderService
{
	public:

		inline static GContactlessReaderService* GetService();
		inline static GContactlessReaderService* CreateInstance(GSPOTSession* pSession);
		                      inline static void DestroyInstance();

		                             static bool isContactlessReaderReply(GSPOTMessage* pMsg, bool blOnlySubCmd = false);

		                                    bool ProcessContactlessReaderReply(GSPOTMessage* pMsg);
		                                    bool ProcessSystemStatusNotification(UINT8 u8Status, UINT8 u8StatusEx, UINT8 u8Error);
 		                                    bool SendEnableCmdToSPOT();
		                                    bool SendDisableCmdToSPOT();


	private:
		                                         GContactlessReaderService(GSPOTSession* pSession);
		                                 virtual ~GContactlessReaderService();

		       static GContactlessReaderService* mst_pSoleInstance;
		                           static GMutex m_createInstanceMutex;
		                      static const UINT8 mst_aryu8EnableCLRCmd	[8];
		                      static const UINT8 mst_aryu8DisableCLRCmd	[6];

		                           GSPOTSession* m_pSession;
		                                  GMutex m_instanceMutex;
};


inline GContactlessReaderService* GContactlessReaderService::GetService()
{
	GMutexLock ml(m_createInstanceMutex);

	return mst_pSoleInstance;
}

inline GContactlessReaderService* GContactlessReaderService::CreateInstance(GSPOTSession* pSession)
{
	GMutexLock ml(m_createInstanceMutex);

	if (mst_pSoleInstance == NULL)
	{
		mst_pSoleInstance = new GContactlessReaderService(pSession);
	}

	return mst_pSoleInstance;
}

inline void GContactlessReaderService::DestroyInstance()
{
	GMutexLock ml(m_createInstanceMutex);

	if (mst_pSoleInstance != NULL)
	{
		delete mst_pSoleInstance;
		mst_pSoleInstance = NULL;
	}
}

#endif // GContactlessReaderService_h
