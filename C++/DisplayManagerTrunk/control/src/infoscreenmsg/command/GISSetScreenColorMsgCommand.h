//! GISSetScreenColorMsgCommand 
/*!
  Implementation of the Class GISSetScreenColorMsgCommand
*/

#if !defined(GISSetScreenColorMsgCommand_h)
#define GISSetScreenColorMsgCommand_h

#include "control/src/infoscreenmsg/base/GISAbsCommand.h"

class GISSetScreenColorMsgCommand : public GISAbsCommand
{
public:

	GISSetScreenColorMsgCommand();
	virtual ~GISSetScreenColorMsgCommand();

	void DoExecute() throw (GException);
};
#endif
