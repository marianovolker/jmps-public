//! GISVideoRangePlayMsgData
/*!
  Implementation of the Class GISVideoRangePlayMsgData
*/

#if !defined(GISVideoRangePlayMsgData_h)
#define GISVideoRangePlayMsgData_h

#include "control/src/infoscreenmsg/base/GISAbsData.h"

class GISVideoRangePlayMsgData : public GISAbsData
{
public:
	GISVideoRangePlayMsgData();
	virtual ~GISVideoRangePlayMsgData();

	void Validate() throw(GException);
	
	QString GetName();
	
	QString GetCommand();	
	
public:	
	static const char * m_CName;
	static const char * m_CCommandId;	
};
#endif
