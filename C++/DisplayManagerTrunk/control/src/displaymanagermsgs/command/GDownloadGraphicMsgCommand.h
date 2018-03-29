//! GDownloadGraphic Message Command 
/*!
  Download Graphic Message Command Class
*/

#ifndef GDOWNLOAD_GRAPHIC_MSGCOMMAND_H
#define GDOWNLOAD_GRAPHIC_MSGCOMMAND_H

#include "control/src/displaymanagermsgs/base/GAbsMsgCommand.h"

class GDownloadGraphicMsgCommand : public GAbsMsgCommand
{

public:
	GDownloadGraphicMsgCommand(GAbsData * pData = NULL);
	virtual ~GDownloadGraphicMsgCommand();

	void Execute() throw(GException);
};
#endif
