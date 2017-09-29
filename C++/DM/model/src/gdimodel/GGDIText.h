#if !defined(GGDIText_h)
#define GGDIText_h

#include <QString>

class GGDIText
{

public:
	GGDIText();
	virtual ~GGDIText();

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

	int GetTextNumber();
	void SetTextNumber(int iTextNumber);

private:
	QString m_strBGColor;
	QString m_strFGColor;
	QString m_strFont;
	QString m_strAttr;
	QString m_strText;
	int m_iTextNumber;
	int m_iXLoc;
	int m_iYLoc;
};

#endif
