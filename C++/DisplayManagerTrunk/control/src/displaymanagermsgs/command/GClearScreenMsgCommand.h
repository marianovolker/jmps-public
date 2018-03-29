//! GClearScreen Message Command 
/*!
  Clear Screen Message Command Class
*/

#ifndef GCLEAR_SCREEN_MSGCOMMAND_H
#define GCLEAR_SCREEN_MSGCOMMAND_H

#include "control/src/displaymanagermsgs/base/GAbsMsgCommand.h"

class GClearScreenMsgCommand : public GAbsMsgCommand
{

public:
	GClearScreenMsgCommand(GAbsData * pData = NULL);
	virtual ~GClearScreenMsgCommand();

	void Execute() throw(GException);
};
#endif
