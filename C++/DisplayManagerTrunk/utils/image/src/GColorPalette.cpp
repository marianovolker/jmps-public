#include "GColorPalette.h"

const QString GColorPalette::CXMLTag			("ColorPalette");
const QString GColorPalette::CXMLRegisterTag	("Register");
const QString GColorPalette::CXMLNumber			("number");
const QString GColorPalette::CXMLRed			("red");
const QString GColorPalette::CXMLGreen			("green");
const QString GColorPalette::CXMLBlue			("blue");

const int GColorPalette::CFontBGColorRegister	(6);
const int GColorPalette::CFontColorRegister		(14);
const int GColorPalette::CFirstRegister			(1);
const int GColorPalette::CLastRegister			(16);


GColorPalette::GColorPalette()
{
	m_aryColors = new GColor* [CLastRegister];	
	m_aryColors[0] = new GColor(1, 0, 0, 0);
	m_aryColors[1] = new GColor(2, 0, 0, 170);
	m_aryColors[2] = new GColor(3, 0, 170, 0);
	m_aryColors[3] = new GColor(4, 0, 170, 170);
	m_aryColors[4] = new GColor(5, 170, 0, 0);
	m_aryColors[5] = new GColor(6, 0, 0, 0); 			// Font Background color
	m_aryColors[6] = new GColor(7, 170, 85, 0);
	m_aryColors[7] = new GColor(8, 170, 170, 170);
	m_aryColors[8] = new GColor(9, 85, 85, 85);
	m_aryColors[9] = new GColor(10, 0, 0, 255);
	m_aryColors[10] = new GColor(11, 0, 255, 0);
	m_aryColors[11] = new GColor(12, 0, 255, 255);
	m_aryColors[12] = new GColor(13, 255, 0, 0);
	m_aryColors[13] = new GColor(14, 255, 255, 255); 	// Font color
	m_aryColors[14] = new GColor(15, 255, 255, 0);
	m_aryColors[15] = new GColor(16, 255, 255, 255);	
}

GColorPalette::~GColorPalette()
{
	
	int i = -1;
	while(++i <= 15) 
	{
		delete m_aryColors[i];
		m_aryColors[i] = NULL;
	}
	delete 	[]m_aryColors;
	m_aryColors = NULL;
}

int GColorPalette::GetBlue(int iRegister) throw(GException)
{
	if(iRegister < CFirstRegister || iRegister > CLastRegister)
	{
		throw GEXCEPTION(QString("Invalid Register"));
	}
	return m_aryColors[iRegister - 1]->GetBlue();
}

int GColorPalette::GetGreen(int iRegister) throw(GException)
{
	if(iRegister < CFirstRegister || iRegister > CLastRegister)
	{
		throw GEXCEPTION(QString("Invalid Register"));
	}
	return m_aryColors[iRegister - 1]->GetGreen();
}

int GColorPalette::GetRed(int iRegister) throw(GException)
{
	if(iRegister < CFirstRegister || iRegister > CLastRegister)	
	{
		throw GEXCEPTION(QString("Invalid Register"));
	}
	return m_aryColors[iRegister - 1]->GetRed();
}

void GColorPalette::SetColor(int iReg, int iRed, int iGreen, int iBlue)  throw(GException)
{
	if(iReg < CFirstRegister || iReg > CLastRegister)	
	{
		throw GEXCEPTION(QString("Invalid Register"));
	}	
	
	m_aryColors[iReg - 1]->SetRed(iRed);
	m_aryColors[iReg - 1]->SetGreen(iGreen);
	m_aryColors[iReg - 1]->SetBlue(iBlue);
}
