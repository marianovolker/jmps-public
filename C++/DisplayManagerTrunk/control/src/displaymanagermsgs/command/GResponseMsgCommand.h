//! GResponse Message Command 
/*!
  Response Message Command Class
*/

#ifndef GRESPONSE_MSGCOMMAND_H
#define GRESPONSE_MSGCOMMAND_H

#include "control/src/displaymanagermsgs/base/GAbsMsgCommand.h"

class GResponseMsgCommand : public GAbsMsgCommand
{

public:
	GResponseMsgCommand(GAbsData * pData = NULL);
	virtual ~GResponseMsgCommand();

	void Execute() throw(GException);
};
#endif
