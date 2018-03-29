#ifndef GPopUpWindow_h
#define GPopUpWindow_h 1

#include "Menus/Templates/GWindowTemplate.h"
#include <map>
#include <string>

#define POPUP_WIN_ID_DEFAULT 101

class GPopUpWindow : public GWindowTemplate
{
	public:

		enum eTAG_ID
		{
			eCONTAINER = 0,		// This tag is to set the background color.
			eTAG_1	   = 1		// This tag is to
		};

		GPopUpWindow( UINT8 winId=POPUP_WIN_ID_DEFAULT );
	    virtual    ~GPopUpWindow();

	    bool SetBackgroundColor(UINT8 u8Red, UINT8 u8Green, UINT8 u8Blue);
	    bool SetWindowWidthAndHeight(UINT16 u16Width, UINT16 u16Height);
	    bool SetBackgroundTextColor(UINT8 u8Red, UINT8 u8Green, UINT8 u8Blue);
	    bool SetTextColor(UINT8 u8Red, UINT8 u8Green, UINT8 u8Blue);
	    bool SetTextPosition(UINT16 u16XOrg, UINT16 u16YOrg);
	    bool SetTextWidthAndHeight(UINT16 u16Width, UINT16 u16Height);
	    bool SetText(std::string sText);

	private:

		void  BuildContainerTag();
		void  BuildTextTag();

		GTagRGB 	m_BackgroundRGB;
		GTagRGB 	m_BackgroundTextColorRGB;
		GTagRGB 	m_TextColorRGB;
		std::string m_sText;

		UINT16 m_u16XOrg;
		UINT16 m_u16YOrg;
		UINT16 m_u16Width;
		UINT16 m_u16Height;
		UINT16 m_u16WindowWidth;
		UINT16 m_u16WindowHeight;
};


#endif
