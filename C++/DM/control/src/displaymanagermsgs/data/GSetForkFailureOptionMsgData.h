//! GSetForkFailureOptionMsgData
/*!
  Set ForkFailureOption Message Data
*/

#ifndef GSetForkFailureOptionMsgData_H
#define GSetForkFailureOptionMsgData_H

#include "control/src/displaymanagermsgs/base/GAbsMsgData.h"
#include <QString>

class GSetForkFailureOptionMsgData : public GAbsMsgData
{
public:
	GSetForkFailureOptionMsgData();
	virtual ~GSetForkFailureOptionMsgData();

	QString GetType();
	void SetType(QString strForkFailureOption);

private:
	QString m_strForkFailureOption;
        // Define a private copy constructor and assignment operator to prevent their use
        GSetForkFailureOptionMsgData( const GSetForkFailureOptionMsgData& gSetForkFailureOptionMsgData );
        GSetForkFailureOptionMsgData& operator=( const GSetForkFailureOptionMsgData& gSetForkFailureOptionMsgData );
};

#endif
