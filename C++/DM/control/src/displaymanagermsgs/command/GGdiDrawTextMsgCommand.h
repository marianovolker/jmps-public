//! GGdiDrawText Message Command 
/*!
  Gdi Draw Text Message Command Class
*/

#ifndef GGDI_DRAW_TEXT_MSGCOMMAND_H
#define GGDI_DRAW_TEXT_MSGCOMMAND_H

#include "control/src/displaymanagermsgs/base/GAbsMsgCommand.h"

class GGdiDrawTextMsgCommand : public GAbsMsgCommand
{

public:
	GGdiDrawTextMsgCommand(GAbsData * pData = NULL);
	virtual ~GGdiDrawTextMsgCommand();

	void Execute() throw(GException);
};
#endif

