//! GISVideoFrameSeekMsgData 
/*!
  Implementation of the Class GISVideoFrameSeekMsgData
*/

#if !defined(GISVideoFrameSeekMsgData_h)
#define GISVideoFrameSeekMsgData_h

#include "control/src/infoscreenmsg/base/GISAbsData.h"

class GISVideoFrameSeekMsgData : public GISAbsData
{
public:
	GISVideoFrameSeekMsgData();
	virtual ~GISVideoFrameSeekMsgData();

	void Validate() throw(GException);
	
	QString GetName();
	
	QString GetCommand();	
	
public:	
	static const char * m_CName;
	static const char * m_CCommandId;	
};
#endif
