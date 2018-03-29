//! GGdiRefreshScreen Message Command 
/*!
  Gdi Refresh Screen Message Command Class
*/

#ifndef GGDI_REFRESH_SCREEN_MSGCOMMAND_H
#define GGDI_REFRESH_SCREEN_MSGCOMMAND_H

#include "control/src/displaymanagermsgs/base/GAbsMsgCommand.h"

class GGdiRefreshScreenMsgCommand : public GAbsMsgCommand
{

public:
	GGdiRefreshScreenMsgCommand(GAbsData * pData = NULL);
	virtual ~GGdiRefreshScreenMsgCommand();

	void Execute() throw(GException);
};
#endif
