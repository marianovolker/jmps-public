//! GISModifyFieldMsgCommand 
/*!
  Implementation of the Class GISModifyFieldMsgCommand
*/

#if !defined(GISModifyFieldMsgCommand_h)
#define GISModifyFieldMsgCommand_h

#include "control/src/infoscreenmsg/base/GISAbsCommand.h"

class GISModifyFieldMsgCommand : public GISAbsCommand
{
public:

	GISModifyFieldMsgCommand();
	virtual ~GISModifyFieldMsgCommand();

	void DoExecute() throw (GException);
};
#endif
