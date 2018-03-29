/*
 * © 2013 Gilbarco Inc.
 * Confidential and Proprietary
 *
 * @file GSpotSysCardReaderPinpadApplications.h
 * @author Mariano Volker
 * @date Oct 02 2013
 * @copyright © 2013 Gilbarco Inc. Confidential and Propietary
 */


#ifndef GSpotSysCardReaderPinpadApplications_h
#define GSpotSysCardReaderPinpadApplications_h 1

#include <map>
#include <string>

#include "GTypes.h"
#include <GMutex.h>

#include "GAbsSpotSysPinpadApplications.h"

class GSpotSysCardReaderPinpadApplications : public GAbsSpotSysPinpadApplications
{
	public:

		inline static GSpotSysCardReaderPinpadApplications* GetOrCreateInstance();
		inline static GSpotSysCardReaderPinpadApplications* GetInstance();
		inline static GSpotSysCardReaderPinpadApplications* CreateInstance();
						  				inline static void DestroyInstance();

	private:
											  			   GSpotSysCardReaderPinpadApplications();
											  	   virtual ~GSpotSysCardReaderPinpadApplications();

											  virtual void populateStatesMap();
											  virtual void populateStateExtensionsMap();
											  virtual void populateErrorsMap();

			static GSpotSysCardReaderPinpadApplications* m_pInstance;
									      static GMutex mst_InstanceMutex;
};

inline GSpotSysCardReaderPinpadApplications* GSpotSysCardReaderPinpadApplications::GetOrCreateInstance()
{
	GMutexLock ml(mst_InstanceMutex);

	if (m_pInstance == NULL)
	{
		m_pInstance = new GSpotSysCardReaderPinpadApplications();
	}

	return m_pInstance;
}

inline GSpotSysCardReaderPinpadApplications* GSpotSysCardReaderPinpadApplications::GetInstance()
{
	GMutexLock ml(mst_InstanceMutex);
	return m_pInstance;
}

inline GSpotSysCardReaderPinpadApplications* GSpotSysCardReaderPinpadApplications::CreateInstance()
{
	GMutexLock ml(mst_InstanceMutex);

	if (m_pInstance == NULL)
	{
		m_pInstance = new GSpotSysCardReaderPinpadApplications();
	}

	return m_pInstance;
}

inline void GSpotSysCardReaderPinpadApplications::DestroyInstance()
{
	GMutexLock ml(mst_InstanceMutex);

	if (m_pInstance != NULL)
	{
		delete m_pInstance;
		m_pInstance = NULL;
	}
}


#endif
