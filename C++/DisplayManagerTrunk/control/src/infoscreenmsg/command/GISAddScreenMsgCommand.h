//! GISAddScreenMsgCommand 
/*!
  Implementation of the Class GISAddScreenMsgCommand
*/

#if !defined(GISAddScreenMsgCommand_h)
#define GISAddScreenMsgCommand_h

#include "control/src/infoscreenmsg/base/GISAbsCommand.h"

class GISAddScreenMsgCommand : public GISAbsCommand
{
public:
	GISAddScreenMsgCommand();
	virtual ~GISAddScreenMsgCommand();

	void DoExecute() throw (GException);
};
#endif
