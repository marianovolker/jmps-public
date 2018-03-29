//! GInfoScreen Message Command 
/*!
  InfoScreen Message Command Class
*/

#ifndef GINFOSCREEN_MSGCOMMAND_H
#define GINFOSCREEN_MSGCOMMAND_H

#include "control/src/displaymanagermsgs/base/GAbsMsgCommand.h"
#include "control/src/infoscreenmsg/factory/GISMsgCmdDataFactory.h"

class GInfoScreenMsgCommand : public GAbsMsgCommand
{

public:
	GInfoScreenMsgCommand(GAbsData * pData = NULL);
	virtual ~GInfoScreenMsgCommand();

	void Execute() throw(GException);
	
private:
	GISMsgCmdDataFactory* m_poISMsgCmdDataFactory;
};
#endif
