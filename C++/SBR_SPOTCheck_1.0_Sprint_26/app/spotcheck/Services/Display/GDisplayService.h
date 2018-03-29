/*!
 * © 2013 Gilbarco Inc.
 * Confidential and Proprietary
 *
 * @file GDisplayService.h
 * @author Mariano volker
 * @date June 12 2013
 * @copyright © 2013 Gilbarco Inc. Confidential and Propietary
 */

#ifndef GDisplayService_h
#define GDisplayService_h 1

#include "SPOT/SPOTProtocolInterface/GSPOTSession.h"
#include "Menus/GMenuManager.h"
#include "GDisplayServiceStatistics.h"

const int    DY_CREATE_RESWIN_CMD_LEN   = 13;
const int    DY_DESTROY_WIN_CMD_LEN     =  9;
const int    DY_SHOW_WINTAG_CMD_LEN     = 10;
const int    DY_HIDE_WINTAG_CMD_LEN     =  9;
const int    DY_SHOW_TAGARRAY_CMD_LEN   =  8;
const int    DY_HIDE_TAGARRAY_CMD_LEN   =  8;
const UINT8  DY_DEFAULT_ZORDER          =  0;

class GDisplayManager;

class GDisplayService
{
public:

	struct SHOW_TAG
	{
		UINT8 u8TagId;
		UINT8 u8Zorder;
	};

public:

		inline static GDisplayService* GetService();
			   static GDisplayService* CreateInstance(GSPOTSession* pSession);
						   static void DestroyInstance();
	inline GDisplayServiceStatistics & GetStatistics() {return m_Statistics;}

								  void AttachManager(GDisplayManager* pMgr);
								  void DetachManager();

								  bool CreateWindowFromResource(UINT8 u8WinId, UINT8 u8ResId);
								  bool DestroyWindow(UINT8 u8WinId);
						   inline bool ShowWindow(UINT8 u8WinId);
						   inline bool HideWindow(UINT8 u8WinId);
								  bool ShowWindowTag(UINT8 u8WinId, UINT8 u8TagId, UINT8 u8Zorder = 0);
								  bool HideWindowTag(UINT8 u8WinId, UINT8 u8TagId);
								  bool ShowWindowTagArray(UINT8 u8WinId, UINT8 u8Qty, SHOW_TAG* pTags);
								  bool ShowWindowTagArray(UINT8 u8WinId, UINT8 u8Qty, UINT8* pTagIDs);
								  bool HideWindowTagArray(UINT8 u8WinId, UINT8 u8Qty, UINT8* pTagIDs);
								  bool SendCmdToSPOT(GSPOTMessage* pMessage);

private:

									   GDisplayService(GSPOTSession* pSession);
							   virtual ~GDisplayService();

				   static const UINT8  mst_aryu8CreateResourceWindowCmd[DY_CREATE_RESWIN_CMD_LEN];
				   static const UINT8  mst_aryu8DestroyWindowCmd[DY_DESTROY_WIN_CMD_LEN];
				   static const UINT8  mst_aryu8ShowWindowTagCmd[DY_SHOW_WINTAG_CMD_LEN];
				   static const UINT8  mst_aryu8HideWindowTagCmd[DY_HIDE_WINTAG_CMD_LEN];


			   static GDisplayService* mst_pSoleInstance;
						 static GMutex mst_createInstanceMutex;

								GMutex m_instanceMutex;
						 GSPOTSession* m_pSession;
					  GDisplayManager* m_pManager;
			 GDisplayServiceStatistics m_Statistics;
};


inline GDisplayService* GDisplayService::GetService()
{
	GMutexLock ml(mst_createInstanceMutex);

	return mst_pSoleInstance;
}

inline bool GDisplayService::ShowWindow(UINT8 u8WinId)
{
	return ShowWindowTag(u8WinId, 0);
}

inline bool GDisplayService::HideWindow(UINT8 u8WinId)
{
	return HideWindowTag(u8WinId, 0);
}

#endif
