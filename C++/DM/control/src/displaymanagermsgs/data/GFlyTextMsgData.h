//! GFlyTextMsgData 
/*!
  FlyText Message Data
*/

#ifndef GFLY_TEXT_MSG_DATA_H
#define GFLY_TEXT_MSG_DATA_H

#include "control/src/displaymanagermsgs/base/GAbsMsgData.h"
#include <QString>

class GFlyTextMsgData : public GAbsMsgData
{

public:
	GFlyTextMsgData();
	virtual ~GFlyTextMsgData();

	QString GetFont();
	void SetFont(QString strFont);
	
	QString GetText();
	void SetText(QString strText);
	
	int GetXLoc();
	void SetXLoc(int iXLoc);
	
	int GetYLoc();
	void SetYLoc(int iYLoc);

private:
	QString m_strFont;
	QString m_strText;
	int m_iXLoc;
	int m_iYLoc;
};

#endif
