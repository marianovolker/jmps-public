#include "Menus/MainMenus/GMainMenu.h"
#include "GSystem.h"
#include "GTypes.h"
#include "SPOT/SystemInformation/GGlobalSystemInformation.h"
#include "Menus/Templates/GPopUpWindow.h"
#include "Common/Util/DEBUGOUT.h"
#include "Menus/MainMenus/GDeviceMenuPage1.h"
#include "Menus/GMenuManager.h"

using namespace GSpotDefs;
	
GMainMenu::GMainMenu() : m_eStatus(eInitialLocked)
{
	m_iWinID = eWinId_MainMenu;

	GetTagStrings(m_eStatus, m_MenuLabelsMap);
}

GMainMenu::~GMainMenu()
{
}


bool GMainMenu::DrawMenu(bool blNotstackAction /* =false*/)
 {
	if(!blNotstackAction )
	{
		GetTagStrings(m_eStatus,m_MenuLabelsMap);
	}
 	return GAbsMenu::DrawMenu();
 }


bool GMainMenu::HandleSPOTMessage(GSPOTMessage* pMsg)
{
   UINT8 AppId = pMsg->GetAppId();
   UINT8 CmdCode = pMsg->GetCommandCode();
   UINT8 SubCmdCode = pMsg->GetSubCommandCode();

   switch(AppId)
   {
      case eSPOT_PP_ANSWER:
         switch (CmdCode)
         {
            case eSPOT_PP_SYSTEM:
               switch (SubCmdCode)
               {
                  case eSPOT_PP_SYS_STATUS_INFO:
                     LOG(eUnCategorized,eError,"GMainMenu::HandleSPOTMessage - got eSPOT_PP_SYS_STATUS_INFO\n");
                     break;
                  case eSPOT_PP_SYS_HWR_INFO:
                     LOG(eUnCategorized,eError,"GMainMenu::HandleSPOTMessage - got eSPOT_PP_SYS_HWR_INFO\n");
                     break;
                  case eSPOT_PP_SYS_SWR_INFO:
                     LOG(eUnCategorized,eError,"GMainMenu::HandleSPOTMessage - got eSPOT_PP_SYS_SWR_INFO\n");
                     break;
               };
               break;
         };
         break;

      case eSPOT_OPT_ANSWER:
         switch (CmdCode)
         {
            case eSPOT_OPT_SYSTEM:
               switch (SubCmdCode)
               {
                  case eSPOT_OPT_SYS_STATUS_INFO:
                     LOG(eUnCategorized,eWarning,"GMainMenu::HandleSPOTMessage - got eSPOT_OPT_SYS_STATUS_INFO\n");
                     break;

                  case eSPOT_OPT_SYS_HWR_INFO:
                     LOG(eUnCategorized,eWarning,"GMainMenu::HandleSPOTMessage - got eSPOT_OPT_SYS_HWR_INFO\n");
                     break;

                  case eSPOT_OPT_SYS_SWR_INFO:
                     LOG(eUnCategorized,eWarning,"GMainMenu::HandleSPOTMessage - got eSPOT_OPT_SYS_SWR_INFO\n");
                     break;
               };
               break;
         };
         break;
   };

   return true;
}

void GMainMenu::HandleSoftKeyEvent(UINT8 KeyCode)
{
	if(m_eStatus == eInitialLocked)
	{
		EnableSoftKeys(); // no action keys
	}
	else if(m_eStatus == eUnLocked)
	{
		switch(KeyCode)
		{
			case eL1:  // L1 - Go to first page of devices menu
				m_pManager->PushMenu(new GDeviceMenuPage1());
				break;

			case eL4:  // L4 - Exit
				m_eStatus = eFinishing;
				BuildAndShowExitScreen();
				m_pManager->QueueMenuPop();
				break;

			case eL2:  // L2
			case eL3:  // L3
			case eR1:  // R1
			case eR2:  // R2
			case eR3:  // R3
			case eR4:  // R4

			default:
				EnableSoftKeys();
				break;
		}
	}
	else if(m_eStatus == eResourceFatalError)// errors downloading resources
	{
		switch(KeyCode)
		{
			case eL4:  // L4 - Exit
				m_eStatus = eFinishing;
				BuildAndShowExitScreen();
				m_pManager->QueueMenuPop();
				break;

			case eL1:  // L1
			case eL2:  // L2
			case eL3:  // L3
			case eR1:  // R1
			case eR2:  // R2
			case eR3:  // R3
			case eR4:  // R4

			default:
				EnableSoftKeys();
				break;
		}
	}
	else // exit phase
	{
	}
}

