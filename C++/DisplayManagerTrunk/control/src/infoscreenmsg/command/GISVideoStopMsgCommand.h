//! GISVideoStopMsgCommand 
/*!
  Implementation of the Class GISVideoStopMsgCommand
*/

#if !defined(GISVideoStopMsgCommand_h)
#define GISVideoStopMsgCommand_h

#include "control/src/infoscreenmsg/base/GISAbsCommand.h"

class GISVideoStopMsgCommand : public GISAbsCommand
{
public:

	GISVideoStopMsgCommand();
	virtual ~GISVideoStopMsgCommand();

	void DoExecute() throw (GException);
};
#endif
