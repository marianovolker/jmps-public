//! GISAdjustSoundVolumeMsgCommand 
/*!
  Implementation of the Class GISAdjustSoundVolumeMsgCommand
*/

#if !defined(GISAdjustSoundVolumeMsgCommand_h)
#define GISAdjustSoundVolumeMsgCommand_h

#include "control/src/infoscreenmsg/base/GISAbsCommand.h"

class GISAdjustSoundVolumeMsgCommand : public GISAbsCommand
{
public:

	GISAdjustSoundVolumeMsgCommand();
	virtual ~GISAdjustSoundVolumeMsgCommand();

	void DoExecute() throw (GException);
};
#endif
