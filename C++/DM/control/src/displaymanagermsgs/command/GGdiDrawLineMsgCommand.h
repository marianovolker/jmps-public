//! GGdiDrawLine Message Command 
/*!
  Gdi Draw Line Message Command Class
*/

#ifndef GGDI_DRAW_LINE_MSGCOMMAND_H
#define GGDI_DRAW_LINE_MSGCOMMAND_H

#include "control/src/displaymanagermsgs/base/GAbsMsgCommand.h"

class GGdiDrawLineMsgCommand : public GAbsMsgCommand
{

public:
	GGdiDrawLineMsgCommand(GAbsData * pData = NULL);
	virtual ~GGdiDrawLineMsgCommand();

	void Execute() throw(GException);
};
#endif
