#include "Menus/Templates/GWindowTemplate.h"
#include "SPOT/SPOTProtocolInterface/GSPOTDefs.h"
#include <string.h>

using namespace GSpotDefs;

char GWindowTemplate::mst_arycEmpty[2] = {'\0', '\0'};

GWindowTemplate::GWindowTemplate()
  :  m_u8WinId(0)
{
}

GWindowTemplate::GWindowTemplate(UINT8 WinId)
  :  m_u8WinId(WinId)
{
}

GWindowTemplate::~GWindowTemplate()
{
	// go through iterator list and destroy tags
	while (!m_TagList.empty())
	{
		GWindowTag* pTag = m_TagList.back();
		m_TagList.pop_back();
		delete pTag;
	}
}

GSPOTMessage* GWindowTemplate::GetCreateMessage(UINT8 u8WinID /* = 0 */)
{
   if (m_TagList.size() > 0)
   {
      GSPOTMessage* pMsg = new GSPOTMessage(eSPOT_PP_CATEGORY, eSPOT_PP_DISPLAY, eSPOT_PP_DY_CREATE_WINDOW);

      if(u8WinID == 0) // parameter is default one
         u8WinID = m_u8WinId;

      // put the winid and the num. of tags in the data
      pMsg->AppendByte(u8WinID);
      pMsg->AppendByte((UINT8)m_TagList.size());

      std::vector<GWindowTag*>::iterator it(m_TagList.begin());
      for ( ; it != m_TagList.end(); ++it)
      {
         AppendTagToCreateMessage((*it), pMsg);
      }
      return pMsg;
   }
   return NULL;
}

GSPOTMessage* GWindowTemplate::GetDestroyMessage(UINT8 u8WinID /* = 0 */)
{
	GSPOTMessage* pMsg = new GSPOTMessage(eSPOT_PP_CATEGORY, eSPOT_PP_DISPLAY, eSPOT_PP_DY_DESTROY_WINDOW);

    if(u8WinID == 0) // parameter is default one
       u8WinID = m_u8WinId;

    // put the winid and the num. of tags in the data
	pMsg->AppendByte(u8WinID);
	pMsg->AppendByte(1); // 1 tags
	pMsg->AppendByte(0); // only references the main container tag

    return pMsg;
}

GSPOTMessage* GWindowTemplate::GetShowMessage(UINT8 u8WinID /* = 0 */)
{
	if (m_TagList.size() > 0)
	{
		GSPOTMessage* pMsg = new GSPOTMessage(eSPOT_PP_CATEGORY, eSPOT_PP_DISPLAY, eSPOT_PP_DY_SHOW_WINDOW);

		if(u8WinID == 0) // parameter is default one
			u8WinID = m_u8WinId;

		// put the winid and the num. of tags in the data
		pMsg->AppendByte(u8WinID);
		pMsg->AppendByte(1); // only container tag
		pMsg->AppendByte(0);
		pMsg->AppendByte(0);

		return pMsg;
	}

	return NULL;
}

GSPOTMessage* GWindowTemplate::GetHideMessage(UINT8 u8WinID /* = 0 */)
{
	if (m_TagList.size() > 0)
	{
		GSPOTMessage* pMsg = new GSPOTMessage(eSPOT_PP_CATEGORY, eSPOT_PP_DISPLAY, eSPOT_PP_DY_HIDE_WINDOW);

		if(u8WinID == 0) // parameter is default one
			u8WinID = m_u8WinId;

		// put the winid and the num. of tags in the data
		pMsg->AppendByte(u8WinID);
		pMsg->AppendByte(1); // only container tag
		pMsg->AppendByte(0);

		return pMsg;
	}

	return NULL;
}

