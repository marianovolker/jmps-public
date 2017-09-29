//! GISVideoPlayMsgCommand 
/*!
  Implementation of the Class GISVideoPlayMsgCommand
*/

#if !defined(GISVideoPlayMsgCommand_h)
#define GISVideoPlayMsgCommand_h

#include "control/src/infoscreenmsg/base/GISAbsCommand.h"

class GISVideoPlayMsgCommand : public GISAbsCommand
{
public:

	GISVideoPlayMsgCommand();
	virtual ~GISVideoPlayMsgCommand();

	void DoExecute() throw (GException);
};
#endif
