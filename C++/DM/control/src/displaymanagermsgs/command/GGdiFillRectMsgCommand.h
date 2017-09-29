//! GGdiFillRect Message Command 
/*!
  Gdi Fill Rect Message Command Class
*/

#ifndef GGDI_FILL_RECT_MSGCOMMAND_H
#define GGDI_FILL_RECT_MSGCOMMAND_H

#include "control/src/displaymanagermsgs/base/GAbsMsgCommand.h"

class GGdiFillRectMsgCommand : public GAbsMsgCommand
{

public:
	GGdiFillRectMsgCommand(GAbsData * pData = NULL);
	virtual ~GGdiFillRectMsgCommand();

	void Execute() throw(GException);
};
#endif
