/*
 * © 2013 Gilbarco Inc.
 * Confidential and Proprietary
 *
 * @file GSpotSysSecurityModulePinpadApplications.h
 * @author Mariano Volker
 * @date Oct 02 2013
 * @copyright © 2013 Gilbarco Inc. Confidential and Propietary
 */


#ifndef GSpotSysSecurityModulePinpadApplications_h
#define GSpotSysSecurityModulePinpadApplications_h 1

#include <map>
#include <string>

#include "GTypes.h"
#include <GMutex.h>

#include "GAbsSpotSysPinpadApplications.h"

class GSpotSysSecurityModulePinpadApplications : public GAbsSpotSysPinpadApplications
{
	public:
		inline static GSpotSysSecurityModulePinpadApplications* GetOrCreateInstance();
		inline static GSpotSysSecurityModulePinpadApplications* GetInstance();
		inline static GSpotSysSecurityModulePinpadApplications* CreateInstance();
											inline static void DestroyInstance();
	private:
															   GSpotSysSecurityModulePinpadApplications();
													   virtual ~GSpotSysSecurityModulePinpadApplications();

												  virtual void populateStatesMap();
												  virtual void populateStateExtensionsMap();
												  virtual void populateErrorsMap();

			   static GSpotSysSecurityModulePinpadApplications* m_pInstance;
											     static GMutex mst_InstanceMutex;
};

inline GSpotSysSecurityModulePinpadApplications* GSpotSysSecurityModulePinpadApplications::GetOrCreateInstance()
{
	GMutexLock ml(mst_InstanceMutex);

	if (m_pInstance == NULL)
	{
		m_pInstance = new GSpotSysSecurityModulePinpadApplications();
	}

	return m_pInstance;
}

inline GSpotSysSecurityModulePinpadApplications* GSpotSysSecurityModulePinpadApplications::GetInstance()
{
	GMutexLock ml(mst_InstanceMutex);

	return m_pInstance;
}

inline GSpotSysSecurityModulePinpadApplications* GSpotSysSecurityModulePinpadApplications::CreateInstance()
{
	GMutexLock ml(mst_InstanceMutex);

	if (m_pInstance == NULL)
	{
		m_pInstance = new GSpotSysSecurityModulePinpadApplications();
	}

	return m_pInstance;
}

inline void GSpotSysSecurityModulePinpadApplications::DestroyInstance()
{
	GMutexLock ml(mst_InstanceMutex);

	if (m_pInstance != NULL)
	{
		delete m_pInstance;
		m_pInstance = NULL;
	}
}


#endif
