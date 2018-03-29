//! GGetForkFailureOptions Message Command
/*!
  Set ForkFailureOptions Message Command Class
 */

#ifndef GGetForkFailureOptionsMsgCommand_H
#define GGetForkFailureOptionsMsgCommand_H

#include "control/src/displaymanagermsgs/base/GAbsMsgCommand.h"

class GGetForkFailureOptionsMsgCommand : public GAbsMsgCommand {
public:
    GGetForkFailureOptionsMsgCommand(GAbsData * pData = NULL);
    virtual ~GGetForkFailureOptionsMsgCommand();

    void Execute() throw (GException);

private:
    // Define a private copy constructor and assignment operator to prevent their use
    GGetForkFailureOptionsMsgCommand( const GGetForkFailureOptionsMsgCommand& gGetForkFailureOptionsMsgCommand );
    GGetForkFailureOptionsMsgCommand& operator=( const GGetForkFailureOptionsMsgCommand& gGetForkFailureOptionsMsgCommand );
};
#endif
