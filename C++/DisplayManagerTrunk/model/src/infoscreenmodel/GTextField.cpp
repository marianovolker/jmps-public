#include "GTextField.h"

#include "GSegment.h"

const QString GTextField::CXMLTag			("TextField");
const QString GTextField::CXMLNumber		("number");
const QString GTextField::CXMLSegmentNumber	("segmentNumber");
const QString GTextField::CXMLText			("text");
const QString GTextField::CXMLFlashing		("flashing");
const QString GTextField::CXMLJustification	("justification");
const QString GTextField::CXMLScrollSpeed	("scrollSpeed");
const QString GTextField::CXMLUnderscore	("underscore");
const QString GTextField::CXMLVisible		("visible");
const QString GTextField::CXMLXPos			("xPos");
const QString GTextField::CXMLYPos			("yPos");
const QString GTextField::CXMLScroll		("scroll");
const QString GTextField::CXMLFont			("font");

const int GTextField::CJustificationNone	(0);
const int GTextField::CJustificationLeft 	(1);
const int GTextField::CJustificationCenter 	(2);
const int GTextField::CJustificationRight 	(3);
const int GTextField::CSpeedZero 			(0);
const int GTextField::CSpeedLow 			(1);
const int GTextField::CSpeedMedium 			(2);
const int GTextField::CSpeedFast 			(3);
const int GTextField::CScrollNoscroll 		(0);
const int GTextField::CScrollHorizontal 	(1);
const int GTextField::CScrollVertical 		(2);
const int GTextField::CScrollPop 			(3);


GTextField::GTextField(int iNumber, GSegment* oSegment) 
	: m_iNumber			(iNumber),
	  m_oSegment		(oSegment),
	  m_iXPos			(0),
	  m_iYPos			(0),	  
	  m_bVisible		(true), 
	  m_bFlashing		(false), 
	  m_bUnderscore		(false),
	  m_iScroll			(CScrollNoscroll), 
	  m_iScrollSpeed	(CSpeedZero), 
	  m_iJustification	(CJustificationNone),
	  m_iFont			(0),
	  m_iAttribute		(0)
{
}

GTextField::GTextField(const GTextField& oTextField)
	: m_iNumber			(oTextField.GetNumber()),
	  m_oSegment		(oTextField.GetSegment()),
	  m_iXPos			(oTextField.GetXpos()),
	  m_iYPos			(oTextField.GetYpos()),
	  m_strText			(oTextField.GetText()),
	  m_bVisible		(oTextField.IsVisible()),
	  m_bFlashing		(oTextField.IsFlashing()),
	  m_bUnderscore		(oTextField.IsUnderscore()),
	  m_iScroll			(CScrollNoscroll),
	  m_iScrollSpeed	(CSpeedZero),
	  m_iJustification	(CJustificationNone),
	  m_iFont			(oTextField.GetFont()),
	  m_iAttribute		(oTextField.GetAttribute())
{
	SetAttr(oTextField.GetAttr());
}

GTextField::~GTextField()
{
}

int GTextField::GetNumber() const
{
	return m_iNumber;
}

GSegment* GTextField::GetSegment() const
{
	return m_oSegment;
}

QString GTextField::GetText() const
{
	return m_strText;
}

void GTextField::SetText(QString strText) 
{
	m_strText = strText;
}

bool GTextField::IsFlashing() const
{
	return m_bFlashing;
}

void GTextField::SetFlashing(bool bFlashing) 
{
	m_bFlashing = bFlashing;
}


int GTextField::GetJustification() const
{
	return m_iJustification;
}

void GTextField::SetJustification(int iJustification) 
{
	m_iJustification = iJustification;
}

int GTextField::GetScrollSpeed() const
{
	return m_iScrollSpeed;
}

void GTextField::SetScrollSpeed(int iScrollSpeed) 
{
	m_iScrollSpeed = iScrollSpeed;
}

bool GTextField::IsUnderscore() const
{
	return m_bUnderscore;
}

void GTextField::SetUnderscore(bool bUnderscore) 
{
	m_bUnderscore = bUnderscore;
}

bool GTextField::IsVisible() const
{
	return m_bVisible;
}

