//! GISDeleteFieldMsgData 
/*!
  Implementation of the Class GISDeleteFieldMsgData
*/

#if !defined(GISDeleteFieldMsgData_h)
#define GISDeleteFieldMsgData_h

#include "control/src/infoscreenmsg/base/GISAbsData.h"

class GISDeleteFieldMsgData : public GISAbsData
{
public:
	GISDeleteFieldMsgData();
	virtual ~GISDeleteFieldMsgData();

	void Validate() throw(GException);
	
	QString GetName();
	
	QString GetCommand();	
	
public:	
	static const char * m_CName;
	static const char * m_CCommandId;	
};
#endif
