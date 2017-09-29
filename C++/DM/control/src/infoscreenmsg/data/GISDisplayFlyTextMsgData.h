//! GISDisplayFlyTextMsgData 
/*!
  Implementation of the Class GISDisplayFlyTextMsgData
*/

#if !defined(GISDisplayFlyTextMsgData_h)
#define GISDisplayFlyTextMsgData_h

#include "control/src/infoscreenmsg/base/GISAbsData.h"

class GISDisplayFlyTextMsgData : public GISAbsData
{
public:
	GISDisplayFlyTextMsgData();
	virtual ~GISDisplayFlyTextMsgData();

	void Validate() throw(GException);
	
	QString GetName();
	
	QString GetCommand();	
	
public:	
	static const char * m_CName;
	static const char * m_CCommandId;	
};
#endif
