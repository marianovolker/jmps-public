//! GGdiGetScreenSize Message Command 
/*!
  Gdi Get Screen Size Message Command Class
*/

#ifndef GGDI_GET_SCREEN_SIZE_MSGCOMMAND_H
#define GGDI_GET_SCREEN_SIZE_MSGCOMMAND_H

#include "control/src/displaymanagermsgs/base/GAbsMsgCommand.h"

class GGdiGetScreenSizeMsgCommand : public GAbsMsgCommand
{

public:
	GGdiGetScreenSizeMsgCommand(GAbsData * pData = NULL);
	virtual ~GGdiGetScreenSizeMsgCommand();

	void Execute() throw(GException);
};
#endif
