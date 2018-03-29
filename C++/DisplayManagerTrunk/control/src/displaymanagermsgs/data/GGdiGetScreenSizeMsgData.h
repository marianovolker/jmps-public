//! GGdiGetScreenSizeMsgData 
/*!
  Gdi GetScreenSize Message Data
*/

#ifndef GGDI_GET_SCREEN_SIZE_MSG_DATA_H
#define GGDI_GET_SCREEN_SIZE_MSG_DATA_H

#include "control/src/displaymanagermsgs/base/GAbsMsgData.h"

class GGdiGetScreenSizeMsgData : public GAbsMsgData
{
public:
	GGdiGetScreenSizeMsgData();
	virtual ~GGdiGetScreenSizeMsgData();
};

#endif
