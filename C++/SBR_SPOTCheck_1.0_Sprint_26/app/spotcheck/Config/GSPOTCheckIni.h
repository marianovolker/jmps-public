/*!
 * © 2013 Gilbarco Inc.
 * Confidential and Proprietary
 *
 * @file GSPOTCheckIni.h
 * @author Sebastián Lipchak
 * @date Jul 26 2013
 * @copyright © 2013 Gilbarco Inc. Confidential and Propietary
 */


#ifndef GSPOTCHECKINI_H_
#define GSPOTCHECKINI_H_

#include <GTypes.h>
#include <GMutex.h>
#include <string>
#include "Common/Logger/ConfigLabels.h"

class GSPOTCheckIni
{
	public:

		   inline static GSPOTCheckIni* GetSoleInstance();
		             inline static void CreateInstance();
		             inline static void Destroy();

		             inline const char* GetSPOTIpAddress();
		             inline const char* GetINIFilename();
		             inline const UINT8 GetKeymatrixScreenResID();
		             inline const UINT8 GetSoftkeyScreenResID();
		             inline const UINT8 GetAuxkeyScreenResID();
		                           bool Initialize();

	private:
		                                GSPOTCheckIni();
		                        virtual ~GSPOTCheckIni();

		                                bool AskIP(bool bAskIP);

		          static GSPOTCheckIni* mst_pInstance;
		                  static GMutex mst_Mutex;

		               static const int INI_BUFFER_SIZE_KEYVALUE;

		                          UINT8 m_u8KeypadResId;
		                          UINT8 m_u8SoftkeyResId;
		                          UINT8 m_u8AuxkeyResId;
		                    std::string m_strIPAddress;
};

inline void GSPOTCheckIni::CreateInstance()
{
	if( mst_pInstance )
	{
		return;
	}

   GMutexLock lock(mst_Mutex);
   mst_pInstance = new GSPOTCheckIni();
}

inline void GSPOTCheckIni::Destroy()
{
   GMutexLock lock(mst_Mutex);
   if( mst_pInstance != NULL )
   {
      delete mst_pInstance;
      mst_pInstance = NULL;
   }
}

inline GSPOTCheckIni* GSPOTCheckIni::GetSoleInstance()
{
   GMutexLock lock(mst_Mutex);
   return mst_pInstance;
}

inline const char* GSPOTCheckIni::GetSPOTIpAddress()
{
	return m_strIPAddress.c_str();
}

inline const char* GSPOTCheckIni::GetINIFilename()
{
	return INI_SPOTCHECK_CONFIG_DEFAULT_FILE;
}

inline const UINT8 GSPOTCheckIni::GetKeymatrixScreenResID()
{
	return m_u8KeypadResId;
}

inline const UINT8 GSPOTCheckIni::GetSoftkeyScreenResID()
{
	return m_u8SoftkeyResId;
}

inline const UINT8 GSPOTCheckIni::GetAuxkeyScreenResID()
{
	return m_u8AuxkeyResId;
}

#endif /* GSPOTCHECKINI_H_ */
