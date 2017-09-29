//! GSetDefaultPersonalityMsgData
/*!
  Set Personality Message Data
*/

#ifndef GSetDefaultPersonalityMsgData_H
#define GSetDefaultPersonalityMsgData_H

#include "control/src/displaymanagermsgs/base/GAbsMsgData.h"
#include <QString>

class GSetDefaultPersonalityMsgData : public GAbsMsgData
{
public:
	GSetDefaultPersonalityMsgData();
	virtual ~GSetDefaultPersonalityMsgData();

	QString GetPersonality();
	void SetPersonality(QString strPersonality);

private:
	QString m_strPersonality;
};

#endif
