//! GISAddDisplayMsgData 
/*!
  Implementation of the Class GISAddDisplayMsgData
*/

#if !defined(GISAddDisplayMsgData_h)
#define GISAddDisplayMsgData_h

#include "control/src/infoscreenmsg/base/GISAbsData.h"

class GISAddDisplayMsgData : public GISAbsData
{
public:
	GISAddDisplayMsgData();
	virtual ~GISAddDisplayMsgData();
	
	void Validate() throw(GException);
	
	QString GetName();
	
	QString GetCommand();	
	
public:	
	static const char * m_CName;
	static const char * m_CCommandId;	
};
#endif
