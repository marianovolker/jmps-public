//! GISVideoRangePlayMsgCommand
/*!
  Implementation of the Class GISVideoRangePlayMsgCommand
*/

#if !defined(GISVideoRangePlayMsgCommand_h)
#define GISVideoRangePlayMsgCommand_h

#include "control/src/infoscreenmsg/base/GISAbsCommand.h"

class GISVideoRangePlayMsgCommand : public GISAbsCommand
{
public:

	GISVideoRangePlayMsgCommand();
	virtual ~GISVideoRangePlayMsgCommand();

	void DoExecute() throw (GException);
};
#endif
