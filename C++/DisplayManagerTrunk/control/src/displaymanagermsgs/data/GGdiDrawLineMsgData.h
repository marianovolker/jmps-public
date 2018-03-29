//! GGdiDrawLineMsgData 
/*!
  Gdi DrawLine Message Data
*/

#ifndef GGDI_DRAW_LINE_MSG_DATA_H
#define GGDI_DRAW_LINE_MSG_DATA_H

#include "control/src/displaymanagermsgs/base/GAbsMsgData.h"
#include <QString>

class GGdiDrawLineMsgData : public GAbsMsgData
{
public:
	GGdiDrawLineMsgData();
	virtual ~GGdiDrawLineMsgData();

	QString GetThickness();
	void SetThickness(QString strThickness);
	
	QString GetColor();
	void SetColor(QString strColor);
	
	int GetLength();
	void SetLength(int iLength);
	
	int GetXLoc();
	void SetXLoc(int iXLoc);
	
	int GetYLoc();
	void SetYLoc(int iYLoc);

private:
	QString m_strThickness;
	QString m_strColor;
	int m_iLength;
	int m_iXLoc;
	int m_iYLoc;
};

#endif
