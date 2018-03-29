#ifndef GSEGMENT_H_
#define GSEGMENT_H_

#include <QMap>
#include <QList>

#include "GTextField.h"
#include "GDisplay.h"

class GScreen;

class GSegment
{
public:
	GSegment(int iNumber, GScreen* oScreen);
	virtual ~GSegment();
	GSegment(const GSegment&);

	void AddField(GTextField* oField);	
	GTextField* GetField(int iNumber) const;
	QList<GTextField *> GetAllFields() const;
	void RemoveField(int number);	
	
	int GetNumber() const;
	GScreen* GetScreen() const;
	
	int GetBGColor() const;
	void SetBGColor(int iColor);
	
	bool IsVisible() const;
	void SetVisible(bool bVisible);

	int GetWidth() const;
	void SetWidth(int iWidth);
	
	int GetHeight() const;
	void SetHeight(int iHeight);
	
	int GetXPos() const;
	void SetXPos(int iXPos);
	
	int GetYPos() const;
	void SetYPos(int iYPos);	

	static GSegment *Clone(GSegment *);

private:

	QMap<int, GTextField *> *GetFieldsMap() const;

public:

	static const QString CXMLTag;
	static const QString CXMLNumber;
	static const QString CXMLScreenNumber;
	static const QString CXMLBGColor;
	static const QString CXMLVisible;
	static const QString CXMLWidth;
	static const QString CXMLHeight;
	static const QString CXMLXPos;
	static const QString CXMLYPos;
	
private:

	int m_iNumber;
	GScreen* m_oScreen;	
	int m_iBGColor;
	bool m_bVisible;

	int m_iWidth;//Segment width in pixels.
	int m_iHeight;//Segment height in pixels.

	/**
	 * X and Y pixel screen coordinate for placement of upper left corner of segment rectangle. 
	 * X Visible range: 001 to 709 for color and 001 to 320 for monochrome.
	 * Y Visible range: 001 to 471 for color and 001 to 240 for monochrome.
	 */
	int m_iXPos;
	int m_iYPos;

	QMap<int, GTextField *> *m_oFields;
};

#endif
