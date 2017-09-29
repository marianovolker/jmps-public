//! GISVideoViewMsgCommand 
/*!
  Implementation of the Class GISVideoViewMsgCommand
*/

#if !defined(GISVideoViewMsgCommand_h)
#define GISVideoViewMsgCommand_h

#include "control/src/infoscreenmsg/base/GISAbsCommand.h"

class GISVideoViewMsgCommand : public GISAbsCommand
{
public:

	GISVideoViewMsgCommand();
	virtual ~GISVideoViewMsgCommand();

	void DoExecute() throw (GException);
};
#endif
