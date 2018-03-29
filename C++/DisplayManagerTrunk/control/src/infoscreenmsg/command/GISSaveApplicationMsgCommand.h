//! GISSaveApplicationMsgCommand 
/*!
  Implementation of the Class GISSaveApplicationMsgCommand
*/

#if !defined(GISSaveApplicationMsgCommand_h)
#define GISSaveApplicationMsgCommand_h

#include "control/src/infoscreenmsg/base/GISAbsCommand.h"

class GISSaveApplicationMsgCommand : public GISAbsCommand
{
public:

	GISSaveApplicationMsgCommand();
	virtual ~GISSaveApplicationMsgCommand();

	void DoExecute() throw (GException);
};
#endif