bool GMainMenu::HandleInactiveTimeEvent(UINT32 tLeft)
{
	return GAbsMenu::HandleInactiveTimeEvent(tLeft);
}




void GMainMenu::BuildAndShowExitScreen()
{
	GPopUpWindow popupWindow(eWinId_ExitScreen);
	GGlobalSystemInformation* pSysInfo = GGlobalSystemInformation::GetInstance();
	DY_HW_INFO &  rDYInfo = pSysInfo->GetDisplayInfo();
	UINT uHeight = rDYInfo.m_DevHwrConfig.m_u16Yres;
	UINT uWidth  = rDYInfo.m_DevHwrConfig.m_u16Xres;

	popupWindow.SetWindowWidthAndHeight(uWidth, uHeight);
	popupWindow.SetTextColor(0, 0, 0);
	popupWindow.SetTextPosition(20, uHeight/2 -13);
	popupWindow.SetTextWidthAndHeight(uWidth - 40, 26);
	popupWindow.SetText("EXIT phase in progress. Please WAIT ...");

	GSPOTMessage* createMessage = popupWindow.GetCreateMessage();
	if( ! m_pManager->SendCommandToSPOT(createMessage))
		LOG(eMenuManager, eError, "Could not send create window message for Exit Window.\n");
	delete createMessage;

	GSPOTMessage* hideMessage = m_pWinTemplate->GetHideMessage();
	if( ! m_pManager->SendCommandToSPOT(hideMessage))
		LOG(eMenuManager, eError, "Could not send hide window message for main menu.\n");
	delete hideMessage;


}

void GMainMenu::ResourceFatalErrorLock()
{
	m_eStatus = eResourceFatalError;
	GetTagStrings(m_eStatus, m_MenuLabelsMap);
	DrawMenu();
}

void GMainMenu::UnLockForNormalUse()
{
	m_eStatus = eUnLocked;
	GetTagStrings(m_eStatus, m_MenuLabelsMap);
	DrawMenu(true);

}


/**
 *
 * STATIC METHODS
 *
 */

int GMainMenu::GetTagStrings(eStatus eStatus, GSoftKeysWindowTemplate::tStringTagMap & rMap)
{
	int iTagNo = 0;
	// Initialize map of tag-id/label pairs for initial window creation
	switch(eStatus)
	{
		case eInitialLocked:
			rMap[GSoftKeysWindowTemplate::eSEGMENT_4] = "INITIALIZATION PHASE";
			rMap[GSoftKeysWindowTemplate::eSEGMENT_6] = "Please wait ...";
			iTagNo = 2;
			break;

		case eUnLocked:
			rMap[GSoftKeysWindowTemplate::eSOFT_L1]   = " Main Menu";
			rMap[GSoftKeysWindowTemplate::eSOFT_L4]   = " Exit";
			rMap[GSoftKeysWindowTemplate::eSEGMENT_4] = " ";
			rMap[GSoftKeysWindowTemplate::eSEGMENT_6] = " ";
			rMap[GSoftKeysWindowTemplate::eSEGMENT_7] = "";

			iTagNo = 4;
			break;

		case eFinishing:
			iTagNo = 0;
			break;

		case eResourceFatalError:
			rMap[GSoftKeysWindowTemplate::eSEGMENT_1] = "SPOTCHECK IS LOCKED.";
			rMap[GSoftKeysWindowTemplate::eSEGMENT_4] = "There was an error downloading resources";
			rMap[GSoftKeysWindowTemplate::eSEGMENT_5] = "This test is disabled.";
			rMap[GSoftKeysWindowTemplate::eSEGMENT_6] = "Please press exit.";
			rMap[GSoftKeysWindowTemplate::eSOFT_L1]   = "";
			rMap[GSoftKeysWindowTemplate::eSOFT_L4]   = std::string(" Exit");
			iTagNo = 6;
			break;
	}

	return iTagNo;
}
