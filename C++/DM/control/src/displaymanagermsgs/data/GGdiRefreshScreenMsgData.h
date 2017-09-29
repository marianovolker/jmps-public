//! GGdiRefreshScreenMsgData 
/*!
  Gdi RefreshScreen Message Data
*/

#ifndef GGDI_REFRESH_SCREEN_MSG_DATA_H
#define GGDI_REFRESH_SCREEN_MSG_DATA_H

#include "control/src/displaymanagermsgs/base/GAbsMsgData.h"

class GGdiRefreshScreenMsgData : public GAbsMsgData
{
public:
	GGdiRefreshScreenMsgData();
	virtual ~GGdiRefreshScreenMsgData();
};

#endif
