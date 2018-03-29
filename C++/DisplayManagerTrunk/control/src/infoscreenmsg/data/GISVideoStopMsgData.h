//! GISVideoStopMsgData 
/*!
  Implementation of the Class GISVideoStopMsgData
*/

#if !defined(GISVideoStopMsgData_h)
#define GISVideoStopMsgData_h

#include "control/src/infoscreenmsg/base/GISAbsData.h"

class GISVideoStopMsgData : public GISAbsData
{
public:
	GISVideoStopMsgData();
	virtual ~GISVideoStopMsgData();

	void Validate() throw(GException);
	
	QString GetName();
	
	QString GetCommand();	
	
public:	
	static const char * m_CName;
	static const char * m_CCommandId;	
};
#endif
