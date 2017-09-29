//! GSetMediaManagerMsgData
/*!
  Set Media Manager Message Data
*/

#ifndef GSetMediaManagerMsgData_H
#define GSetMediaManagerMsgData_H

#include "control/src/displaymanagermsgs/base/GAbsMsgData.h"
#include <QString>

class GSetMediaManagerMsgData : public GAbsMsgData
{
public:
	GSetMediaManagerMsgData();
	virtual ~GSetMediaManagerMsgData();

	QString GetType();
	void SetType(QString strMediaManagerType);

private:
	QString m_strMediaManagerType;
        // Define a private copy constructor and assignment operator to prevent their use
        GSetMediaManagerMsgData( const GSetMediaManagerMsgData& gSetMediaManagerMsgData );
        GSetMediaManagerMsgData& operator=( const GSetMediaManagerMsgData& gSetMediaManagerMsgData );
};

#endif
