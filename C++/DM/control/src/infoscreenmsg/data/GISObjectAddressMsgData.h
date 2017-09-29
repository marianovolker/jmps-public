//! GISObjectAddressMsgData 
/*!
  Implementation of the Class GISObjectAddressMsgData
*/

#if !defined(GISObjectAddressMsgData_h)
#define GISObjectAddressMsgData_h

#include "control/src/infoscreenmsg/base/GISAbsData.h"

class GISObjectAddressMsgData : public GISAbsData
{
public:
	GISObjectAddressMsgData();
	virtual ~GISObjectAddressMsgData();

	void Validate() throw(GException);
	
	QString GetName();
	
	QString GetCommand();	
	
public:	
	static const char * m_CName;
	static const char * m_CCommandId;	
};
#endif
