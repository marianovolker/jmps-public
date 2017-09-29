//! GKioskId Message Command 
/*!
  Kiosk ID Message Command Class
*/

#ifndef GKIOSK_ID_MSGCOMMAND_H
#define GKIOSK_ID_MSGCOMMAND_H

#include "control/src/displaymanagermsgs/base/GAbsMsgCommand.h"

class GKioskIdMsgCommand : public GAbsMsgCommand
{

public:
	GKioskIdMsgCommand(GAbsData * pData = NULL);
	virtual ~GKioskIdMsgCommand();

	void Execute() throw(GException);
};
#endif
