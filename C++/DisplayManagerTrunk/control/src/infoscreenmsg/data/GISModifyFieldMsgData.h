//! GISModifyFieldMsgData 
/*!
  Implementation of the Class GISModifyFieldMsgData
*/

#if !defined(GISModifyFieldMsgData_h)
#define GISModifyFieldMsgData_h

#include "control/src/infoscreenmsg/base/GISAbsData.h"

class GISModifyFieldMsgData : public GISAbsData
{
public:
	GISModifyFieldMsgData();
	virtual ~GISModifyFieldMsgData();

	void Validate() throw(GException);
	
	QString GetName();
	
	QString GetCommand();	
	
public:	
	static const char * m_CName;
	static const char * m_CCommandId;	
};
#endif
