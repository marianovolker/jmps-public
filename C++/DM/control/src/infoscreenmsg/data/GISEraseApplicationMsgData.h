//! GISEraseApplicationMsgData 
/*!
  Implementation of the Class GISEraseApplicationMsgData
*/

#if !defined(GISEraseApplicationMsgData_h)
#define GISEraseApplicationMsgData_h

#include "control/src/infoscreenmsg/base/GISAbsData.h"

class GISEraseApplicationMsgData : public GISAbsData
{
public:
	GISEraseApplicationMsgData();
	virtual ~GISEraseApplicationMsgData();

	void Validate() throw(GException);
	
	QString GetName();
	
	QString GetCommand();	
	
public:	
	static const char * m_CName;
	static const char * m_CCommandId;	
};
#endif
