//! GISObjectAddressMsgCommand 
/*!
  Implementation of the Class GISObjectAddressMsgCommand
*/

#if !defined(GISObjectAddressMsgCommand_h)
#define GISObjectAddressMsgCommand_h

#include "control/src/infoscreenmsg/base/GISAbsCommand.h"

class GISObjectAddressMsgCommand : public GISAbsCommand
{
public:

	GISObjectAddressMsgCommand();
	virtual ~GISObjectAddressMsgCommand();

	void DoExecute() throw (GException);
};
#endif
