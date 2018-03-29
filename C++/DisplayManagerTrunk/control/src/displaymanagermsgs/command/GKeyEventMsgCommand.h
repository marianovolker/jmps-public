//! GKey Message Command 
/*!
  Key Message Command Class
*/

#ifndef GKEY_MSGCOMMAND_H
#define GKEY_MSGCOMMAND_H

#include "control/src/displaymanagermsgs/base/GAbsMsgCommand.h"

class GKeyEventMsgCommand : public GAbsMsgCommand
{

public:
	GKeyEventMsgCommand(GAbsData * pData = NULL);
	virtual ~GKeyEventMsgCommand();

	void Execute() throw(GException);
};
#endif
