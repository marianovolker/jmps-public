//! GISDeleteDisplayMsgCommand 
/*!
  Implementation of the Class GISDeleteDisplayMsgCommand
*/

#if !defined(GISDeleteDisplayMsgCommand_h)
#define GISDeleteDisplayMsgCommand_h

#include "control/src/infoscreenmsg/base/GISAbsCommand.h"

class GISDeleteDisplayMsgCommand : public GISAbsCommand
{
public:
	GISDeleteDisplayMsgCommand();
	virtual ~GISDeleteDisplayMsgCommand();

	void DoExecute() throw (GException);
};
#endif
