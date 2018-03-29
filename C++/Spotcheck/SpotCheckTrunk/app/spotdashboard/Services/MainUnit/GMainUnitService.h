/**
 * © 2013 Gilbarco Inc.
 * Confidential and Proprietary
 *
 * @file GMainUnitService.h
 * @author Guillermo Paris
 * @date Sep 30 2013
 * @copyright © 2013 Gilbarco Inc. Confidential and Proprietary
 */

#include "GMainUnitStateRuleProcessor.h"
#include "SPOT/SPOTProtocolInterface/GSPOTSession.h"

#ifndef GMAINUNITSERVICE_H_
#define GMAINUNITSERVICE_H_

class GMainUnitService
{
public:
	inline static GMainUnitService* GetService();
	       static GMainUnitService* CreateInstance(GSPOTSession* pSession);
	       static              void DestroyInstance();

	                           bool SendStatusRequestCmdToSPOT();
	                           bool ProcessSystemStatusNotification(UINT8 u8Status, UINT8 u8StatusEx, UINT8 u8Error);

	inline const GMainUnitStateRuleProcessor & GetRulesProcessor() const {return m_oRulesProcessor;}

private:
	                                GMainUnitService(GSPOTSession* pSession);
	                                ~GMainUnitService();

	static GMainUnitService*        mst_pSoleInstance;
	static GMutex                   mst_createInstanceMutex;
	static const UINT8              mst_aryu8SystemStatusRequestCmd[7];

	GSPOTSession*                   m_pSession;
	GMutex                          m_instanceMutex;
	GMainUnitStateRuleProcessor     m_oRulesProcessor;
};

inline GMainUnitService* GMainUnitService::GetService()
{
	GMutexLock ml(mst_createInstanceMutex);

	return mst_pSoleInstance;
}


#endif /* GMAINUNITSERVICE_H_ */
