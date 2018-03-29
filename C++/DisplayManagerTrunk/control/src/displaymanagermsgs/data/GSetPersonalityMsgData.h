//! GSetPersonalityMsgData 
/*!
  Set Personality Message Data
*/

#ifndef GSET_PERSONALITY_MSG_DATA_H
#define GSET_PERSONALITY_MSG_DATA_H

#include "control/src/displaymanagermsgs/base/GAbsMsgData.h"
#include <QString>

class GSetPersonalityMsgData : public GAbsMsgData
{
public:
	GSetPersonalityMsgData();
	virtual ~GSetPersonalityMsgData();

	QString GetPersonality();
	void SetPersonality(QString strPersonality);

private:
	QString m_strPersonality;
};

#endif
