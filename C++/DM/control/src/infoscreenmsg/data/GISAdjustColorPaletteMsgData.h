//! GISAdjustColorPaletteMsgData 
/*!
  Implementation of the Class GISAdjustColorPaletteMsgData
*/

#if !defined(GISAdjustColorPaletteMsgData_h)
#define GISAdjustColorPaletteMsgData_h

#include "control/src/infoscreenmsg/base/GISAbsData.h"

class GISAdjustColorPaletteMsgData : public GISAbsData
{
public:
	GISAdjustColorPaletteMsgData();
	virtual ~GISAdjustColorPaletteMsgData();
	
	void Validate() throw(GException);
	
	QString GetName();
	
	QString GetCommand();	
	
public:	
	static const char * m_CName;
	static const char * m_CCommandId;	
};
#endif
