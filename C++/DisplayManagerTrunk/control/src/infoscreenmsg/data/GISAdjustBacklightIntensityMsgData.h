//! GISAdjustBacklightIntensityMsgData 
/*!
  Implementation of the Class GISAdjustBacklightIntensityMsgData
*/

#if !defined(GISAdjustBacklightIntensityMsgData_h)
#define GISAdjustBacklightIntensityMsgData_h

#include "control/src/infoscreenmsg/base/GISAbsData.h"

class GISAdjustBacklightIntensityMsgData : public GISAbsData
{
public:
	GISAdjustBacklightIntensityMsgData();
	virtual ~GISAdjustBacklightIntensityMsgData();

	void Validate() throw(GException);
	
	QString GetName();
	
	QString GetCommand();	
	
public:	
	static const char * m_CName;
	static const char * m_CCommandId;	
};
#endif
