/*
 * GKeyMatrixWindowFromResource.cpp
 *
 *  Created on: 01/07/2013
 *      Author: Guillermo Paris
 */

#include "Common/Util/DEBUGOUT.h"
#include "Services/Display/GDisplayService.h"
#include "GKeyMatrixWindowFromResource.h"

const UINT8 GKeyMatrixWindowFromResource::mst_Cu8TestTitle_TagID  = RESOURCE_WIN_TAG_TEST_TITLE;
const UINT8 GKeyMatrixWindowFromResource::mst_Cu8StatsTitle_TagID = RESOURCE_WIN_TAG_STATS_TITLE;
const UINT8 GKeyMatrixWindowFromResource::mst_Cu8StatsExit_TagID  = RESOURCE_WIN_TAG_STATS_EXIT;
const UINT8 GKeyMatrixWindowFromResource::mst_Cu8WinTag_Activation_Offset = RESOURCE_WIN_TAG_ACTIVATION;


GKeyMatrixWindowFromResource::GKeyMatrixWindowFromResource(UINT8 u8ResID)
  :	GWindowFromResource(u8ResID)
{
}

GKeyMatrixWindowFromResource::~GKeyMatrixWindowFromResource()
{
}

bool GKeyMatrixWindowFromResource::MarkKeyPressed(UINT8 u8KeyCode)
{
	if(m_u8WindowId == 0 ) // bad resource ID at creation time
		return false;

	GDisplayService* pDYService = GDisplayService::GetService();
	if( pDYService == NULL )
		return false;

	bool  blret      = false;
	UINT8 u8TagIdOFF = 1 + u8KeyCode;
	UINT8 u8TagIdON  = u8TagIdOFF + mst_Cu8WinTag_Activation_Offset;

	LOG(eUnCategorized, eDebug,
	    "GKeyMatrixWindowFromResource::MarkKeyPressed() - Tags: Hide %d , Show %d\n",
	    u8TagIdOFF, u8TagIdON);

	blret = pDYService->ShowWindowTag(m_u8WindowId, u8TagIdON, 2);
	return blret;
}

bool GKeyMatrixWindowFromResource::MarkForStatistics(UINT uQty, UINT8* pu8KeyCodes,
                                                     bool blTitleSwap,  /* = false */
                                                     bool blExitTagSwap /* = false */)
{
	if(m_u8WindowId == 0 || pu8KeyCodes == NULL)
		return false;

	GDisplayService* pDYService = GDisplayService::GetService();
	if( pDYService == NULL )
		return false;

	UINT   uIndexBase  = 0;

	// Holds all the pressed key tag IDs, plus title and exit tagIDs.
	UINT8* aryu8TagIds = new UINT8[ 2 + uQty ];

	if(blTitleSwap)
		aryu8TagIds[uIndexBase++] = mst_Cu8StatsTitle_TagID;

	if(blExitTagSwap)
		aryu8TagIds[uIndexBase++] = mst_Cu8StatsExit_TagID;

	UINT uIndex = uIndexBase;

	while(uIndex - uIndexBase < uQty)
		aryu8TagIds[uIndex++] = 1 + pu8KeyCodes[uIndex-uIndexBase] + mst_Cu8WinTag_Activation_Offset;

	return pDYService->ShowWindowTagArray(m_u8WindowId, uIndexBase + uQty, aryu8TagIds);
}
