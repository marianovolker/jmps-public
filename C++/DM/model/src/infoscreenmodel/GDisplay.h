#ifndef GDISPLAY_H_
#define GDISPLAY_H_

#include <QMap>
#include <QList>
#include <QString>

#include "utils/image/src/GColorPalette.h"

class GScreen;

class GDisplay
{
public:
	GDisplay(int iNumber);
	virtual ~GDisplay();
	
	int GetNumber();
	
	void SetColor(bool bColor);
	bool IsColor();
	
	void AddScreen(GScreen* oScreen);
	GScreen* GetScreen(int iNumber);
	QList<GScreen *> GetAllScreens();
	void RemoveScreen(int iNumber);
	
	GScreen* GetCurrentScreen();	
	bool IsCurrentScreen(GScreen *oScreen);
	void SetCurrentScreen(GScreen *oScreen);
	
	GColorPalette* GetColorPalette();
	void SetColorPalette(GColorPalette *oColorPalette);	
	
public:
	static const QString CXMLTag;
	static const QString CXMLNumber;
	static const QString CXMLCurrentScreen;
	static const QString CXMLIsColor;
	
private:
	int m_iNumber;
	bool m_bIsColor;
	QMap<int, GScreen *> *m_oScreens;
	GScreen *m_oCurrentScreen;
	GColorPalette *m_oColorPalette;
};

#endif
