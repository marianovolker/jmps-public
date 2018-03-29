//! GISModifySegmentMsgData 
/*!
  Implementation of the Class GISModifySegmentMsgData
*/

#if !defined(GISModifySegmentMsgData_h)
#define GISModifySegmentMsgData_h

#include "control/src/infoscreenmsg/base/GISAbsData.h"

class GISModifySegmentMsgData : public GISAbsData
{
public:
	GISModifySegmentMsgData();
	virtual ~GISModifySegmentMsgData();

	void Validate() throw(GException);
	
	QString GetName();
	
	QString GetCommand();	
	
public:	
	static const char * m_CName;
	static const char * m_CCommandId;	
};
#endif
