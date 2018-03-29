/**
 * © 2013 Gilbarco Inc.
 * Confidential and Proprietary
 *
 * @file GSecurityModuleService.h
 * @author Guillermo Paris
 * @date Sep 30 2013
 * @copyright © 2013 Gilbarco Inc. Confidential and Proprietary
 */

#include "GSecurityModuleStateRuleProcessor.h"
#include "SPOT/SPOTProtocolInterface/GSPOTSession.h"

#ifndef GSECURITYMODULESERVICE_H_
#define GSECURITYMODULESERVICE_H_

class GSecurityModuleService
{
public:
	inline static GSecurityModuleService* GetService();
	       static GSecurityModuleService* CreateInstance(GSPOTSession* pSession);
	       static                    void DestroyInstance();

	                                 bool SendStatusRequestCmdToSPOT();
	                                 bool ProcessSystemStatusNotification(UINT8 u8Status, UINT8 u8StatusEx, UINT8 u8Error);

	inline const GSecurityModuleStateRuleProcessor & GetRulesProcessor() const {return m_oRulesProcessor;}

private:
	                                      GSecurityModuleService(GSPOTSession* pSession);
	                                      ~GSecurityModuleService();

	static GSecurityModuleService*        mst_pSoleInstance;
	static GMutex                         mst_createInstanceMutex;

	GSPOTSession*                         m_pSession;
	GMutex                                m_instanceMutex;
	GSecurityModuleStateRuleProcessor     m_oRulesProcessor;
};


inline GSecurityModuleService* GSecurityModuleService::GetService()
{
	GMutexLock ml(mst_createInstanceMutex);

	return mst_pSoleInstance;
}

#endif /* GSECURITYMODULESERVICE_H_ */
