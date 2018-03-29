//! GISAddFieldMsgCommand 
/*!
  Implementation of the Class GISAddFieldMsgCommand
*/

#if !defined(GISAddFieldMsgCommand_h)
#define GISAddFieldMsgCommand_h

#include "control/src/infoscreenmsg/base/GISAbsCommand.h"

class GISAddFieldMsgCommand : public GISAbsCommand
{
public:
	GISAddFieldMsgCommand();
	virtual ~GISAddFieldMsgCommand();
	
	void DoExecute() throw (GException);
};
#endif
