//! GISSetChromakeyColorMsgCommand 
/*!
  Implementation of the Class GISSetChromakeyColorMsgCommand
*/

#if !defined(GISSetChromakeyColorMsgCommand_h)
#define GISSetChromakeyColorMsgCommand_h

#include "control/src/infoscreenmsg/base/GISAbsCommand.h"

class GISSetChromakeyColorMsgCommand : public GISAbsCommand
{
public:

	GISSetChromakeyColorMsgCommand();
	virtual ~GISSetChromakeyColorMsgCommand();

	void DoExecute() throw (GException);
};
#endif
