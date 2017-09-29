//! GISDisplayScreenMsgData 
/*!
  Implementation of the Class GISDisplayScreenMsgData
*/

#if !defined(GISDisplayScreenMsgData_h)
#define GISDisplayScreenMsgData_h

#include "control/src/infoscreenmsg/base/GISAbsData.h"

class GISDisplayScreenMsgData : public GISAbsData
{
public:
	GISDisplayScreenMsgData();
	virtual ~GISDisplayScreenMsgData();

	void Validate() throw(GException);
	
	QString GetName();
	
	QString GetCommand();	
	
public:	
	static const char * m_CName;
	static const char * m_CCommandId;	
};
#endif
