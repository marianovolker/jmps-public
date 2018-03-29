/*
 * © 2013 Gilbarco Inc.
 * Confidential and Proprietary
 *
 * @file GAckCodesPinpadApplications.h
 * @author Mariano Volker
 * @date Oct 02 2013
 * @copyright © 2013 Gilbarco Inc. Confidential and Propietary
 */

#ifndef GAckCodesPinpadApplications_h
#define GAckCodesPinpadApplications_h 1

#include <map>
#include <string>

#include "GTypes.h"
#include <GMutex.h>

class GAckCodesPinpadApplications
{
	public:

		typedef struct itemType
		{
			std::string sNemonic;
			std::string sDescription;

			itemType(const char *s1, const char *s2)
			{
				sNemonic=s1;
				sDescription=s2;
			}
		} itemType;

				   typedef std::map<int, itemType> MapType;

		inline static GAckCodesPinpadApplications* GetInstance();
		inline static GAckCodesPinpadApplications* GetOrCreateInstance();
		inline static GAckCodesPinpadApplications* CreateInstance();
								inline static void DestroyInstance();

											   int GetNextKey(int &iKey);
									   const char* GetDescription(int iAckCode);
									   const char* GetNemonic(int iAckCode);
											  bool SetDescription(int iAckCode, const char* descrip);

	private:

												   GAckCodesPinpadApplications();
										   virtual ~GAckCodesPinpadApplications();

										      void populateMap();

			   static GAckCodesPinpadApplications* m_pInstance;
								     static GMutex mst_InstanceMutex;

										   MapType mapAckCodes;
						  MapType::const_iterator* m_pIter;
};

inline GAckCodesPinpadApplications* GAckCodesPinpadApplications::GetOrCreateInstance()
{
	GMutexLock ml(mst_InstanceMutex);

	if (m_pInstance == NULL)
	{
		m_pInstance = new GAckCodesPinpadApplications();
	}

	return m_pInstance;
}

inline GAckCodesPinpadApplications* GAckCodesPinpadApplications::GetInstance()
{
	GMutexLock ml(mst_InstanceMutex);

	return m_pInstance;
}

inline GAckCodesPinpadApplications* GAckCodesPinpadApplications::CreateInstance()
{
	GMutexLock ml(mst_InstanceMutex);

	if (m_pInstance == NULL)
	{
		m_pInstance = new GAckCodesPinpadApplications();
	}

	return m_pInstance;
}

inline void GAckCodesPinpadApplications::DestroyInstance()
{
	GMutexLock ml(mst_InstanceMutex);

	if (m_pInstance != NULL)
	{
		delete m_pInstance;
		m_pInstance = NULL;
	}
}


#endif
