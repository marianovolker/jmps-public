//! GISAddFieldMsgData 
/*!
  Implementation of the Class GISAddFieldMsgData
*/

#if !defined(GISAddFieldMsgData_h)
#define GISAddFieldMsgData_h

#include "control/src/infoscreenmsg/base/GISAbsData.h"

class GISAddFieldMsgData : public GISAbsData
{
public:
	GISAddFieldMsgData();
	virtual ~GISAddFieldMsgData();
	
	void Validate() throw(GException);
	
	QString GetName();
	
	QString GetCommand();	
	
public:	
	static const char * m_CName;
	static const char * m_CCommandId;	
};
#endif
