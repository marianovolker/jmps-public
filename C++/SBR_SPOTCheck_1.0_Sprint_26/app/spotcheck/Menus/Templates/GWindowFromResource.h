/**
 * © 2013 Gilbarco Inc.
 * Confidential and Proprietary
 *
 * @file GWindowFromResource.h
 * @author Guillermo Paris
 * @date Jul 01 2013
 * @copyright © 2013 Gilbarco Inc. Confidential and Proprietary
 */


#ifndef GWINDOWFROMRESOURCE_H_
#define GWINDOWFROMRESOURCE_H_

#include <GTypes.h>
#include "Services/Display/GDisplayService.h"

// Avoid silly collision with Windows macros
#undef CreateWindow
#undef DestroyWindow

class GWindowFromResource
{
public:
	GWindowFromResource(UINT8 u8ResID);
	virtual ~GWindowFromResource();

	inline bool CreateWindow();
	inline bool DestroyWindow();
	inline bool ShowWindow();
	inline bool HideWindow();
	       bool ShowWindowTags(UINT uQty, GDisplayService::SHOW_TAG* pTags);
	       bool HideWindowTags(UINT uQty, UINT8* pu8TagIDs);


protected:

	enum eWindowAction
	{
		eNone, eCreate, eShow, eHide, eDestroy
	};

	UINT8       m_u8WindowId;
	UINT8       m_u8ResourceId;


private:

	bool        PerformWindowAction(eWindowAction);

	bool        m_blCreated;
};


inline bool GWindowFromResource::CreateWindow()
{
	return PerformWindowAction(eCreate);
}

inline bool GWindowFromResource::DestroyWindow()
{
	return PerformWindowAction(eDestroy);
}

inline bool GWindowFromResource::ShowWindow()
{
	return PerformWindowAction(eShow);
}

inline bool GWindowFromResource::HideWindow()
{
	return PerformWindowAction(eHide);
}


#endif /* GWINDOWFROMRESOURCE_H_ */
