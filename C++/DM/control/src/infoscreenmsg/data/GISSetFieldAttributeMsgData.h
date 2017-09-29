//! GISSetFieldAttributeMsgData 
/*!
  Implementation of the Class GISSetFieldAttributeMsgData
*/

#if !defined(GISSetFieldAttributeMsgData_h)
#define GISSetFieldAttributeMsgData_h

#include "control/src/infoscreenmsg/base/GISAbsData.h"

class GISSetFieldAttributeMsgData : public GISAbsData
{
public:
	GISSetFieldAttributeMsgData();
	virtual ~GISSetFieldAttributeMsgData();

	void Validate() throw(GException);
	
	QString GetName();
	
	QString GetCommand();	
	
public:	
	static const char * m_CName;
	static const char * m_CCommandId;	
};
#endif
