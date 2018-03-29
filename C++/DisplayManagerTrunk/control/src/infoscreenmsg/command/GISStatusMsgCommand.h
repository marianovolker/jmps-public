//! GISStatusMsgCommand 
/*!
  Implementation of the Class GISStatusMsgCommand
*/

#if !defined(GISStatusMsgCommand_h)
#define GISStatusMsgCommand_h

#include "control/src/infoscreenmsg/base/GISAbsCommand.h"

class GISStatusMsgCommand : public GISAbsCommand
{
public:

	GISStatusMsgCommand();
	virtual ~GISStatusMsgCommand();

	void DoExecute() throw (GException);
};
#endif
