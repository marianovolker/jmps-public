/*!
 * © 2013 Gilbarco Inc.
 * Confidential and Proprietary
 *
 * @file SPOTDashboardIni.h
 * @author Volker Mariano
 * @date Aug 29 2013
 * @copyright © 2013 Gilbarco Inc. Confidential and Propietary
 */


#ifndef GSPOTDASHBOARDINI_H_
#define GSPOTDASHBOARDINI_H_

#include <GMutex.h>
#include <string>

#include "GTypes.h"

class SPOTDashboardIni
{
	public:

	    inline static SPOTDashboardIni* GetSoleInstance();
	                 inline static void CreateInstance();
	                 inline static void Destroy();

	                 inline static void SetINIFilename( const char* szName ) {INI_DASHBOARD_CONFIG_DEFAULT_FILE = szName;};

		             inline const char* GetSPOTIpAddress();
		             inline const char* GetINIFilename();
		                           bool Initialize();

	private:
		                                SPOTDashboardIni();
		                        virtual ~SPOTDashboardIni();

								   bool AskIP(bool bAskIP);
								   bool LoadPropietaryCodes(const char * czIniSection);
								   bool SetDescriptionToAppropriateInternalMap(const char * czIniSection, UINT8 uiKey, const char * czNewDescription);

	           static SPOTDashboardIni* mst_pInstance;
	                      static GMutex mst_Mutex;

		              static const int INI_BUFFER_SIZE_KEYVALUE;
		           static const char * INI_DASHBOARD_CONFIG_DEFAULT_FILE;

		                   std::string m_strIPAddress;
};


inline void SPOTDashboardIni::CreateInstance()
{
	if( mst_pInstance )
	{
		return;
	}

   GMutexLock lock(mst_Mutex);
   mst_pInstance = new SPOTDashboardIni();
}

inline void SPOTDashboardIni::Destroy()
{
   GMutexLock lock(mst_Mutex);
   if( mst_pInstance != NULL )
   {
      delete mst_pInstance;
      mst_pInstance = NULL;
   }
}

inline SPOTDashboardIni* SPOTDashboardIni::GetSoleInstance()
{
   GMutexLock lock(mst_Mutex);
   return mst_pInstance;
}

inline const char* SPOTDashboardIni::GetSPOTIpAddress()
{
	return m_strIPAddress.c_str();
}

inline const char* SPOTDashboardIni::GetINIFilename()
{
	return INI_DASHBOARD_CONFIG_DEFAULT_FILE;
}



#endif /* GSPOTDASHBOARDINI_H_ */
