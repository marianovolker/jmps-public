//! GKeyEventMsgData 
/*!
  Key Event Message Data
*/

#ifndef GKEY_EVENT_MSG_DATA_H
#define GKEY_EVENT_MSG_DATA_H

#include "control/src/displaymanagermsgs/base/GAbsMsgData.h"

class GKeyEventMsgData : public GAbsMsgData
{
public:
	GKeyEventMsgData();
	virtual ~GKeyEventMsgData();

	QString GetKeyCode();
	void SetKeyCode(QString strKeyCode);

private:
	QString m_strKeyCode;
};

#endif
