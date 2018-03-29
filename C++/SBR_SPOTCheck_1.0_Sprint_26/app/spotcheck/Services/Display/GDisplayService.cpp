/*
 * © 2013 Gilbarco Inc.
 * Confidential and Proprietary
 *
 * @file GDisplayService.h
 * @author Mariano Volker
 * @date June 03 2013
 * @copyright © 2013 Gilbarco Inc. Confidential and Propietary
 */

#include <unistd.h>
#include <cstring>

#include "GDisplayService.h"

#include "Common/Util/DEBUGOUT.h"
#include "Common/Util/GConversion.h"
#include "Menus/Display/GDisplayManager.h"
#include "SPOT/SPOTProtocolInterface/GSPOTSession.h"

const UINT8 GDisplayService::mst_aryu8CreateResourceWindowCmd[DY_CREATE_RESWIN_CMD_LEN] =
            {0x00, 0x0D, 0x01, 0x00, 0x30, 0x0A, 0x00, 0x00, 0xFF, 0x00, 0x00, 0x00, 0x00};

const UINT8 GDisplayService::mst_aryu8DestroyWindowCmd[DY_DESTROY_WIN_CMD_LEN] =
            {0x00, 0x09, 0x01, 0x00, 0x30, 0x04, 0x00, 0x01, 0x00};

const UINT8 GDisplayService::mst_aryu8ShowWindowTagCmd[DY_SHOW_WINTAG_CMD_LEN] =
            {0x00, 0x0A, 0x01, 0x00, 0x30, 0x07, 0x00, 0x01, 0x00, 0x00};

const UINT8 GDisplayService::mst_aryu8HideWindowTagCmd[DY_HIDE_WINTAG_CMD_LEN] =
            {0x00, 0x09, 0x01, 0x00, 0x30, 0x08, 0x00, 0x01, 0x00};

GDisplayService* GDisplayService::mst_pSoleInstance = NULL;
GMutex GDisplayService::mst_createInstanceMutex;

GDisplayService::GDisplayService(GSPOTSession* pSession):
		m_pSession(pSession),
		m_pManager(NULL)
{
	LOG(eDisplay, eInfo, "Display Service started.\n");
}

GDisplayService::~GDisplayService()
{
	LOG(eDisplay, eInfo, "Display Service stopped.\n");
}

GDisplayService* GDisplayService::CreateInstance(GSPOTSession* pSession)
{
	GMutexLock ml(mst_createInstanceMutex);

	if (mst_pSoleInstance == NULL)
	{
		mst_pSoleInstance = new GDisplayService(pSession);
	}

	return mst_pSoleInstance;
}

void GDisplayService::DestroyInstance()
{
	GMutexLock ml(mst_createInstanceMutex);

	if (mst_pSoleInstance != NULL)
	{
		delete mst_pSoleInstance;
		mst_pSoleInstance = NULL;
	}
}

void GDisplayService::AttachManager(GDisplayManager* pMgr)
{
	GMutexLock ml(m_instanceMutex);

	if( !pMgr )
	{
		return;
	}

	m_pManager = pMgr;
}

void GDisplayService::DetachManager()
{
	GMutexLock ml(m_instanceMutex);

	m_pManager = NULL;
}

bool GDisplayService::CreateWindowFromResource(UINT8 u8WinId, UINT8 u8ResId)
{
	if(m_pSession == NULL)
		return false;

	UINT8 aryu8CreateCmd[DY_CREATE_RESWIN_CMD_LEN];

	memcpy(aryu8CreateCmd, mst_aryu8CreateResourceWindowCmd, sizeof(aryu8CreateCmd));
	aryu8CreateCmd[6] = u8WinId;
	aryu8CreateCmd[7] = u8ResId;

	GSPOTMessage oMsg(aryu8CreateCmd);
	bool blret = SendCmdToSPOT(&oMsg);
	LOG(eDisplay, eDebug,
	    "GDisplayService::CreateWindowFromResource - WinID=%d, ResID=%d   %s\n",
	    int(u8WinId), int(u8ResId), blret?"OK":"FAIL");
	return blret;
}

bool GDisplayService::DestroyWindow(UINT8 u8WinId)
{
	if(m_pSession == NULL)
		return false;

	UINT8 aryu8DestroyCmd[DY_DESTROY_WIN_CMD_LEN];

	memcpy(aryu8DestroyCmd, mst_aryu8DestroyWindowCmd, sizeof(aryu8DestroyCmd));
	aryu8DestroyCmd[6] = u8WinId;

	GSPOTMessage oMsg(aryu8DestroyCmd);
	return SendCmdToSPOT(&oMsg);
}

bool GDisplayService::ShowWindowTag(UINT8 u8WinId, UINT8 u8TagId, UINT8 u8Zorder /* = 0 */)
{
	if(m_pSession == NULL)
		return false;

	UINT8 aryu8ShowCmd[DY_SHOW_WINTAG_CMD_LEN];

	memcpy(aryu8ShowCmd, mst_aryu8ShowWindowTagCmd, sizeof(aryu8ShowCmd));
	aryu8ShowCmd[6] = u8WinId;
	aryu8ShowCmd[8] = u8TagId;

	if(u8Zorder != 0)
		aryu8ShowCmd[9] = u8Zorder;

	GSPOTMessage oMsg(aryu8ShowCmd);
	return SendCmdToSPOT(&oMsg);
}

