#ifndef GTEXTFIELD_H_
#define GTEXTFIELD_H_

#include <QString>

#include "GAttributeEnum.h"

class GSegment;

class GTextField
{
public:
	GTextField(int iNumber, GSegment* oSegment);
	virtual ~GTextField();
	GTextField(const GTextField&);
	
	int GetNumber() const;
	GSegment* GetSegment() const;
	
	QString GetText() const;
	void SetText(QString strText);
	
	bool IsFlashing() const;
	void SetFlashing(bool bFlashing);
		
	int GetJustification() const;
	void SetJustification(int iJustification);

	int GetScrollSpeed() const;
	void SetScrollSpeed(int iScrollSpeed);

	bool IsUnderscore() const;
	void SetUnderscore(bool bUnderscore);
	
	bool IsVisible() const;
	void SetVisible(bool bVisible);

	int GetXpos() const;
	void SetXpos(int iXPos);
	
	int GetYpos() const;
	void SetYpos(int iYPos);
	
	int GetScroll() const;
	void SetScroll(int iScroll);
	
	int GetFont() const;
	void SetFont(int iFont);
	
	void SetAttr(QString strAttr);
	QString GetAttr() const;
	
	int GetAttribute() const;
	void SetAttribute(int iAttribute);

public:
	static const QString CXMLTag;
	static const QString CXMLNumber;
	static const QString CXMLSegmentNumber;
	static const QString CXMLText;	
	static const QString CXMLFlashing;
	static const QString CXMLJustification;
	static const QString CXMLScrollSpeed;
	static const QString CXMLUnderscore;
	static const QString CXMLVisible;
	static const QString CXMLXPos;
	static const QString CXMLYPos;
	static const QString CXMLScroll;
	static const QString CXMLFont;		
	
	static const int CJustificationNone;
	static const int CJustificationLeft;
	static const int CJustificationCenter;
	static const int CJustificationRight;
	static const int CSpeedZero;
	static const int CSpeedLow;
	static const int CSpeedMedium;
	static const int CSpeedFast;
	static const int CScrollNoscroll;
	static const int CScrollHorizontal;
	static const int CScrollVertical;
	static const int CScrollPop;

private:
	int m_iNumber;
	GSegment* m_oSegment;
	int m_iXPos;
	int m_iYPos;
	QString m_strText;
	bool m_bVisible;
	bool m_bFlashing;
	bool m_bUnderscore;
	int m_iScroll;
	int m_iScrollSpeed;
	int m_iJustification;
	int m_iFont;
	QString m_strAttribute;
	int m_iAttribute;
};

#endif
