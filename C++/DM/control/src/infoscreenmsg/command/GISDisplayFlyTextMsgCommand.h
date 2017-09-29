//! GISDisplayFlyTextMsgCommand 
/*!
  Implementation of the Class GISDisplayFlyTextMsgCommand
*/

#if !defined(GISDisplayFlyTextMsgCommand_h)
#define GISDisplayFlyTextMsgCommand_h

#include "control/src/infoscreenmsg/base/GISAbsCommand.h"

class GISDisplayFlyTextMsgCommand : public GISAbsCommand
{
public:
	GISDisplayFlyTextMsgCommand();
	virtual ~GISDisplayFlyTextMsgCommand();

	void DoExecute() throw (GException);
};
#endif
