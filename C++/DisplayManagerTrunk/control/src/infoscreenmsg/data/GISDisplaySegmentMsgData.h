//! GISDisplaySegmentMsgData 
/*!
  Implementation of the Class GISDisplaySegmentMsgData
*/

#if !defined(GISDisplaySegmentMsgData_h)
#define GISDisplaySegmentMsgData_h

#include "control/src/infoscreenmsg/base/GISAbsData.h"

class GISDisplaySegmentMsgData : public GISAbsData
{
public:
	GISDisplaySegmentMsgData();
	virtual ~GISDisplaySegmentMsgData();

	void Validate() throw(GException);
	
	QString GetName();
	
	QString GetCommand();	
	
public:	
	static const char * m_CName;
	static const char * m_CCommandId;	
};
#endif
