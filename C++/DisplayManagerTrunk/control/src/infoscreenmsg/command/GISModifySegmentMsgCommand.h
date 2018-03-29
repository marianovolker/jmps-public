//! GISModifySegmentMsgCommand 
/*!
  Implementation of the Class GISModifySegmentMsgCommand
*/

#if !defined(GISModifySegmentMsgCommand_h)
#define GISModifySegmentMsgCommand_h

#include "control/src/infoscreenmsg/base/GISAbsCommand.h"

class GISModifySegmentMsgCommand : public GISAbsCommand
{
public:

	GISModifySegmentMsgCommand();
	virtual ~GISModifySegmentMsgCommand();

	void DoExecute() throw (GException);
};
#endif
