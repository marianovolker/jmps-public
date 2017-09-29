#if !defined(GGDIRect_h)
#define GGDIRect_h

#include <QString>

class GGDIRect
{

public:
	GGDIRect();
	virtual ~GGDIRect();

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

	int GetRectNumber();
	void SetRectNumber(int iRectNumber);

private:
	QString m_strColor;
	int m_iXLoc;
	int m_iYLoc;
	int m_iHeight;
	int m_iWidth;
	int m_iRectNumber;
};

#endif
