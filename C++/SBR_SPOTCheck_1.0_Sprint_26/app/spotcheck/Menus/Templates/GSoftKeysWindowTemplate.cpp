#include "Menus/Templates/GSoftKeysWindowTemplate.h"
#include "Common/Util/DEBUGOUT.h"
#include "Version/version.h"
#include "Version/buildinfo.h"
#include <cstring>

GSoftKeysWindowTemplate* GSoftKeysWindowTemplate::mst_pSoleInstance(NULL);

GSoftKeysWindowTemplate* GSoftKeysWindowTemplate::GetSoleInstance()
{
   if (!mst_pSoleInstance)
   {
      mst_pSoleInstance = new GSoftKeysWindowTemplate();
   }
   return mst_pSoleInstance;
}

void GSoftKeysWindowTemplate::DestroySoleInstance()
{
   if (mst_pSoleInstance)
   {
      delete mst_pSoleInstance;
      mst_pSoleInstance = NULL;
   }
}

GSoftKeysWindowTemplate::GSoftKeysWindowTemplate()
{
   m_u8WinId = eWinId_SoftKeyTemplate;

   InitializeTagList();
}

GSoftKeysWindowTemplate::~GSoftKeysWindowTemplate()
{
}

void GSoftKeysWindowTemplate::InitializeTagList()
{
	   // initialize the map with the tag id and labels
	   m_TagList.clear();

	   eTAG_ID new_seg = eSOFT_L1;
	   for ( int i=0; i < 17; i++)
	   {
	      if (new_seg != eSEGMENT_9)
	         m_TagMap[new_seg] = std::string("");
	      else
	      {
	         std::string sFoot = std::string("SPOTCheck ").append(gszVersion);
	         sFoot.append(" rev. ");
	         m_TagMap[new_seg] = sFoot.append(gBuildNumber);
	      }
	      new_seg = static_cast<eTAG_ID>((static_cast<int>(new_seg) + 1));
	   }

	   // build the base window to contain all other tags
	   BuildContainerTag();

	   // build all the tags for the soft key and center sections
	   std::map<eTAG_ID,std::string>::iterator it(m_TagMap.begin());
	   for ( ; it != m_TagMap.end(); it++)
	   {
	      BuildTextTag((*it).second, (*it).first);
	   }
}

void GSoftKeysWindowTemplate::ClearLabels()
{
   std::map<eTAG_ID,std::string>::iterator it(m_TagMap.begin());
   for ( ; it != m_TagMap.end(); it++)
   {
      // segment nine is reserved for the spotcheck version info so don't clear it
      if ((*it).first != eSEGMENT_9)
      {
         // .first is the tag id, second is the text string for that tag
         (*it).second = "";
         UpdateTextTagString((UINT8)(*it).first, (*it).second);	 
      }
   }
}

void GSoftKeysWindowTemplate::SetLabels(tStringTagMap& NewMap, bool ClearAllLabels)
{
   // clear all labels if requested
   if (ClearAllLabels)
      ClearLabels();

   // go through the new map and assign the new strings to the member map
   std::map<eTAG_ID,std::string>::iterator it(NewMap.begin());
   for ( ; it != NewMap.end(); it++)
   {
      // segment nine is reserved for the spotcheck version info so don't change it
      if ((*it).first != eSEGMENT_9)
      {
	 // .first is the tag id, second is the text string for that tag
	 m_TagMap[(*it).first] = (*it).second;
         UpdateTextTagString((UINT8)(*it).first, (*it).second);
      }
   }
}

void GSoftKeysWindowTemplate::BuildContainerTag()
{
   GWindowTag* ContainerTag = new GWindowTag();

   ContainerTag->m_u8TagId = 0;
   ContainerTag->m_u16XOrg = 0;
   ContainerTag->m_u16YOrg = 0;
   ContainerTag->m_u16Width = 320;
   ContainerTag->m_u16Height = 240;
   ContainerTag->m_BackgroundRGB.Red = 255;
   ContainerTag->m_BackgroundRGB.Green = 255;
   ContainerTag->m_BackgroundRGB.Blue = 255;
   ContainerTag->m_u8BorderSize = 0;
   ContainerTag->m_BorderRGB.Red = 0;
   ContainerTag->m_BorderRGB.Green = 0;
   ContainerTag->m_BorderRGB.Blue = 0;
   ContainerTag->m_bVisible = false;//true;
   ContainerTag->m_u8TagType = 0;
  
   AddTag(ContainerTag);
}

