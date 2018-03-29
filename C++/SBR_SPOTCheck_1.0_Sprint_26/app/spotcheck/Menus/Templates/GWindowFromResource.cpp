/*
 * GWindowFromResource.cpp
 *
 *  Created on: 01/07/2013
 *      Author: Guillermo Paris
 */

#include "GWindowTemplate.h"
#include "GWindowFromResource.h"
#include "Services/Resource/GResourcesService.h"


GWindowFromResource::GWindowFromResource(UINT8 u8ResID)
  :	m_blCreated(false),
  	m_u8ResourceId(u8ResID)
{
	if(u8ResID == GResourcesService::GetService()->GetResourceIdForKeyMatrix())
	{
		m_u8WindowId = eWinId_KeypadMatrix;
	}
	else 	if(u8ResID == GResourcesService::GetService()->GetResourceIdForSoftKeys())
	{
		m_u8WindowId = eWinId_SoftKeyMatrix;
	}
	else 	if(u8ResID == GResourcesService::GetService()->GetResourceIdForAuxKeys())
	{
		m_u8WindowId = eWinId_AuxKeyMatrix;
	}
	else
	{
		m_u8WindowId = 0;
		m_u8ResourceId = 0; // unknown resource ID mark

	}

}

GWindowFromResource::~GWindowFromResource()
{
	if(m_blCreated)
	{
		DestroyWindow();
	}
}

bool GWindowFromResource::PerformWindowAction(eWindowAction eAction)
{
	if(m_u8WindowId == 0) // bad resource ID at creation time
		return false;

	GDisplayService* pDYService = GDisplayService::GetService();
	if( pDYService == NULL )
		return false;

	bool blret = false;

	switch(eAction)
	{
		case eCreate:
			blret = pDYService->CreateWindowFromResource(m_u8WindowId, m_u8ResourceId);
			m_blCreated = blret;
			break;

		case eDestroy:
			blret = pDYService->DestroyWindow(m_u8WindowId);
			if(blret)
				m_blCreated = false;
			break;

		case eShow:
			blret = pDYService->ShowWindow(m_u8WindowId);
			break;

		case eHide:
			blret = pDYService->HideWindow(m_u8WindowId);
			break;

		default:
			break;
	}

	return blret;
}

bool GWindowFromResource::ShowWindowTags(UINT uQty, GDisplayService::SHOW_TAG* pTags)
{
	if(m_u8WindowId == 0 || uQty == 0 || pTags == NULL)
		return false;

	GDisplayService* pDYService = GDisplayService::GetService();
	if( pDYService == NULL )
		return false;

	return pDYService->ShowWindowTagArray(m_u8WindowId, UINT8(uQty), pTags);
}

bool GWindowFromResource::HideWindowTags(UINT uQty, UINT8* pu8TagIDs)
{
	if(m_u8WindowId == 0 || uQty == 0 || pu8TagIDs == NULL)
		return false;

	GDisplayService* pDYService = GDisplayService::GetService();
	if( pDYService == NULL )
		return false;

	return pDYService->HideWindowTagArray(m_u8WindowId, UINT8(uQty), pu8TagIDs);
}