void GWindowTemplate::AppendTagToCreateMessage(GWindowTag* pTag, GSPOTMessage* pMsg)
{
   // add fields common to all tag types
   pMsg->AppendByte(pTag->m_u8TagId);
   pMsg->AppendWord(pTag->m_u16XOrg);
   pMsg->AppendWord(pTag->m_u16YOrg);
   pMsg->AppendWord(pTag->m_u16Width);
   pMsg->AppendWord(pTag->m_u16Height);
   pMsg->AppendByte(pTag->m_BackgroundRGB.Red);
   pMsg->AppendByte(pTag->m_BackgroundRGB.Green);
   pMsg->AppendByte(pTag->m_BackgroundRGB.Blue);
   pMsg->AppendByte(pTag->m_u8BorderSize);
   pMsg->AppendByte(pTag->m_BorderRGB.Red);
   pMsg->AppendByte(pTag->m_BorderRGB.Green);
   pMsg->AppendByte(pTag->m_BorderRGB.Blue);
   if (pTag->m_bVisible)
      pMsg->AppendByte(0x01);
   else
      pMsg->AppendByte(0x00);
   pMsg->AppendByte(pTag->m_u8TagType);

   // handle non-container tags
   if (pTag->m_u8TagType == 0x01) // text tag
   {
      GWindowTextTag* pTextTag = (GWindowTextTag*)pTag;
      pMsg->AppendByte(pTextTag->m_u8FontId);
      pMsg->AppendByte(pTextTag->m_FontRGB.Red);
      pMsg->AppendByte(pTextTag->m_FontRGB.Green);
      pMsg->AppendByte(pTextTag->m_FontRGB.Blue);
      pMsg->AppendByte(pTextTag->m_u8Align);
      pMsg->AppendWord(pTextTag->m_u16PosX);
      pMsg->AppendWord(pTextTag->m_u16PosY);
      pMsg->AppendWord(pTextTag->m_u16TextLen);
      pMsg->Append(pTextTag->m_pText, pTextTag->m_u16TextLen);
   }
   else if (pTag->m_u8TagType == 0x02) // graphic tag
   {
      GWindowGraphicTag* pGraphTag = (GWindowGraphicTag*)pTag;
      pMsg->AppendByte(pGraphTag->m_u8ImageId);
      pMsg->AppendByte(0);  //Align (fixed at 0)
      pMsg->AppendWord(pGraphTag->m_u16PosX);
      pMsg->AppendWord(pGraphTag->m_u16PosY);
   }
}

GSPOTMessage* GWindowTemplate::GetUpdateTextMessage(bool blForNewFreshWindow)
{
	if (m_TagList.size() > 0)
	{
		GSPOTMessage* pMsg = new GSPOTMessage(eSPOT_PP_CATEGORY, eSPOT_PP_DISPLAY, eSPOT_PP_DY_SET_WINDOW_TEXT);

		// put the winid and the num. of tags in the data
		pMsg->AppendByte(m_u8WinId);
		pMsg->AppendByte(0); // number of tags to be completed later

		UINT uTagQty = 0;
		std::vector<GWindowTag*>::iterator  it(m_TagList.begin());

		for ( ; it != m_TagList.end(); ++it)
		{
			// we don't include the container tag

			if ((*it)->m_u8TagType == 0x01) // only investigate text segments
			{
				GWindowTextTag* pTag = (GWindowTextTag*)*it;
				if(!blForNewFreshWindow || !isBlankString(pTag->m_pText, pTag->m_u16TextLen))
				{
					pMsg->AppendByte(pTag->m_u8TagId);
					pMsg->AppendByte(pTag->m_u8Align);
					pMsg->AppendWord(pTag->m_u16PosX);
					pMsg->AppendWord(pTag->m_u16PosY);
					pMsg->AppendWord(pTag->m_u16TextLen);
					pMsg->Append(pTag->m_pText, pTag->m_u16TextLen);
					uTagQty++;
				}
			}
		}

		pMsg->PokeByte(1, uTagQty); // number of tags completed
		return pMsg;
	}

	return NULL;
}

