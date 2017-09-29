//! GISDisplayScreenMsgCommand 
/*!
  Implementation of the Class GISDisplayScreenMsgCommand
*/

#if !defined(GISDisplayScreenMsgCommand_h)
#define GISDisplayScreenMsgCommand_h

#include "control/src/infoscreenmsg/base/GISAbsCommand.h"

class GISDisplayScreenMsgCommand : public GISAbsCommand
{
public:
	GISDisplayScreenMsgCommand();
	virtual ~GISDisplayScreenMsgCommand();

	void DoExecute() throw (GException);
};
#endif
