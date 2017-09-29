//! GISDeleteScreenMsgCommand 
/*!
  Implementation of the Class GISDeleteScreenMsgCommand
*/

#if !defined(GISDeleteScreenMsgCommand_h)
#define GISDeleteScreenMsgCommand_h

#include "control/src/infoscreenmsg/base/GISAbsCommand.h"

class GISDeleteScreenMsgCommand : public GISAbsCommand
{
public:
	GISDeleteScreenMsgCommand();
	virtual ~GISDeleteScreenMsgCommand();

	void DoExecute() throw (GException);
};
#endif
