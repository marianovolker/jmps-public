//! GURLMsgCommand
/*!
  URL Message Command Class
*/

#ifndef GURLMsgCommand_H_
#define GURLMsgCommand_H_

#include "control/src/displaymanagermsgs/base/GAbsMsgCommand.h"

class GURLMsgCommand : public GAbsMsgCommand
{

public:
	GURLMsgCommand(GAbsData * pData = NULL);
	virtual ~GURLMsgCommand();

	void Execute() throw(GException);
};
#endif