void GSoftKeysWindowTemplate::BuildTextTag(std::string text, eTAG_ID TagId)
{
   GWindowTextTag *TextTag = new GWindowTextTag();

   TextTag->m_u8TagId = TagId;
   GetTagPosition(TagId, TextTag->m_u16XOrg, TextTag->m_u16YOrg);
   GetTagSize(TagId, TextTag->m_u16Width, TextTag->m_u16Height);
   TextTag->m_BackgroundRGB.Red = 255;
   TextTag->m_BackgroundRGB.Green = 255;
   TextTag->m_BackgroundRGB.Blue = 255;
   TextTag->m_u8BorderSize = 0; // 1;
   TextTag->m_BorderRGB.Red = 0; // 0xFF;
   TextTag->m_BorderRGB.Green = 0;
   TextTag->m_BorderRGB.Blue = 0;
   TextTag->m_bVisible = true;
   TextTag->m_u8TagType = 1;  // text tag
   // text tag specific fields
   TextTag->m_u8FontId = 1;
   GetFontColor(TagId, &TextTag->m_FontRGB);
   TextTag->m_u8Align = GetTextAlignment(TagId);
   TextTag->m_u16PosX = 0;
   TextTag->m_u16PosY = 0;
   TextTag->m_u16TextLen = text.length();
   TextTag->m_pText = new UINT8[text.length()];
   memcpy(reinterpret_cast<void*>(TextTag->m_pText), reinterpret_cast<const void*>(text.data()), text.length());

   AddTag(TextTag);
}

void GSoftKeysWindowTemplate::GetTagPosition(eTAG_ID TagId, UINT16& XOrg, UINT16& YOrg)
{
   switch (TagId)
   {
      case eSOFT_L1:
      case eSOFT_L2:
      case eSOFT_L3:
      case eSOFT_L4:
         XOrg = 0;
	 YOrg = (TagId - 1) * 60;
         break;

      case eSOFT_R1:
      case eSOFT_R2:
      case eSOFT_R3:
      case eSOFT_R4:
         XOrg = 260;
	 YOrg = (TagId % eSOFT_R1) * 60;
         break;

      case eSEGMENT_1:
      case eSEGMENT_2:
      case eSEGMENT_3:
      case eSEGMENT_4:
      case eSEGMENT_5:
      case eSEGMENT_6:
      case eSEGMENT_7:
      case eSEGMENT_8:
         XOrg = 60;
	 YOrg = (TagId % eSEGMENT_1) * 26 + 6;
         break;
      
      case eSEGMENT_9:
         XOrg = 60;
	 YOrg = (TagId % eSEGMENT_1) * 26 + 6;
         break;
   }
}

void GSoftKeysWindowTemplate::GetTagSize(eTAG_ID TagId, UINT16& Width, UINT16& Height)
{
   switch (TagId)
   {
      case eSOFT_L1:
      case eSOFT_L2:
      case eSOFT_L3:
      case eSOFT_L4:
      case eSOFT_R1:
      case eSOFT_R2:
      case eSOFT_R3:
      case eSOFT_R4:
         Width = 60;
         Height = 60;
         break;

      case eSEGMENT_1:
      case eSEGMENT_2:
      case eSEGMENT_3:
      case eSEGMENT_4:
      case eSEGMENT_5:
      case eSEGMENT_6:
      case eSEGMENT_7:
      case eSEGMENT_8:
	 Width = 200;
	 Height = 20;
	 break;

      case eSEGMENT_9:
         Width = 180;
	 Height = 20;
         break;
   }
}

UINT8 GSoftKeysWindowTemplate::GetTextAlignment(eTAG_ID TagId)
{
   UINT8 TextAlign = 0x00;

   switch (TagId)
   {
      case eSOFT_L1:
      case eSOFT_L2:
      case eSOFT_L3:
      case eSOFT_L4:
         TextAlign = 0x04; // align left and middle (vertical)
         break;

      case eSOFT_R1:
      case eSOFT_R2:
      case eSOFT_R3:
      case eSOFT_R4:
         TextAlign = 0x06; // align right and middle (vertical)
         break;

      case eSEGMENT_1:
      case eSEGMENT_2:
      case eSEGMENT_3:
      case eSEGMENT_4:
      case eSEGMENT_5:
      case eSEGMENT_6:
      case eSEGMENT_7:
      case eSEGMENT_8:
      case eSEGMENT_9:
         TextAlign = 0x05; // align center (horizontal) and middle (vertical)
         break;
   }
   return TextAlign;
}

void GSoftKeysWindowTemplate::GetFontColor(eTAG_ID TagId, GTagRGB* RGB)
{
   switch (TagId)
   {
      // soft key text is blue
      case eSOFT_L1:
      case eSOFT_L2:
      case eSOFT_L3:
      case eSOFT_L4:
      case eSOFT_R1:
      case eSOFT_R2:
      case eSOFT_R3:
      case eSOFT_R4:
         RGB->Red = 0x00;
	 RGB->Green = 0x00;
	 RGB->Blue = 0xFF;
         break;

      // the top 8 segments text is black
      case eSEGMENT_1:
      case eSEGMENT_2:
      case eSEGMENT_3:
      case eSEGMENT_4:
      case eSEGMENT_5:
      case eSEGMENT_6:
      case eSEGMENT_7:
      case eSEGMENT_8:
         RGB->Red = 0x00;
	 RGB->Green = 0x00;
	 RGB->Blue = 0x00;
	 break;

      // the bottom segment (version) has green text
      case eSEGMENT_9:
         RGB->Red = 0x00;
	 RGB->Green = 0x7f;
	 RGB->Blue = 0x00;
	 break;
   }
}


