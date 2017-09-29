//! GSetPersonality Message Command 
/*!
  Set Personality Message Command Class
*/

#ifndef GSetPersonalityMsgCommand_H
#define GSetPersonalityMsgCommand_H

#include "control/src/displaymanagermsgs/base/GAbsMsgCommand.h"

class GSetPersonalityMsgCommand : public GAbsMsgCommand
{

public:
	GSetPersonalityMsgCommand(GAbsData * pData = NULL);
	virtual ~GSetPersonalityMsgCommand();

	void Execute() throw(GException);	
};
#endif
