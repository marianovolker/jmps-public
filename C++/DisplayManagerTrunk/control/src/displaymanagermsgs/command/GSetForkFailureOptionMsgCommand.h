//! GSetForkFailureOptionMsgCommand
/*!
  Set ForkFailureOption Message Command Class
*/

#ifndef GSetForkFailureOptionMsgCommand_H
#define GSetForkFailureOptionMsgCommand_H

#include "control/src/displaymanagermsgs/base/GAbsMsgCommand.h"

class GSetForkFailureOptionMsgCommand : public GAbsMsgCommand
{

public:
	GSetForkFailureOptionMsgCommand(GAbsData * pData = NULL);
	virtual ~GSetForkFailureOptionMsgCommand();

	void Execute() throw(GException);

private:
    // Define a private copy constructor and assignment operator to prevent their use
    GSetForkFailureOptionMsgCommand( const GSetForkFailureOptionMsgCommand& gSetForkFailureOptionMsgCommand );
    GSetForkFailureOptionMsgCommand& operator=( const GSetForkFailureOptionMsgCommand& gSetForkFailureOptionMsgCommand );
};
#endif
