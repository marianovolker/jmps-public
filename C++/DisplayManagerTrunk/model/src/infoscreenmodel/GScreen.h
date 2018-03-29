#ifndef GSCREEN_H_
#define GSCREEN_H_

#include <QMap>
#include <QList>

#include "GSegment.h"

class GDisplay;

class GScreen
{
public:
	GScreen(int number, GDisplay *oDisplay);
	virtual ~GScreen();
	
	int GetNumber();
	GDisplay* GetDisplay();	
	
	int GetBGColor();
	void SetBGColor(int iColor);
	
	void AddSegment(GSegment* oSegment);	
	GSegment* GetSegment(int iNumber);
	QList<GSegment *> GetAllSegments();
	void RemoveSegment(int iNumber);
	bool IsPersistent();
	
public:
	static const QString CXMLTag;
	static const QString CXMLNumber;
	static const QString CXMLDisplayNumber;
	static const QString CXMLBGColor;
	
private:
	int m_iNumber;
	GDisplay* m_oDisplay;
	QMap<int, GSegment *> *m_oSegments;	
	int m_iBGColor;
};

#endif
