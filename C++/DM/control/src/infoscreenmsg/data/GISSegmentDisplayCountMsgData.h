//! GISSegmentDisplayCountMsgData 
/*!
  Implementation of the Class GISSegmentDisplayCountMsgData
*/

#if !defined(GISSegmentDisplayCountMsgData_h)
#define GISSegmentDisplayCountMsgData_h

#include "control/src/infoscreenmsg/base/GISAbsData.h"

class GISSegmentDisplayCountMsgData : public GISAbsData
{
public:
	GISSegmentDisplayCountMsgData();
	virtual ~GISSegmentDisplayCountMsgData();

	void Validate() throw(GException);
	
	QString GetName();
	
	QString GetCommand();	
	
public:	
	static const char * m_CName;
	static const char * m_CCommandId;	
};
#endif
