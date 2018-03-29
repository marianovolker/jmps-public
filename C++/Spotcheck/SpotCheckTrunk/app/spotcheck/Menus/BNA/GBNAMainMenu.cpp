#include "Common/Util/DEBUGOUT.h"
#include "Menus/BNA/GBNAMainMenu.h"
#include "Menus/GMenuManager.h"
#include "SPOT/SPOTProtocolInterface/GSPOTDefs.h"
#include "GSystem.h"
#include "GTypes.h"

using namespace GSpotDefs;

GBNAMainMenu::GBNAMainMenu()
{
	m_iWinID = eWinId_BankNoteAcceptorMenu;

	//TODO: Eliminate this when repository is working OK
	GetTagStrings(m_MenuLabelsMap);
}

GBNAMainMenu::~GBNAMainMenu()
{
}

bool GBNAMainMenu::DrawMenu()
{
	return GAbsMenu::DrawMenu();
}

void GBNAMainMenu::PostDrawingAction()
{
	EnableSoftKeys();
	GetBNAFirmwareVersion();
	EnableBNA();
}

bool GBNAMainMenu::HandleSPOTMessage(GSPOTMessage* pMsg)
{
   UINT8 AppId = pMsg->GetAppId();
   UINT8 CmdCode = pMsg->GetCommandCode();
   UINT8 SubCmdCode = pMsg->GetSubCommandCode();
   UINT16 data_len = pMsg->GetDataLen();
   UINT8* data = NULL;

   data = new UINT8[data_len];
   pMsg->GetData(data, data_len);

   switch(AppId)
   {
      case eSPOT_OPT_ANSWER:
         switch (CmdCode)
         {
            case eSPOT_OPT_SYSTEM:
               switch (SubCmdCode)
               {
                  case eSPOT_OPT_SYS_BNA_STATUS:
                     HandleBNAStatus(data,data_len);
                     break;
               };
               break;

            case eSPOT_OPT_BNA:
               switch (SubCmdCode)
               {
                  case eSPOT_OPT_BNA_VERSIONQUERY:
                     HandleBNAVersion(data,data_len);
                     break;
               };
               break;
         };
         break;

      default:
         break;
   };

   delete[] data;
   return true;
}

void GBNAMainMenu::HandleSoftKeyEvent(UINT8 KeyCode)
{
   switch(KeyCode)
   {
      case eL4:  // L4 - back to previous menu
         DisableBNA();
         m_pManager->QueueMenuPop();
         break;

      case eL1:  // L1
      case eL2:  // L2
      case eL3:  // L3
      case eR1:  // R1
      case eR2:  // R2
      case eR3:  // R3
      case eR4:  // R4
         EnableSoftKeys();
	 break;
   };
}

void GBNAMainMenu::GetBNAFirmwareVersion()
{
   GSPOTMessage* msg = new GSPOTMessage(eSPOT_OPT_CATEGORY, eSPOT_OPT_BNA, eSPOT_OPT_BNA_VERSIONQUERY);

   if (msg)
   {
      m_pManager->SendCommandToSPOT(msg);
      delete msg;
   }
}

void GBNAMainMenu::HandleBNAVersion(UINT8* Data, UINT16 DataLen)
{
   std::string ver_text("");
   GSPOTMessage* msg = NULL;

   // first check to see if we have an ack or error
   if (Data[0] != 0x00)
   {
      ver_text.assign("ERROR");
   }
   else
   {
      ver_text.assign(reinterpret_cast<const char*>(&Data[1]),25); // field is always 50 bytes, but 25 should be enough (limited space)
   }
   m_pWinTemplate->UpdateTextTagString((UINT8)GSoftKeysWindowTemplate::eSEGMENT_4, ver_text);

   msg = m_pWinTemplate->GetUpdateTextMessage(true);
   if (msg)
   {
      m_pManager->SendCommandToSPOT(msg);
      delete msg;
   }
}

