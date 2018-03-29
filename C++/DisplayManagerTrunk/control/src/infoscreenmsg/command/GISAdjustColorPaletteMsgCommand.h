//! GISAdjustColorPaletteMsgCommand 
/*!
  Implementation of the Class GISAdjustColorPaletteMsgCommand
*/

#if !defined(GISAdjustColorPaletteMsgCommand_h)
#define GISAdjustColorPaletteMsgCommand_h

#include "control/src/infoscreenmsg/base/GISAbsCommand.h"

class GISAdjustColorPaletteMsgCommand : public GISAbsCommand
{
public:
	GISAdjustColorPaletteMsgCommand();
	virtual ~GISAdjustColorPaletteMsgCommand();
	
	void DoExecute() throw (GException);
};
#endif
