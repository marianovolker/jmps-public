//! GISSegmentDisplayCountMsgCommand 
/*!
  Implementation of the Class GISSegmentDisplayCountMsgCommand
*/

#if !defined(GISSegmentDisplayCountMsgCommand_h)
#define GISSegmentDisplayCountMsgCommand_h

#include "control/src/infoscreenmsg/base/GISAbsCommand.h"

class GISSegmentDisplayCountMsgCommand : public GISAbsCommand
{
public:

	GISSegmentDisplayCountMsgCommand();
	virtual ~GISSegmentDisplayCountMsgCommand();

	void DoExecute() throw (GException);
};
#endif
