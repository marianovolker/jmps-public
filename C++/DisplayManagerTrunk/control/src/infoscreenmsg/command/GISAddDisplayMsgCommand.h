//! GISAddDisplayMsgCommand 
/*!
  Implementation of the Class GISAddDisplayMsgCommand
*/

#if !defined(GISAddDisplayMsgCommand_h)
#define GISAddDisplayMsgCommand_h

#include "control/src/infoscreenmsg/base/GISAbsCommand.h"

class GISAddDisplayMsgCommand : public GISAbsCommand
{
public:
	GISAddDisplayMsgCommand();
	virtual ~GISAddDisplayMsgCommand();

	void DoExecute() throw (GException);
};
#endif
