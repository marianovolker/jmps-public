#ifndef GCOLORPALETTE_H_
#define GCOLORPALETTE_H_

#include <QString>

#include "GColor.h"
#include "utils/base/src/GException.h"

class GColorPalette
{
public:
	GColorPalette();
	virtual ~GColorPalette();
	
	int GetBlue(int iRegister) 	throw(GException);
	int GetGreen(int iRegister) throw(GException);
	int GetRed(int iRegister) 	throw(GException);
	void SetColor(int iReg, int iRed, int iGreen, int iBlue) throw(GException);	

public:		
	static const QString CXMLTag;
	static const QString CXMLRegisterTag;	
	static const QString CXMLNumber;
	static const QString CXMLRed;
	static const QString CXMLGreen;
	static const QString CXMLBlue;
	
	static const int CFontBGColorRegister;
	static const int CFontColorRegister;
	static const int CFirstRegister;
	static const int CLastRegister;
	
private:
	GColor** m_aryColors;
};

#endif
