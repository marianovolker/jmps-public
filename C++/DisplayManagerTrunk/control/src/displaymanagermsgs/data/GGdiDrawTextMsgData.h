//! GGdiDrawTextMsgData 
/*!
  Gdi DrawText Message Data
*/

#ifndef GGDI_DRAW_TEXT_MSG_DATA_H
#define GGDI_DRAW_TEXT_MSG_DATA_H

#include <QSet>
#include "control/src/displaymanagermsgs/base/GAbsMsgData.h"

class GGdiDrawTextMsgData : public GAbsMsgData
{

public:
	GGdiDrawTextMsgData();
	virtual ~GGdiDrawTextMsgData();

	QString GetBGColor();
	void SetBGColor(QString strBGColor);

	QString GetFGColor();
	void SetFGColor(QString strFGColor);
	
	QString GetText();
	void SetText(QString strText);
	
	int GetXLoc();
	void SetXLoc(int iXLoc);
	
	int GetYLoc();
	void SetYLoc(int iYLoc);
	
	QString GetFont();
	void SetFont(QString strFont);
	
	QString GetAttr();
	void SetAttr(QString strAttr);

	virtual void Validate() throw(GException);

protected:
	static bool IsValidFont(QString const & qksFont);

private:
	QString m_strBGColor;
	QString m_strFGColor;
	QString m_strText;
	int m_iXLoc;
	int m_iYLoc;
	QString m_strFont;
	QString m_strAttr;

private:
	static QSet<QString> const _qksetValidFont;
};

#endif

