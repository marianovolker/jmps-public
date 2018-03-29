//! GISSaveApplicationMsgData 
/*!
  Implementation of the Class GISSaveApplicationMsgData
*/

#if !defined(GISSaveApplicationMsgData_h)
#define GISSaveApplicationMsgData_h

#include "control/src/infoscreenmsg/base/GISAbsData.h"

class GISSaveApplicationMsgData : public GISAbsData
{
public:
	GISSaveApplicationMsgData();
	virtual ~GISSaveApplicationMsgData();

	void Validate() throw(GException);
	
	QString GetName();
	
	QString GetCommand();	
	
public:	
	static const char * m_CName;
	static const char * m_CCommandId;	
};
#endif
