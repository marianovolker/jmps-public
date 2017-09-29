//! GInfoScreenMsgData 
/*!
  InfoScreen Message Data
*/

#ifndef GINFOSCREEN_MSG_DATA_H
#define GINFOSCREEN_MSG_DATA_H

#include "control/src/displaymanagermsgs/base/GAbsMsgData.h"
#include <QByteArray>

class GInfoScreenMsgData : public GAbsMsgData
{
public:
	GInfoScreenMsgData();
	virtual ~GInfoScreenMsgData();

	QByteArray GetData();
	void SetData(QByteArray oData);

private:
	QByteArray m_oData;
};

#endif