GSPOTMessage* GWindowTemplate::GetUpdateTextTagMessage(UINT8 TagId)
{
	if (m_TagList.size() > 0)
	{
		GSPOTMessage* pMsg = new GSPOTMessage(eSPOT_PP_CATEGORY, eSPOT_PP_DISPLAY, eSPOT_PP_DY_SET_WINDOW_TEXT);
		// put the winid and the num. of tags in the data
		pMsg->AppendByte(m_u8WinId);
		pMsg->AppendByte(0); // number of tags to be completed later

		UINT uTagQty = 0;
		std::vector<GWindowTag*>::iterator  it(m_TagList.begin());

		for ( ; it != m_TagList.end(); ++it)
		{
			// we don't include the container tag

			if ((*it)->m_u8TagType == 0x01) // only investigate text segments
			{
				GWindowTextTag* pTag = (GWindowTextTag*)*it;
				if( pTag->m_u8TagId == TagId )
				{
					pMsg->AppendByte(pTag->m_u8TagId);
					pMsg->AppendByte(pTag->m_u8Align);
					pMsg->AppendWord(pTag->m_u16PosX);
					pMsg->AppendWord(pTag->m_u16PosY);
					pMsg->AppendWord(pTag->m_u16TextLen);
					pMsg->Append(pTag->m_pText, pTag->m_u16TextLen);
					uTagQty++;
					break;
				}
			}
		}

		pMsg->PokeByte(1, uTagQty); // number of tags completed
		return pMsg;
	}

		return NULL;
	}



GSPOTMessage* GWindowTemplate::GetUpdateTextClearedMessage()
{
	if (m_TagList.size() > 0)
	{
		GSPOTMessage* pMsg = new GSPOTMessage(eSPOT_PP_CATEGORY, eSPOT_PP_DISPLAY, eSPOT_PP_DY_SET_WINDOW_TEXT);

		// put the winid and the num. of tags in the data
		pMsg->AppendByte(m_u8WinId);
		pMsg->AppendByte(0); // number of tags to be completed later

		UINT uTagQty = 0;
		std::vector<GWindowTag*>::iterator  it(m_TagList.begin());

		for ( ; it != m_TagList.end(); ++it)
		{
			// we don't include the container tag

			if ((*it)->m_u8TagType == 0x01) // only investigate text segments
			{
				GWindowTextTag* pTag = (GWindowTextTag*)*it;
				if(!isBlankString(pTag->m_pText, pTag->m_u16TextLen))
				{
					pMsg->AppendByte(pTag->m_u8TagId);
					pMsg->AppendByte(pTag->m_u8Align);
					pMsg->AppendWord(pTag->m_u16PosX);
					pMsg->AppendWord(pTag->m_u16PosY);
					pMsg->AppendWord(1); // clean the used text tag.
					pMsg->AppendByte(' ');
					uTagQty++;
				}
			}
		}

		pMsg->PokeByte(1, uTagQty); // number of tags completed
		return pMsg;
	}

	return NULL;
}

bool GWindowTemplate::AddTag(GWindowTag* pTag)
{
   if (pTag)
   {
      m_TagList.push_back(pTag);
      return true;
   }
   return false;
}

bool GWindowTemplate::UpdateTextTagString(UINT8 TagId, std::string NewString)
{
   std::vector<GWindowTag*>::iterator it(m_TagList.begin());
   for ( ; it != m_TagList.end(); ++it)
   {
      // make sure we have the right tag id and that it is a text tag
      if ( ((*it)->m_u8TagId == TagId) && ((*it)->m_u8TagType == 0x01) )
      {
         GWindowTextTag *pTag = (GWindowTextTag*)*it;
         // clean up the old string
         delete[] pTag->m_pText;
         pTag->m_u16TextLen = NewString.length();
         pTag->m_pText = new UINT8[NewString.length()];
         memcpy(pTag->m_pText, NewString.data(), NewString.length());
         return true;
      }
   }

   return false;
}

GWindowTag* GWindowTemplate::GetTag(UINT8 TagId)
{
	std::vector<GWindowTag*>::iterator it(m_TagList.begin());
	for (; it != m_TagList.end(); ++it)
	{
		// make sure we have the right tag id and that it is a text tag
		if( ((*it)->m_u8TagId == TagId) )
		{
			return ((GWindowTextTag*) *it);
		}
	}

	return NULL;
}



/**
 *
 * STATIC METHODS
 *
 */


bool GWindowTemplate::isBlankString(const char* pcString, UINT uLength)
{
	if(pcString == NULL || uLength == 0)
		return true; // NULL or empty strings are considered "blank" for our purpose

	UINT u = 0;

	while( u < uLength && isBlankChar(pcString[u]) )
		u++;

	if(u >= uLength)
		return true;  // All the scanned characters in string are "blank"

	if(pcString[u] == '\0')
		return true; // End of string found while all the scanned characters were "blank"

	return false;
}
