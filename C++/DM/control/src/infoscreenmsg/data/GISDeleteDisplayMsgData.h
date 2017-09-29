//! GISDeleteDisplayMsgData 
/*!
  Implementation of the Class GISDeleteDisplayMsgData
*/

#if !defined(GISDeleteDisplayMsgData_h)
#define GISDeleteDisplayMsgData_h

#include "control/src/infoscreenmsg/base/GISAbsData.h"

class GISDeleteDisplayMsgData : public GISAbsData
{
public:
	GISDeleteDisplayMsgData();
	virtual ~GISDeleteDisplayMsgData();

	void Validate() throw(GException);
	
	QString GetName();
	
	QString GetCommand();	
	
public:	
	static const char * m_CName;
	static const char * m_CCommandId;	
};
#endif
