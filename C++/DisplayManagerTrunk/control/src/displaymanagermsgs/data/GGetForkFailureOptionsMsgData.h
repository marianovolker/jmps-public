//! GGetForkFailureOptionsMsgData
/*!
  Get ForkFailureOptions Message Data
 */

#ifndef GGetForkFailureOptionsMsgData_H
#define GGetForkFailureOptionsMsgData_H

#include "control/src/displaymanagermsgs/base/GAbsMsgData.h"
#include <QString>

class GGetForkFailureOptionsMsgData : public GAbsMsgData {
public:
    GGetForkFailureOptionsMsgData();
    virtual ~GGetForkFailureOptionsMsgData();

private:
    // Define a private copy constructor and assignment operator to prevent their use
    GGetForkFailureOptionsMsgData( const GGetForkFailureOptionsMsgData& gGetForkFailureOptionsMsgData );
    GGetForkFailureOptionsMsgData& operator=( const GGetForkFailureOptionsMsgData& gGetForkFailureOptionsMsgData );

};

#endif
