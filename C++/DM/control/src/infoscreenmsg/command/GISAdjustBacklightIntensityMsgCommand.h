//! GISAdjustBacklightIntensityMsgCommand 
/*!
  Implementation of the Class GISAdjustBacklightIntensityMsgCommand
*/

#if !defined(GISAdjustBacklightIntensityMsgCommand_h)
#define GISAdjustBacklightIntensityMsgCommand_h

#include "control/src/infoscreenmsg/base/GISAbsCommand.h"

class GISAdjustBacklightIntensityMsgCommand : public GISAbsCommand
{
public:

	GISAdjustBacklightIntensityMsgCommand();
	virtual ~GISAdjustBacklightIntensityMsgCommand();

	void DoExecute() throw (GException);
};
#endif
