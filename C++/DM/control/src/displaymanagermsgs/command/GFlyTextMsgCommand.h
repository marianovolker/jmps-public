//! GFlyText Message Command 
/*!
  Fly Text Message Command Class
*/

#ifndef GFLY_TEXT_MSGCOMMAND_H
#define GFLY_TEXT_MSGCOMMAND_H

#include "control/src/displaymanagermsgs/base/GAbsMsgCommand.h"

class GFlyTextMsgCommand : public GAbsMsgCommand
{

public:
	GFlyTextMsgCommand(GAbsData * pData = NULL);
	virtual ~GFlyTextMsgCommand();

	void Execute() throw(GException);
};
#endif
