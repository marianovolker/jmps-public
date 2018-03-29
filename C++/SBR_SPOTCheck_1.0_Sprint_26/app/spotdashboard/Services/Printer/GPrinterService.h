/**
 * GPrinterService.h
 *
 * © 2013 Gilbarco Inc.
 * Confidential and Proprietary
 *
 * @file GPrinterService.h
 * @author Volker Mariano
 * @date Aug 27 2013
 * @copyright © 2013 Gilbarco Inc. Confidential and Proprietary
 */

#ifndef GPRINTERSERVICE_H_
#define GPRINTERSERVICE_H_

#include "Common/Util/DEBUGOUT.h"
#include "Common/Util/GSimpleCommand.h"

#include "SPOT/SPOTProtocolInterface/GSPOTSession.h"

class GPrinterService
{
public:

	inline static GPrinterService* GetService();
	inline static GPrinterService* CreateInstance(GSPOTSession* pSession);
	            inline static void DestroyInstance();

	                   static bool isPrinterReply(GSPOTMessage* pMsg, bool blOnlySubCmd = false);

					          bool ProcessOPTSystemStatusNotification(UINT8 u8Status, UINT8 u8StatusEx, UINT8 u8Error);
					          bool ProcessPrinterReply(GSPOTMessage* pMsg);

	                          bool QueryPrinterStatus();

private:
	                               GPrinterService(GSPOTSession* pSession);
	                       virtual ~GPrinterService();

	            static const UINT8 mst_aryu8PRTStatusCmd[7];
	       static GPrinterService* mst_pSoleInstance;
	                 static GMutex mst_InstanceMutex;

	                 GSPOTSession* m_pSession;
	                        GMutex m_instanceMutex;
};

inline GPrinterService* GPrinterService::GetService()
{
	GMutexLock ml(mst_InstanceMutex);

	return mst_pSoleInstance;
}

inline GPrinterService* GPrinterService::CreateInstance(GSPOTSession* pSession)
{
	GMutexLock ml(mst_InstanceMutex);

	if (mst_pSoleInstance == NULL)
	{
		mst_pSoleInstance = new GPrinterService(pSession);
	}

	return mst_pSoleInstance;
}

inline void GPrinterService::DestroyInstance()
{
	GMutexLock ml(mst_InstanceMutex);

	if (mst_pSoleInstance != NULL)
	{
		delete mst_pSoleInstance;
		mst_pSoleInstance = NULL;
	}
}

#endif /* GPRINTERSERVICE_H_ */
