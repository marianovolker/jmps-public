//! GISSetGraphicsTypeMsgData 
/*!
  Implementation of the Class GISSetGraphicsTypeMsgData
*/

#if !defined(GISSetGraphicsTypeMsgData_h)
#define GISSetGraphicsTypeMsgData_h

#include "control/src/infoscreenmsg/base/GISAbsData.h"

class GISSetGraphicsTypeMsgData : public GISAbsData
{
public:
	GISSetGraphicsTypeMsgData();
	virtual ~GISSetGraphicsTypeMsgData();

	void Validate() throw(GException);
	
	QString GetName();
	
	QString GetCommand();	
	
public:	
	static const char * m_CName;
	static const char * m_CCommandId;	
};
#endif
