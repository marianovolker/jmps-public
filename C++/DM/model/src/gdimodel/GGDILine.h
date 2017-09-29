#if !defined(GGDILine_h)
#define GGDILine_h

#include <QString>

class GGDILine
{
public:
	GGDILine();
	virtual ~GGDILine();

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

	int GetLineNumber();
	void SetLineNumber(int iLineNumber);

private:
	QString m_strThickness;
	QString m_strColor;
	int m_iLength;
	int m_iLineNumber;
	int m_iXLoc;
	int m_iYLoc;
};

#endif