void GBNAMainMenu::EnableBNA()
{
   GSPOTMessage* msg = new GSPOTMessage(eSPOT_OPT_CATEGORY, eSPOT_OPT_BNA, eSPOT_OPT_BNA_ENABLE);
   if (msg)
   {
	   msg->AppendWord(0xFF);  // enable all bill types
	   msg->AppendByte(0x00);  // normal security level
	   m_pManager->SendCommandToSPOT(msg);
      delete msg;
   }
}

void GBNAMainMenu::DisableBNA()
{
   GSPOTMessage* msg = new GSPOTMessage(eSPOT_OPT_CATEGORY, eSPOT_OPT_BNA, eSPOT_OPT_BNA_DISABLE);
   if (msg)
   {
      m_pManager->SendCommandToSPOT(msg);
      delete msg;
   }
}

void GBNAMainMenu::ConfirmRead()
{
   GSPOTMessage* msg = new GSPOTMessage(eSPOT_OPT_CATEGORY, eSPOT_OPT_BNA, eSPOT_OPT_BNA_READCONFIRM);
   if (msg)
   {
      m_pManager->SendCommandToSPOT(msg);
      delete msg;
   }
}

void GBNAMainMenu::HandleBNAStatus(UINT8* Data, UINT16 DataLen)
{
   std::string text("");
   UINT8 u8AckCode = Data[0];
   UINT8 u8Status = Data[1];
   UINT8 u8ExtStatus = Data[2];
   UINT8 u8Error = Data[3];
   UINT8 u8BillIndex = Data[7];

   // first check to see if we have an ack or error
   if ((u8AckCode == 0x00) && (u8Status == 0x00))
   {
      // make sure this isn't just a status update
      if ((u8ExtStatus == 0x00) && (u8BillIndex != 0x00))
      {
         GSPOTMessage* msg = NULL;
         bool bEscrowed = false;

          // we don't handle the extended enable (0x02) yet (not valid for COPEC)
         if ((u8ExtStatus == 0x00) || (u8ExtStatus == 0x01))
         {
            text.assign(GetBillName(u8BillIndex));

            if (u8ExtStatus == 0x01) // if BNA is enabled (ext status = 1) then bill was just read, not escrowed
            {
               text.append(" note read");
            }
            else // BNA is disabled, meaning bill was escrowed
            {
               text.append(" note escrowed");
               bEscrowed = true;
            }

            // update the appropriate segment with the read/escrow info 
            m_pWinTemplate->UpdateTextTagString((UINT8)GSoftKeysWindowTemplate::eSEGMENT_6, text);
            msg = m_pWinTemplate->GetUpdateTextMessage(true);
            if (msg)
            {
               m_pManager->SendCommandToSPOT(msg);
               delete msg;
            }

            // we only confirm the read on escrow, which happens automatically for COPEC
            if (bEscrowed)
               ConfirmRead();
	 }
      }
   }
   else
   {
      text.assign("Response Error");
   }

   if (u8ExtStatus == 0x00) // check the extended status byte and re-enable the note acceptor if not enabled
      EnableBNA();
}

std::string GBNAMainMenu::GetBillName(UINT8 index)
{
   switch (index)
   {
      case 0:
	 return std::string("No");
      case 1:
	 return std::string("1000");
      case 2:
	 return std::string("2000");
      case 3:
	 return std::string("5000");
      case 4:
	 return std::string("10000");
      case 5:
	 return std::string("20000");
   }
   return std::string("");
}


/**
 *
 * STATIC METHODS
 *
 */


int GBNAMainMenu::GetTagStrings(GSoftKeysWindowTemplate::tStringTagMap & rMap)
{
	// initialize map of tag-id/label pairs for initial window creation
	rMap[GSoftKeysWindowTemplate::eSEGMENT_1] = std::string("Note Acceptor Diagnostics");
	rMap[GSoftKeysWindowTemplate::eSOFT_L4] = std::string(" Previous\n Screen");
	rMap[GSoftKeysWindowTemplate::eSEGMENT_3] = std::string("Note Acceptor F/W:");
	rMap[GSoftKeysWindowTemplate::eSEGMENT_6] = std::string("Insert Note");

	return 4;
}

