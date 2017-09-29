#ifndef CLOCK_MSG_COMMAND_H
#define CLOCK_MSG_COMMAND_H

#include "control/src/displaymanagermsgs/base/GAbsMsgCommand.h"

class GClockMsgCommand : public GAbsMsgCommand
{
public:
	GClockMsgCommand(GAbsData *pData = NULL);
	virtual ~GClockMsgCommand();

	void Execute() throw(GException);
};

#endif
