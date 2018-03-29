//! GClearScreenMsgData 
/*!
  Clear Screen Message Data
*/

#ifndef GCLEAR_SCREEN_MSG_DATA_H
#define GCLEAR_SCREEN_MSG_DATA_H

#include "control/src/displaymanagermsgs/base/GAbsMsgData.h"
#include <QString>

class GClearScreenMsgData : public GAbsMsgData
{
public:
	GClearScreenMsgData();
	virtual ~GClearScreenMsgData();

	QString GetColor();
	void SetColor(QString strColor);

private:
	QString m_strColor;
};

#endif
