//! GSetMediaManagerMsgCommand
/*!
  Set Media Manager Message Command Class
*/

#ifndef GSetMediaManagerMsgCommand_H
#define GSetMediaManagerMsgCommand_H

#include "control/src/displaymanagermsgs/base/GAbsMsgCommand.h"

class GSetMediaManagerMsgCommand : public GAbsMsgCommand
{

public:
	GSetMediaManagerMsgCommand(GAbsData * pData = NULL);
	virtual ~GSetMediaManagerMsgCommand();

	void Execute() throw(GException);

private:
    // Define a private copy constructor and assignment operator to prevent their use
    GSetMediaManagerMsgCommand( const GSetMediaManagerMsgCommand& gSetMediaManagerMsgCommand );
    GSetMediaManagerMsgCommand& operator=( const GSetMediaManagerMsgCommand& gSetMediaManagerMsgCommand );
};
#endif
