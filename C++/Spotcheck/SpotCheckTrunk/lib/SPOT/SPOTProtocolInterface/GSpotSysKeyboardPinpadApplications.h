/*
 * © 2013 Gilbarco Inc.
 * Confidential and Proprietary
 *
 * @file GSpotSysKeyboardPinpadApplications.h
 * @author Mariano Volker
 * @date Oct 02 2013
 * @copyright © 2013 Gilbarco Inc. Confidential and Propietary
 */

#ifndef GSpotSysKeyboardPinpadApplications_h
#define GSpotSysKeyboardPinpadApplications_h 1

#include <map>
#include <string>

#include "GTypes.h"
#include <GMutex.h>

#include "GAbsSpotSysPinpadApplications.h"

class GSpotSysKeyboardPinpadApplications : public GAbsSpotSysPinpadApplications
{
	public:
		inline static GSpotSysKeyboardPinpadApplications* GetOrCreateInstance();
		inline static GSpotSysKeyboardPinpadApplications* GetInstance();
		inline static GSpotSysKeyboardPinpadApplications* CreateInstance();
						  			  inline static void DestroyInstance();
	private:

											  			 GSpotSysKeyboardPinpadApplications();
											  	 virtual ~GSpotSysKeyboardPinpadApplications();

											virtual void populateStatesMap();
											virtual void populateStateExtensionsMap();
											virtual void populateErrorsMap();

			   static GSpotSysKeyboardPinpadApplications* m_pInstance;
									       static GMutex mst_InstanceMutex;
};

inline GSpotSysKeyboardPinpadApplications* GSpotSysKeyboardPinpadApplications::GetOrCreateInstance()
{
	GMutexLock ml(mst_InstanceMutex);

	if (m_pInstance == NULL)
	{
		m_pInstance = new GSpotSysKeyboardPinpadApplications();
	}

	return m_pInstance;
}

inline GSpotSysKeyboardPinpadApplications* GSpotSysKeyboardPinpadApplications::GetInstance()
{
	GMutexLock ml(mst_InstanceMutex);
	return m_pInstance;
}

inline GSpotSysKeyboardPinpadApplications* GSpotSysKeyboardPinpadApplications::CreateInstance()
{
	GMutexLock ml(mst_InstanceMutex);

	if (m_pInstance == NULL)
	{
		m_pInstance = new GSpotSysKeyboardPinpadApplications();
	}

	return m_pInstance;
}

inline void GSpotSysKeyboardPinpadApplications::DestroyInstance()
{
	GMutexLock ml(mst_InstanceMutex);

	if (m_pInstance != NULL)
	{
		delete m_pInstance;
		m_pInstance = NULL;
	}
}


#endif
