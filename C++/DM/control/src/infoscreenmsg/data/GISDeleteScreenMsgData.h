//! GISDeleteScreenMsgData 
/*!
  Implementation of the Class GISDeleteScreenMsgData
*/

#if !defined(GISDeleteScreenMsgData_h)
#define GISDeleteScreenMsgData_h

#include "control/src/infoscreenmsg/base/GISAbsData.h"

class GISDeleteScreenMsgData : public GISAbsData
{
public:
	GISDeleteScreenMsgData();
	virtual ~GISDeleteScreenMsgData();

	void Validate() throw(GException);
	
	QString GetName();
	
	QString GetCommand();	
	
public:	
	static const char * m_CName;
	static const char * m_CCommandId;	
};
#endif
