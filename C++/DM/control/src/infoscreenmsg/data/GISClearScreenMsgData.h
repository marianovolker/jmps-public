//! GISClearScreenMsgData 
/*!
  Implementation of the Class GISClearScreenMsgData
*/

#if !defined(GISClearScreenMsgData_h)
#define GISClearScreenMsgData_h

#include "control/src/infoscreenmsg/base/GISAbsData.h"

class GISClearScreenMsgData : public GISAbsData
{
public:
	GISClearScreenMsgData();
	virtual ~GISClearScreenMsgData();

	void Validate() throw(GException);
	
	QString GetName();
	
	QString GetCommand();	
	
public:	
	static const char * m_CName;
	static const char * m_CCommandId;	
};
#endif
