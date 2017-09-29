//! GISAddSegmentMsgData 
/*!
  Implementation of the Class GISAddSegmentMsgData
*/

#if !defined(GISAddSegmentMsgData_h)
#define GISAddSegmentMsgData_h

#include "control/src/infoscreenmsg/base/GISAbsData.h"

class GISAddSegmentMsgData : public GISAbsData
{
public:
	GISAddSegmentMsgData();
	virtual ~GISAddSegmentMsgData();

	void Validate() throw(GException);
	
	QString GetName();
	
	QString GetCommand();	
	
public:	
	static const char * m_CName;
	static const char * m_CCommandId;
	
};
#endif
