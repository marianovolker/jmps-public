//! GISDeleteFieldMsgCommand 
/*!
  Implementation of the Class GISDeleteFieldMsgCommand
*/

#if !defined(GISDeleteFieldMsgCommand_h)
#define GISDeleteFieldMsgCommand_h

#include "control/src/infoscreenmsg/base/GISAbsCommand.h"

class GISDeleteFieldMsgCommand : public GISAbsCommand
{
public:
	GISDeleteFieldMsgCommand();
	virtual ~GISDeleteFieldMsgCommand();

	void DoExecute() throw (GException);
};
#endif
