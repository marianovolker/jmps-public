//! GGdiFillRectMsgData 
/*!
  Gdi FillRect Message Data
*/

#ifndef GGDI_FILLRECT_MSG_DATA_H
#define GGDI_FILLRECT_MSG_DATA_H

#include "control/src/displaymanagermsgs/base/GAbsMsgData.h"
#include <QString>

class GGdiFillRectMsgData : public GAbsMsgData
{
public:
	GGdiFillRectMsgData();
	virtual ~GGdiFillRectMsgData();

	int GetXLoc();
	void SetXLoc(int iXLoc);
	
	int GetYLoc();
	void SetYLoc(int iYLoc);
	
	QString GetColor();
	void SetColor(QString strColor);
	
	int GetHeight();
	void SetHeight(int iHeight);
	
	int GetWidth();
	void SetWidth(int iWidth);

private:
	int m_iXLoc;
	int m_iYLoc;
	QString m_strColor;
	int m_iHeight;
	int m_iWidth;
};

#endif
