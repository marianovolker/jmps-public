//! GISDeleteSegmentMsgData 
/*!
  Implementation of the Class GISDeleteSegmentMsgData
*/

#if !defined(GISDeleteSegmentMsgData_h)
#define GISDeleteSegmentMsgData_h

#include "control/src/infoscreenmsg/base/GISAbsData.h"

class GISDeleteSegmentMsgData : public GISAbsData
{
public:
	GISDeleteSegmentMsgData();
	virtual ~GISDeleteSegmentMsgData();

	void Validate() throw(GException);
	
	QString GetName();
	
	QString GetCommand();	
	
public:	
	static const char * m_CName;
	static const char * m_CCommandId;	
};
#endif
