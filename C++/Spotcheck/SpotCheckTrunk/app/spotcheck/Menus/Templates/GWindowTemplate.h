/**
 * © 2012, 2013 Gilbarco Inc.
 * Confidential and Proprietary
 *
 * @file GWindowTemplate.h
 * @copyright © 2012, 2013 Gilbarco Inc. Confidential and Proprietary
 */

#ifndef GWindowTemplate_h
#define GWindowTemplate_h 1

#include "SPOT/SPOTProtocolInterface/GSPOTMessage.h"
#include <vector>
#include <string>

/**
 * Window and Resource Ids used by SPOTCheck application
 */
#define SPOTCHECK_BASE_WIN_ID        5
#define SPOTCHECK_BASE_SCR_RES_ID    0

enum eWindows_Ids
{
	eWinId_SoftKeyTemplate = SPOTCHECK_BASE_WIN_ID,
	eWinId_MainMenu        = SPOTCHECK_BASE_WIN_ID,
	eWinId_ExitScreen,
	eWinId_KeypadMatrix,
	eWinId_SoftKeyMatrix,
	eWinId_AuxKeyMatrix,
	eWinId_SecureAudio,

	// The following ID are only used to set an unused private variable,
	// the Window ID differentiation mechanism will not be implemented
	eWinId_DeviceMenu,
	eWinId_BarcodeScannerMenu,
	eWinId_CardReaderMenu,
	eWinId_ContactlessMenu,
	eWinId_PrinterMenu,
	eWinId_KeypadMenu,
	eWinId_SoftKeyMenu,
	eWinId_AuxKeyMenu,
	eWinId_DisplayMenu,
	eWinId_SecureAudioMenu,
	eWinId_FeatureConnectorMenu,
	eWinId_TrindMenu,
	eWinId_LastMenu = eWinId_TrindMenu,
	eWinId_BankNoteAcceptorMenu // TODO: move this above eWinId_LastMenu when ready
};


struct GTagRGB 
{
   UINT8 Red;
   UINT8 Green;
   UINT8 Blue;

   GTagRGB(UINT8 R, UINT8 G, UINT8 B)
   {
	   Red   = R;
	   Green = G;
	   Blue  = B;
   };

   GTagRGB()
   {
	   Red   = 0;
	   Green = 0;
	   Blue  = 0;
   };
};

const GTagRGB RGB_BLACK, RGB_WHITE(255, 255, 255), RGB_LT_GRAY(208, 208, 208), RGB_GREEN(0,192,0);

struct GWindowTag
{
   virtual ~GWindowTag()
   {

   };

   UINT8 m_u8TagId;
   UINT16 m_u16XOrg;
   UINT16 m_u16YOrg;
   UINT16 m_u16Width;
   UINT16 m_u16Height;
   GTagRGB m_BackgroundRGB;
   UINT8 m_u8BorderSize;
   GTagRGB m_BorderRGB;
   bool m_bVisible;
   UINT8 m_u8TagType;
};

struct GWindowTextTag : public GWindowTag
{
   ~GWindowTextTag()
   {
      if (m_pText)
      {
         delete[] m_pText;
      }
   };

   UINT8 m_u8FontId;
   GTagRGB m_FontRGB;
   UINT8 m_u8Align;
   UINT16 m_u16PosX;
   UINT16 m_u16PosY;
   UINT16 m_u16TextLen;
   UINT8* m_pText;
};

struct GWindowGraphicTag : public GWindowTag
{
   UINT8  m_u8ImageId;
   UINT8  m_u8Align;  //fixed to 0
   UINT16 m_u16PosX;
   UINT16 m_u16PosY;
};

class GWindowTemplate
{
   public:
                         GWindowTemplate();
                         GWindowTemplate(UINT8 WinId);
      virtual            ~GWindowTemplate();

      GSPOTMessage*      GetCreateMessage(UINT8 u8WinID = 0);
      GSPOTMessage*      GetDestroyMessage(UINT8 u8WinID = 0);
      GSPOTMessage*      GetShowMessage(UINT8 u8WinID = 0);
      GSPOTMessage*      GetHideMessage(UINT8 u8WinID = 0);
      GSPOTMessage*      GetUpdateTextMessage(bool blForNewFreshWindow);
      GSPOTMessage*      GetUpdateTextClearedMessage();
      GSPOTMessage*      GetUpdateTextTagMessage(UINT8 TagId);
      bool               AddTag(GWindowTag* tag);
      bool               UpdateTextTagString(UINT8 TagId, std::string NewString);
      GWindowTag*        GetTag(UINT8 TagId);

    inline static char*  GetEmptyString();
    inline static void   GetEmptyString(UINT8**);
    inline static bool   isBlankChar(char c);
    inline static bool   isBlankString(std::string s);
    inline static bool   isBlankString(const UINT8* pu8String, UINT uLength);
           static bool   isBlankString(const char* pcString, UINT uLength);


   protected:
      void               AppendTagToCreateMessage(GWindowTag* tag, GSPOTMessage* msg);

      static char              mst_arycEmpty[2];

      UINT8                    m_u8WinId;
      std::vector<GWindowTag*> m_TagList;
};


inline char* GWindowTemplate::GetEmptyString()
{
	return mst_arycEmpty;
}

inline void GWindowTemplate::GetEmptyString(UINT8** ppu8Dest)
{
	*ppu8Dest = reinterpret_cast<UINT8*>(mst_arycEmpty);
}

inline bool GWindowTemplate::isBlankChar(char c)
{
	if( c == ' ' || c == '\t' || c == '\n' || c == '\r' || c == '\v' ||  c == '\b' )
		return true;
	else
		return false;
}

inline bool GWindowTemplate::isBlankString(std::string s)
{
	return isBlankString(s.data(), s.length());
}

inline bool GWindowTemplate::isBlankString(const UINT8* pu8String, UINT uLength)
{
	return isBlankString( reinterpret_cast<const char*>(pu8String), uLength );
}


#endif
