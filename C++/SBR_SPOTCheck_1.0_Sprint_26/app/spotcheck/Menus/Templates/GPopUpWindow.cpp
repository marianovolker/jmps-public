#include "Menus/Templates/GPopUpWindow.h"
#include "Common/Util/DEBUGOUT.h"
#include "Version/version.h"
#include "Version/buildinfo.h"
#include <cstring>


GPopUpWindow::GPopUpWindow(UINT8 winId):
	GWindowTemplate(winId),
	m_BackgroundRGB(255,255,255),
	m_BackgroundTextColorRGB(255,255,255),
	m_TextColorRGB(255,255,255),
	m_u16XOrg(0),
	m_u16YOrg(0),
	m_u16Width(0),
	m_u16Height(0),
	m_u16WindowWidth(320),
	m_u16WindowHeight(240)
{
   // build the base window to contain all other tags
   BuildContainerTag();
   BuildTextTag();
}

GPopUpWindow::~GPopUpWindow()
{
}

void GPopUpWindow::BuildContainerTag()
{
   GWindowTag* ContainerTag = new GWindowTag();

   ContainerTag->m_u8TagId 			= 0;
   ContainerTag->m_u16XOrg 			= 0;
   ContainerTag->m_u16YOrg 			= 0;
   ContainerTag->m_u16Width 		= m_u16WindowWidth;
   ContainerTag->m_u16Height 		= m_u16WindowHeight;
   ContainerTag->m_BackgroundRGB	= m_BackgroundRGB;
   ContainerTag->m_u8BorderSize		= 0;
   ContainerTag->m_BorderRGB.Red	= 0;
   ContainerTag->m_BorderRGB.Green	= 0;
   ContainerTag->m_BorderRGB.Blue	= 0;
   ContainerTag->m_bVisible			= true;
   ContainerTag->m_u8TagType		= 0;
  
   AddTag(ContainerTag);
}

void GPopUpWindow::BuildTextTag()
{
   GWindowTextTag *TextTag = new GWindowTextTag();

   TextTag->m_u8TagId = eTAG_1;

   TextTag->m_u16XOrg 				= m_u16XOrg;
   TextTag->m_u16YOrg 				= m_u16YOrg;
   TextTag->m_u16Width 				= m_u16Width;
   TextTag->m_u16Height 			= m_u16Height;
   TextTag->m_BackgroundRGB			= m_BackgroundTextColorRGB;
   TextTag->m_u8BorderSize 			= 0; // 1;
   TextTag->m_BorderRGB.Red 		= 0; // 0xFF;
   TextTag->m_BorderRGB.Green 		= 0;
   TextTag->m_BorderRGB.Blue 		= 0;
   TextTag->m_bVisible 				= true;
   TextTag->m_u8TagType 			= 1;  // text tag

   // text tag specific fields
   TextTag->m_u8FontId 				= 1;
   TextTag->m_FontRGB				= m_TextColorRGB;
   TextTag->m_u8Align 				= 0x05; // align center (horizontal) and middle (vertical)
   TextTag->m_u16PosX 				= 0;
   TextTag->m_u16PosY 				= 0;
   TextTag->m_u16TextLen			= m_sText.length();
   TextTag->m_pText					= new UINT8[m_sText.length()];
   memcpy(reinterpret_cast<void*>(TextTag->m_pText), reinterpret_cast<const void*>(m_sText.c_str()), m_sText.length());

   AddTag(TextTag);
}

bool GPopUpWindow::SetText(std::string sText)
{
	m_sText=sText;

	return UpdateTextTagString(eTAG_1, m_sText);
}

bool GPopUpWindow::SetBackgroundColor(UINT8 u8Red, UINT8 u8Green, UINT8 u8Blue)
{
	m_BackgroundRGB.Red   = u8Red;
	m_BackgroundRGB.Green = u8Green;
	m_BackgroundRGB.Blue  = u8Blue;

	GWindowTag *pTag = GetTag(eCONTAINER);

	if (pTag)
	{
		pTag->m_BackgroundRGB = m_BackgroundRGB;
		return true;
	}

	return false;
}

bool GPopUpWindow::SetWindowWidthAndHeight(UINT16 u16Width, UINT16 u16Height)
{
	m_u16WindowWidth  = u16Width;
	m_u16WindowHeight = u16Height;

	GWindowTag *pTag = GetTag(eCONTAINER);

	if (pTag)
	{
		pTag->m_u16Width  = m_u16WindowWidth;
		pTag->m_u16Height = m_u16WindowHeight;
		return true;
	}

	return false;
}

bool GPopUpWindow::SetBackgroundTextColor(UINT8 u8Red, UINT8 u8Green, UINT8 u8Blue)
{
	m_BackgroundTextColorRGB.Red   = u8Red;
	m_BackgroundTextColorRGB.Green = u8Green;
	m_BackgroundTextColorRGB.Blue  = u8Blue;

	GWindowTag *pTag = GetTag(eTAG_1);

	if (pTag)
	{
		pTag->m_BackgroundRGB = m_BackgroundTextColorRGB;
		return true;
	}

	return false;
}

bool GPopUpWindow::SetTextColor(UINT8 u8Red, UINT8 u8Green, UINT8 u8Blue)
{
	m_TextColorRGB.Red   = u8Red;
	m_TextColorRGB.Green = u8Green;
	m_TextColorRGB.Blue  = u8Blue;

	GWindowTextTag *pTag = (GWindowTextTag *)GetTag(eTAG_1);

	if (pTag)
	{
		pTag->m_FontRGB = m_TextColorRGB;
		return true;
	}

	return false;
}

bool GPopUpWindow::SetTextPosition(UINT16 u16XOrg, UINT16 u16YOrg)
{
	m_u16XOrg = u16XOrg;
	m_u16YOrg = u16YOrg;

	GWindowTag *pTag = GetTag(eTAG_1);

	if (pTag)
	{
		pTag->m_u16XOrg = m_u16XOrg;
		pTag->m_u16YOrg = m_u16YOrg;
		return true;
	}

	return false;
}

bool GPopUpWindow::SetTextWidthAndHeight(UINT16 u16Width, UINT16 u16Height)
{
	m_u16Width  = u16Width;
	m_u16Height = u16Height;

	GWindowTag *pTag = GetTag(eTAG_1);

	if (pTag)
	{
		pTag->m_u16Width  = m_u16Width;
		pTag->m_u16Height = m_u16Height;
		return true;
	}

	return false;
}
