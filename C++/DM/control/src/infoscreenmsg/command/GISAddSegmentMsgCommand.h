//! GISAddSegmentMsgCommand 
/*!
  Implementation of the Class GISAddSegmentMsgCommand
*/

#if !defined(GISAddSegmentMsgCommand_h)
#define GISAddSegmentMsgCommand_h

#include "control/src/infoscreenmsg/base/GISAbsCommand.h"

class GISAddSegmentMsgCommand : public GISAbsCommand
{
public:
	GISAddSegmentMsgCommand();
	virtual ~GISAddSegmentMsgCommand();

	void DoExecute() throw (GException);
};
#endif
