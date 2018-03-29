//! GGdiClearScreen Message Command 
/*!
  Gdi Clear Screen Message Command Class
*/

#ifndef GGDI_CLEAR_SCREEN_MSGCOMMAND_H
#define GGDI_CLEAR_SCREEN_MSGCOMMAND_H

#include "control/src/displaymanagermsgs/base/GAbsMsgCommand.h"

class GGdiClearScreenMsgCommand : public GAbsMsgCommand
{

public:
	GGdiClearScreenMsgCommand(GAbsData * pData = NULL);
	virtual ~GGdiClearScreenMsgCommand();

	void Execute() throw(GException);
};
#endif
