//! GISAdjustSoundVolumeMsgData 
/*!
  Implementation of the Class GISAdjustSoundVolumeMsgData
*/

#if !defined(GISAdjustSoundVolumeMsgData_h)
#define GISAdjustSoundVolumeMsgData_h

#include "control/src/infoscreenmsg/base/GISAbsData.h"

class GISAdjustSoundVolumeMsgData : public GISAbsData
{
public:
	GISAdjustSoundVolumeMsgData();
	virtual ~GISAdjustSoundVolumeMsgData();

	void Validate() throw(GException);
	
	QString GetName();
	
	QString GetCommand();	
	
public:	
	static const char * m_CName;
	static const char * m_CCommandId;	
};
#endif
