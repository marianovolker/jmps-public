//! GISSetScreenColorMsgData 
/*!
  Implementation of the Class GISSetScreenColorMsgData
*/

#if !defined(GISSetScreenColorMsgData_h)
#define GISSetScreenColorMsgData_h

#include "control/src/infoscreenmsg/base/GISAbsData.h"

class GISSetScreenColorMsgData : public GISAbsData
{
public:
	GISSetScreenColorMsgData();
	virtual ~GISSetScreenColorMsgData();

	void Validate() throw(GException);
	
	QString GetName();
	
	QString GetCommand();	
	
public:	
	static const char * m_CName;
	static const char * m_CCommandId;	
};
#endif
