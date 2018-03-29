#include "GColor.h"

GColor::GColor(int iReg, int iRed, int iGreen, int iBlue)
	: m_iBlue(iBlue), 
	  m_iGreen(iGreen),
	  m_iRed(iRed),
	  m_iRegister(iReg)
{
		
}

int GColor::GetBlue() 
{
	return m_iBlue;
}

void GColor::SetBlue(int iBlue) 
{
	m_iBlue = iBlue;
}

int GColor::GetGreen() 
{
	return m_iGreen;
}

void GColor::SetGreen(int iGreen) 
{
	m_iGreen = iGreen;
}

int GColor::GetRed() 
{
	return m_iRed;
}

void GColor::SetRed(int iRed) 
{
	m_iRed = iRed;
}

int GColor::GetRegister() 
{
	return m_iRegister;
}

