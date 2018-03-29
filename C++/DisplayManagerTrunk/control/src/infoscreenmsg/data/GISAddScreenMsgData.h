//! GISAddScreenMsgData 
/*!
  Implementation of the Class GISAddScreenMsgData
*/

#if !defined(GISAddScreenMsgData_h)
#define GISAddScreenMsgData_h

#include "control/src/infoscreenmsg/base/GISAbsData.h"

class GISAddScreenMsgData : public GISAbsData
{
public:
	GISAddScreenMsgData();
	virtual ~GISAddScreenMsgData();

	void Validate() throw(GException);
	
	QString GetName();
	
	QString GetCommand();	
	
public:	
	static const char * m_CName;
	static const char * m_CCommandId;	
};
#endif
