//! GISStatusMsgData 
/*!
  Implementation of the Class GISStatusMsgData
*/

#if !defined(GISStatusMsgData_h)
#define GISStatusMsgData_h

#include "control/src/infoscreenmsg/base/GISAbsData.h"

class GISStatusMsgData : public GISAbsData
{
public:
	GISStatusMsgData();
	virtual ~GISStatusMsgData();

	void Validate() throw(GException);
	
	QString GetName();
	
	QString GetCommand();	
	
public:	
	static const char * m_CName;
	static const char * m_CCommandId;	
};
#endif