void GTextField::SetVisible(bool bVisible) 
{
	m_bVisible = bVisible;
}	

int GTextField::GetXpos() const
{
	return m_iXPos;
}

void GTextField::SetXpos(int iXPos)
{
	m_iXPos = iXPos;
}

int GTextField::GetYpos() const
{
	return m_iYPos;
}

void GTextField::SetYpos(int iYPos)
{
	m_iYPos = iYPos;
}

int GTextField::GetScroll() const
{
	return m_iScroll;
}

void GTextField::SetScroll(int iScroll)
{
	m_iScroll = iScroll;
}

int GTextField::GetFont() const
{
	return m_iFont;
}

void GTextField::SetFont(int iFont) 
{
	m_iFont = iFont;
}

void GTextField::SetAttr(QString strAttr) 
{
	m_strAttribute = strAttr;
	m_iAttribute = strAttr.toInt();
	
	switch(m_iAttribute) 
	{
		case GAttributeEnum::eVisible:
			SetVisible(true);
			break;
			
		case GAttributeEnum::eInvisible:
			SetVisible(false);
			break;
			
		case GAttributeEnum::eStartFlash:
			SetFlashing(true);
			break;
			
		case GAttributeEnum::eStopFlash:
			SetFlashing(false);
			break;
			
		case GAttributeEnum::eScrollMedium: // mono:pop info:horizontal
			SetScroll(CScrollPop);
			break;
			
		case GAttributeEnum::eStopScroll:
			break;
			
		case GAttributeEnum::eCenterText:
			SetJustification(CJustificationCenter);
			break;
			
		case GAttributeEnum::eLeftText:
			SetJustification(CJustificationLeft);
			break;
			
		case GAttributeEnum::eRightText:
			SetJustification(CJustificationRight);
			break;
			
		case GAttributeEnum::eUnderscoreOn:
			SetUnderscore(true);
			break;
			
		case GAttributeEnum::eCenterWithUnderscore:
			SetUnderscore(true);
			SetJustification(CJustificationCenter);
			break;
			
		case GAttributeEnum::eLeftWithUnderscore:
			SetUnderscore(true);
			SetJustification(CJustificationLeft);
			break;
			
		case GAttributeEnum::eRightWithUnderscore:
			SetUnderscore(true);
			SetJustification(CJustificationRight);
			break;
			
		case GAttributeEnum::eUnderscoreOff:
			SetUnderscore(false);
			break;
			
		case GAttributeEnum::eHorizontalScrollSlow:
			SetScroll(CScrollHorizontal);
			SetScrollSpeed(CSpeedLow);
			break;
			
		case GAttributeEnum::eHorizontalScrollMedium:
			SetScroll(CScrollHorizontal);
			SetScrollSpeed(CSpeedMedium);
			break;
			
		case GAttributeEnum::eHorizontalScrollFast:
			SetScroll(CScrollHorizontal);
			SetScrollSpeed(CSpeedFast);
			break;
			
		case GAttributeEnum::ePopTextSlow:
			SetScroll(CScrollPop);
			SetScrollSpeed(CSpeedLow);
			break;
			
		case GAttributeEnum::ePopTextMedium:
			SetScroll(CScrollPop);
			SetScrollSpeed(CSpeedMedium);
			break;
			
		case GAttributeEnum::ePopTextFast:
			SetScroll(CScrollPop);
			SetScrollSpeed(CSpeedFast);
			break;
			
		case GAttributeEnum::eVerticalScrollSlow:
			SetScroll(CScrollVertical);
			SetScrollSpeed(CSpeedLow);
			break;
			
		case GAttributeEnum::eVerticalScrollMedium:
			SetScroll(CScrollVertical);
			SetScrollSpeed(CSpeedMedium);
			break;
			
		case GAttributeEnum::eVerticalScrollFast:
			SetScroll(CScrollVertical);
			SetScrollSpeed(CSpeedFast);
			break;
	}		
}		

QString GTextField::GetAttr() const
{	
	return m_strAttribute;
}
	
int GTextField::GetAttribute() const
{
	return m_iAttribute;
}

void GTextField::SetAttribute(int iAttribute) 
{
	m_iAttribute = iAttribute;
}
