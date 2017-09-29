//! GISDeleteSegmentMsgCommand 
/*!
  Implementation of the Class GISDeleteSegmentMsgCommand
*/

#if !defined(GISDeleteSegmentMsgCommand_h)
#define GISDeleteSegmentMsgCommand_h

#include "control/src/infoscreenmsg/base/GISAbsCommand.h"

class GISDeleteSegmentMsgCommand : public GISAbsCommand
{
public:
	GISDeleteSegmentMsgCommand();
	virtual ~GISDeleteSegmentMsgCommand();

	void DoExecute() throw (GException);
};
#endif
