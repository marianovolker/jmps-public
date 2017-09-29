//! GISSetGraphicsTypeMsgCommand 
/*!
  Implementation of the Class GISSetGraphicsTypeMsgCommand
*/

#if !defined(GISSetGraphicsTypeMsgCommand_h)
#define GISSetGraphicsTypeMsgCommand_h

#include "control/src/infoscreenmsg/base/GISAbsCommand.h"

class GISSetGraphicsTypeMsgCommand : public GISAbsCommand
{
public:

	GISSetGraphicsTypeMsgCommand();
	virtual ~GISSetGraphicsTypeMsgCommand();

	void DoExecute() throw (GException);
};
#endif
