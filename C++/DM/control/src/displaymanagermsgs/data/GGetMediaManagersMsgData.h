//! GGetMediaManagersMsgData
/*!
  Set MediaManagers Message Data
 */

#ifndef GGetMediaManagersMsgData_H
#define GGetMediaManagersMsgData_H

#include "control/src/displaymanagermsgs/base/GAbsMsgData.h"
#include <QString>

class GGetMediaManagersMsgData : public GAbsMsgData {
public:
    GGetMediaManagersMsgData();
    virtual ~GGetMediaManagersMsgData();

private:
    // Define a private copy constructor and assignment operator to prevent their use
    GGetMediaManagersMsgData( const GGetMediaManagersMsgData& gGetMediaManagersMsgData );
    GGetMediaManagersMsgData& operator=( const GGetMediaManagersMsgData& gGetMediaManagersMsgData );

};

#endif
