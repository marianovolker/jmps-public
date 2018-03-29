/*
 * © 2013 Gilbarco Inc.
 * Confidential and Proprietary
 *
 * @file GSpotSysMainPinpadApplications.h
 * @author Mariano Volker
 * @date Oct 02 2013
 * @copyright © 2013 Gilbarco Inc. Confidential and Propietary
 */


#ifndef GSpotSysMainPinpadApplications_h
#define GSpotSysMainPinpadApplications_h 1

#include <map>
#include <string>

#include "GTypes.h"
#include <GMutex.h>

#include "GAbsSpotSysPinpadApplications.h"

class GSpotSysMainPinpadApplications : public GAbsSpotSysPinpadApplications
{
	public:

		inline static GSpotSysMainPinpadApplications* GetOrCreateInstance();
		inline static GSpotSysMainPinpadApplications* GetInstance();
		inline static GSpotSysMainPinpadApplications* CreateInstance();
								   inline static void DestroyInstance();

	private:
												      GSpotSysMainPinpadApplications();
											  virtual ~GSpotSysMainPinpadApplications();

										 virtual void populateStatesMap();
										 virtual void populateStateExtensionsMap();
										 virtual void populateErrorsMap();

			static GSpotSysMainPinpadApplications* m_pInstance;
								     static GMutex mst_InstanceMutex;
};

inline GSpotSysMainPinpadApplications* GSpotSysMainPinpadApplications::GetOrCreateInstance()
{
	GMutexLock ml(mst_InstanceMutex);

	if (m_pInstance == NULL)
	{
		m_pInstance = new GSpotSysMainPinpadApplications();
	}

	return m_pInstance;
}

inline GSpotSysMainPinpadApplications* GSpotSysMainPinpadApplications::GetInstance()
{
	GMutexLock ml(mst_InstanceMutex);

	return m_pInstance;
}

inline GSpotSysMainPinpadApplications* GSpotSysMainPinpadApplications::CreateInstance()
{
	GMutexLock ml(mst_InstanceMutex);

	if (m_pInstance == NULL)
	{
		m_pInstance = new GSpotSysMainPinpadApplications();
	}

	return m_pInstance;
}

inline void GSpotSysMainPinpadApplications::DestroyInstance()
{
	GMutexLock ml(mst_InstanceMutex);

	if (m_pInstance != NULL)
	{
		delete m_pInstance;
		m_pInstance = NULL;
	}
}


#endif
