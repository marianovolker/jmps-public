//! GGetMediaManagers Message Command
/*!
  Set Media Managers Message Command Class
 */

#ifndef GGetMediaManagersMsgCommand_H
#define GGetMediaManagersMsgCommand_H

#include "control/src/displaymanagermsgs/base/GAbsMsgCommand.h"

class GGetMediaManagersMsgCommand : public GAbsMsgCommand {
public:
    GGetMediaManagersMsgCommand(GAbsData * pData = NULL);
    virtual ~GGetMediaManagersMsgCommand();

    void Execute() throw (GException);

private:
    // Define a private copy constructor and assignment operator to prevent their use
    GGetMediaManagersMsgCommand( const GGetMediaManagersMsgCommand& gGetMediaManagersMsgCommand );
    GGetMediaManagersMsgCommand& operator=( const GGetMediaManagersMsgCommand& gGetMediaManagersMsgCommand );
};
#endif
