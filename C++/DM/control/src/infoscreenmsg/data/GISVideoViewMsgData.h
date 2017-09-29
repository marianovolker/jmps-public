//! GISVideoViewMsgData 
/*!
  Implementation of the Class GISVideoViewMsgData
*/

#if !defined(GISVideoViewMsgData_h)
#define GISVideoViewMsgData_h

#include "control/src/infoscreenmsg/base/GISAbsData.h"

class GISVideoViewMsgData : public GISAbsData
{
public:
	GISVideoViewMsgData();
	virtual ~GISVideoViewMsgData();

	void Validate() throw(GException);
	
	QString GetName();
	
	QString GetCommand();	
	
public:	
	static const char * m_CName;
	static const char * m_CCommandId;	
};
#endif
