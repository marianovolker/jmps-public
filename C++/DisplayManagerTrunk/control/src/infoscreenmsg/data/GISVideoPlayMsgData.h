//! GISVideoPlayMsgData 
/*!
  Implementation of the Class GISVideoPlayMsgData
*/

#if !defined(GISVideoPlayMsgData_h)
#define GISVideoPlayMsgData_h

#include "control/src/infoscreenmsg/base/GISAbsData.h"

class GISVideoPlayMsgData : public GISAbsData
{
public:
	GISVideoPlayMsgData();
	virtual ~GISVideoPlayMsgData();

	void Validate() throw(GException);
	
	QString GetName();
	
	QString GetCommand();	
	
public:	
	static const char * m_CName;
	static const char * m_CCommandId;	
};
#endif
