//! GSetDefaultPersonalityMsgCommand
/*!
  Set Personality Message Command Class
*/

#ifndef GSetDefaultPersonalityMsgCommand_H
#define GSetDefaultPersonalityMsgCommand_H

#include "control/src/displaymanagermsgs/base/GAbsMsgCommand.h"

class GSetDefaultPersonalityMsgCommand : public GAbsMsgCommand
{

public:
	GSetDefaultPersonalityMsgCommand(GAbsData * pData = NULL);
	virtual ~GSetDefaultPersonalityMsgCommand();

	void Execute() throw(GException);	
};
#endif
