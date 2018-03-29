/**
 * © 2013 Gilbarco Inc.
 * Confidential and Proprietary
 *
 * @file GKeyMatrixWindowFromResource.h
 * @author Guillermo Paris
 * @date Jul 01 2013
 * @copyright © 2013 Gilbarco Inc. Confidential and Proprietary
 */

#ifndef GKEYMATRIXWINDOWFROMRESOURCE_H_
#define GKEYMATRIXWINDOWFROMRESOURCE_H_

#include "GWindowFromResource.h"

#define RESOURCE_WIN_TAG_TEST_TITLE    37
#define RESOURCE_WIN_TAG_STATS_TITLE   38
#define RESOURCE_WIN_TAG_STATS_EXIT    40
#define RESOURCE_WIN_TAG_ACTIVATION    40


class GKeyMatrixWindowFromResource : public GWindowFromResource
{
public:
	GKeyMatrixWindowFromResource(UINT8 u8ResID);
	virtual ~GKeyMatrixWindowFromResource();

	bool    MarkKeyPressed(UINT8 u8KeyCode);
	bool    MarkForStatistics(UINT uQty, UINT8* pu8KeyCodes,
	                          bool blTitleSwap = false,
	                          bool blExitTagSwap = false);

private:
	const static UINT8 mst_Cu8TestTitle_TagID;  // = RESOURCE_WIN_TAG_TEST_TITLE
	const static UINT8 mst_Cu8StatsTitle_TagID; // = RESOURCE_WIN_TAG_STATS_TITLE
	const static UINT8 mst_Cu8StatsExit_TagID;  // = RESOURCE_WIN_TAG_STATS_EXIT
	const static UINT8 mst_Cu8WinTag_Activation_Offset; // = RESOURCE_WIN_TAG_ACTIVATION
};

#endif /* GKEYMATRIXWINDOWFROMRESOURCE_H_ */
