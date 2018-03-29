//! GISDisplaySegmentMsgCommand 
/*!
  Implementation of the Class GISDisplaySegmentMsgCommand
*/

#if !defined(GISDisplaySegmentMsgCommand_h)
#define GISDisplaySegmentMsgCommand_h

#include "control/src/infoscreenmsg/base/GISAbsCommand.h"

class GISDisplaySegmentMsgCommand : public GISAbsCommand
{
public:
	GISDisplaySegmentMsgCommand();
	virtual ~GISDisplaySegmentMsgCommand();

	void DoExecute() throw (GException);
};
#endif
