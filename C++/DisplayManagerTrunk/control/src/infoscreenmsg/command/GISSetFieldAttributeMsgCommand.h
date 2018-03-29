//! GISSetFieldAttributeMsgCommand 
/*!
  Implementation of the Class GISSetFieldAttributeMsgCommand
*/

#if !defined(GISSetFieldAttributeMsgCommand_h)
#define GISSetFieldAttributeMsgCommand_h

#include "control/src/infoscreenmsg/base/GISAbsCommand.h"

class GISSetFieldAttributeMsgCommand : public GISAbsCommand
{
public:

	GISSetFieldAttributeMsgCommand();
	virtual ~GISSetFieldAttributeMsgCommand();

	void DoExecute() throw (GException);
};
#endif
