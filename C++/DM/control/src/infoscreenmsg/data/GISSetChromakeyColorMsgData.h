//! GISSetChromakeyColorMsgData 
/*!
  Implementation of the Class GISSetChromakeyColorMsgData
*/

#if !defined(GISSetChromakeyColorMsgData_h)
#define GISSetChromakeyColorMsgData_h

#include "control/src/infoscreenmsg/base/GISAbsData.h"

class GISSetChromakeyColorMsgData : public GISAbsData
{
public:
	GISSetChromakeyColorMsgData();
	virtual ~GISSetChromakeyColorMsgData();

	void Validate() throw(GException);
	
	QString GetName();
	
	QString GetCommand();	
	
public:	
	static const char * m_CName;
	static const char * m_CCommandId;	
};
#endif
