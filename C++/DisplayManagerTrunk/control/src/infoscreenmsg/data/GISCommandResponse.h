//! GISCommandResponse 
/*!
  Implementation of the Class GISCommandResponse
*/

#if !defined(GISCommandResponse_h)
#define GISCommandResponse_h

#include <QString>
#include "utils/base/src/GAbsData.h"
#include "control/src/infoscreenmsg/utils/GISCommandStatusEnum.h"

class GISCommandResponse : public GAbsData
{
public:
	GISCommandResponse();
	virtual ~GISCommandResponse();
	
	int GetCommand();
	void SetCommand(int iCommand);
	
	GISCommandStatusEnum GetCommandStatus();
	void SetCommandStatus(GISCommandStatusEnum oCommandStatus);
	
	QString GetData();
	void SetData(QString strData);
	
	int GetDisplay();
	void SetDisplay(int iDisplay);
	
	virtual void Validate() throw(GException);
	
	QString ToString();

private:
	int m_iCommand;
	int m_iDisplay;
	GISCommandStatusEnum m_oCommandStatus;
	QString m_strData;
};
#endif
