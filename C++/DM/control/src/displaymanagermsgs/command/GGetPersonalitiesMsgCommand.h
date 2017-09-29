//! GGetPersonalities Message Command
/*!
  Set Personality Message Command Class
*/

#ifndef GGetPersonalitiesMsgCommand_H
#define GGetPersonalitiesMsgCommand_H

#include "control/src/displaymanagermsgs/base/GAbsMsgCommand.h"

class GGetPersonalitiesMsgCommand : public GAbsMsgCommand
{

public:
	GGetPersonalitiesMsgCommand(GAbsData * pData = NULL);
	virtual ~GGetPersonalitiesMsgCommand();

	void Execute() throw(GException);	
};
#endif
