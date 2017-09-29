//! GGdiClearScreenMsgData 
/*!
  Gdi ClearScreen Message Data
*/

#ifndef GGDI_CLEAR_SCREEN_MSG_DATA_H
#define GGDI_CLEAR_SCREEN_MSG_DATA_H

#include "control/src/displaymanagermsgs/base/GAbsMsgData.h"

class GGdiClearScreenMsgData : public GAbsMsgData
{
public:
	GGdiClearScreenMsgData();
	virtual ~GGdiClearScreenMsgData();
};

#endif