bool GDisplayService::HideWindowTag(UINT8 u8WinId, UINT8 u8TagId)
{
	if(m_pSession == NULL)
		return false;

	UINT8 aryu8HideCmd[DY_HIDE_WINTAG_CMD_LEN];

	memcpy(aryu8HideCmd, mst_aryu8HideWindowTagCmd, sizeof(aryu8HideCmd));
	aryu8HideCmd[6] = u8WinId;
	aryu8HideCmd[8] = u8TagId;

	GSPOTMessage oMsg(aryu8HideCmd);
	return SendCmdToSPOT(&oMsg);
}

bool GDisplayService::ShowWindowTagArray(UINT8 u8WinId, UINT8 u8Qty, SHOW_TAG* aryTags)
{
	if(m_pSession == NULL || u8Qty == 0 || aryTags == NULL)
		return false;

	int    i            = DY_SHOW_TAGARRAY_CMD_LEN;
	int    iExtraLength = 2 * u8Qty;
	int    iCmdSize     = i + iExtraLength;
	int    j            = 0;

	UINT8* aryu8ShowCmd = new UINT8[DY_SHOW_TAGARRAY_CMD_LEN + iExtraLength];

	memcpy(aryu8ShowCmd, mst_aryu8ShowWindowTagCmd, DY_SHOW_TAGARRAY_CMD_LEN);
	aryu8ShowCmd[0] = UINT8(iCmdSize / 256);
	aryu8ShowCmd[1] = UINT8(iCmdSize % 256);
	aryu8ShowCmd[6] = u8WinId;
	aryu8ShowCmd[7] = u8Qty;

	while(i < iCmdSize)
	{
		aryu8ShowCmd[i++] = aryTags[j++].u8TagId;
		aryu8ShowCmd[i++] = aryTags[j++].u8Zorder;
	}

	GSPOTMessage oMsg(aryu8ShowCmd);
	delete [] aryu8ShowCmd;
	return SendCmdToSPOT(&oMsg);
}

bool GDisplayService::ShowWindowTagArray(UINT8 u8WinId, UINT8 u8Qty, UINT8* aryTagIDs)
{
	if(m_pSession == NULL || u8Qty == 0 || aryTagIDs == NULL)
		return false;

	int    i            = DY_SHOW_TAGARRAY_CMD_LEN;
	int    iExtraLength = 2 * u8Qty;
	int    iCmdSize     = i + iExtraLength;
	int    j            = 0;

	UINT8* aryu8ShowCmd = new UINT8[DY_SHOW_TAGARRAY_CMD_LEN + iExtraLength];

	memcpy(aryu8ShowCmd, mst_aryu8ShowWindowTagCmd, DY_SHOW_TAGARRAY_CMD_LEN);
	aryu8ShowCmd[0] = UINT8(iCmdSize / 256);
	aryu8ShowCmd[1] = UINT8(iCmdSize % 256);
	aryu8ShowCmd[6] = u8WinId;
	aryu8ShowCmd[7] = u8Qty;

	while(i < iCmdSize)
	{
		aryu8ShowCmd[i++] = aryTagIDs[j++];
		aryu8ShowCmd[i++] = DY_DEFAULT_ZORDER;
	}

	GSPOTMessage oMsg(aryu8ShowCmd);
	delete [] aryu8ShowCmd;
	return SendCmdToSPOT(&oMsg);
}

bool GDisplayService::HideWindowTagArray(UINT8 u8WinId, UINT8 u8Qty, UINT8* aryTagIDs)
{
	if(m_pSession == NULL || u8Qty == 0 || aryTagIDs == NULL)
		return false;

	int    i            = DY_HIDE_TAGARRAY_CMD_LEN;
	int    iCmdSize     = i + u8Qty;
	int    j            = 0;

	UINT8* aryu8HideCmd = new UINT8[DY_HIDE_TAGARRAY_CMD_LEN + u8Qty];

	memcpy(aryu8HideCmd, mst_aryu8ShowWindowTagCmd, DY_HIDE_TAGARRAY_CMD_LEN);
	aryu8HideCmd[0] = UINT8(iCmdSize / 256);
	aryu8HideCmd[1] = UINT8(iCmdSize % 256);
	aryu8HideCmd[6] = u8WinId;
	aryu8HideCmd[7] = u8Qty;

	while(i < iCmdSize)
	{
		aryu8HideCmd[i++] = aryTagIDs[j++];
	}

	GSPOTMessage oMsg(aryu8HideCmd);
	return SendCmdToSPOT(&oMsg);
}

bool GDisplayService::SendCmdToSPOT(GSPOTMessage* pMessage)
{
	if( m_pSession == NULL || pMessage == NULL )
	{
		return false;
	}

	return m_pSession->SendSPOTCommand( pMessage );
}
