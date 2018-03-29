//! GISClearScreenMsgCommand 
/*!
  Implementation of the Class GISClearScreenMsgCommand
*/

#if !defined(GISClearScreenMsgCommand_h)
#define GISClearScreenMsgCommand_h

#include "control/src/infoscreenmsg/base/GISAbsCommand.h"

class GISClearScreenMsgCommand : public GISAbsCommand
{
public:
	GISClearScreenMsgCommand();
	virtual ~GISClearScreenMsgCommand();

	void DoExecute() throw (GException);
};
#endif
