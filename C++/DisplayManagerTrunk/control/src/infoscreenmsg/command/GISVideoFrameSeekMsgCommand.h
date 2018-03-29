//! GISVideoFrameSeekMsgCommand 
/*!
  Implementation of the Class GISVideoFrameSeekMsgCommand
*/

#if !defined(GISVideoFrameSeekMsgCommand_h)
#define GISVideoFrameSeekMsgCommand_h

#include "control/src/infoscreenmsg/base/GISAbsCommand.h"

class GISVideoFrameSeekMsgCommand : public GISAbsCommand
{
public:

	GISVideoFrameSeekMsgCommand();
	virtual ~GISVideoFrameSeekMsgCommand();

	void DoExecute() throw (GException);
};
#endif
