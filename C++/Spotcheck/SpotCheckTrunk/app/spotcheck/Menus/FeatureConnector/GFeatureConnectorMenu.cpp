/*
 * GFeatureConnectorMenu.cpp
 *
 *  Created on: Feb 9, 2015
 *      Author: Sayyed Mohammad
 */

#include <cstdio>
#include <string>

#include "Common/Util/DEBUGOUT.h"
#include "Common/Util/GConversion.h"
#include "GFeatureConnectorMenu.h"
#include "SPOT/SystemInformation/GGlobalSystemInformation.h"
#include "GSystem.h"
#include "GTypes.h"


SPOT_PP_DY_SECUREMEDIACONNECTOR_MODES GFeatureConnectorMenu::m_eDisplayMode = eSPOT_PP_DY_SECUREMEDIACONNECTOR_INTERNAL_SECURE;

GFeatureConnectorMenu::GFeatureConnectorMenu():
		m_pTimer(NULL),
		m_eFCStatus(eFCStatusInitial),
		m_TOutCommand(this, &GFeatureConnectorMenu::ProcessTimeout)

{
	m_iWinID = eWinId_FeatureConnectorMenu;

	//TODO: Eliminate this when repository is working OK
	GetTagStrings(m_MenuLabelsMap);
	m_pTimer = new GTimer(&m_TOutCommand, FC_TOUT_FEATURECONNECTOR_DELAY, false, false);

}


/**
 *
 * VIRTUAL METHODS
 *
 */

GFeatureConnectorMenu::~GFeatureConnectorMenu()
{

}


bool GFeatureConnectorMenu::DrawMenu( bool blDisablePost /* = false */ )
{
	return GAbsMenu::DrawMenu( blDisablePost );
}

void GFeatureConnectorMenu::ClearScreen()
{
	m_MenuLabelsMap[GSoftKeysWindowTemplate::eSOFT_L1] = "";
	m_MenuLabelsMap[GSoftKeysWindowTemplate::eSOFT_L2] = "";
	m_MenuLabelsMap[GSoftKeysWindowTemplate::eSOFT_L3] = "";
	m_MenuLabelsMap[GSoftKeysWindowTemplate::eSOFT_L4] = "";

	m_MenuLabelsMap[GSoftKeysWindowTemplate::eSEGMENT_1] = "";
	m_MenuLabelsMap[GSoftKeysWindowTemplate::eSEGMENT_2] = "";
	m_MenuLabelsMap[GSoftKeysWindowTemplate::eSEGMENT_3] = "";
	m_MenuLabelsMap[GSoftKeysWindowTemplate::eSEGMENT_4] = "";
	m_MenuLabelsMap[GSoftKeysWindowTemplate::eSEGMENT_5] = "";
	m_MenuLabelsMap[GSoftKeysWindowTemplate::eSEGMENT_6] = "";
	m_MenuLabelsMap[GSoftKeysWindowTemplate::eSEGMENT_7] = "";
	m_MenuLabelsMap[GSoftKeysWindowTemplate::eSEGMENT_8] = "";
	m_MenuLabelsMap[GSoftKeysWindowTemplate::eSEGMENT_9] = "";

	m_MenuLabelsMap[GSoftKeysWindowTemplate::eSOFT_R1] = "";
	m_MenuLabelsMap[GSoftKeysWindowTemplate::eSOFT_R2] = "";
	m_MenuLabelsMap[GSoftKeysWindowTemplate::eSOFT_R3] = "";
	m_MenuLabelsMap[GSoftKeysWindowTemplate::eSOFT_R4] = "";

	DrawMenu();

}


bool GFeatureConnectorMenu::DrawExternalMediaTestMenu(std::string text)
{
	m_MenuLabelsMap[GSoftKeysWindowTemplate::eSOFT_L1] = std::string(" Yes");
	m_MenuLabelsMap[GSoftKeysWindowTemplate::eSOFT_L2] = std::string("");
	m_MenuLabelsMap[GSoftKeysWindowTemplate::eSOFT_L3] = std::string(" No");
	m_MenuLabelsMap[GSoftKeysWindowTemplate::eSOFT_L4] = std::string(" Previous\n Menu");

	m_MenuLabelsMap[GSoftKeysWindowTemplate::eSEGMENT_1] = "External PC Test";
	m_MenuLabelsMap[GSoftKeysWindowTemplate::eSEGMENT_2] = "";
	m_MenuLabelsMap[GSoftKeysWindowTemplate::eSEGMENT_3] = "";
	m_MenuLabelsMap[GSoftKeysWindowTemplate::eSEGMENT_4] = "Did you see the external media screen?";
	m_MenuLabelsMap[GSoftKeysWindowTemplate::eSEGMENT_5] = "";
	m_MenuLabelsMap[GSoftKeysWindowTemplate::eSEGMENT_6] = "";
	m_MenuLabelsMap[GSoftKeysWindowTemplate::eSEGMENT_7] = "";
	m_MenuLabelsMap[GSoftKeysWindowTemplate::eSEGMENT_8] = "";
	m_MenuLabelsMap[GSoftKeysWindowTemplate::eSEGMENT_9] = "";

	m_MenuLabelsMap[GSoftKeysWindowTemplate::eSOFT_R1] = "";
	m_MenuLabelsMap[GSoftKeysWindowTemplate::eSOFT_R2] = "";
	m_MenuLabelsMap[GSoftKeysWindowTemplate::eSOFT_R3] = "";
	m_MenuLabelsMap[GSoftKeysWindowTemplate::eSOFT_R4] = "";

	return DrawMenu();
}

bool GFeatureConnectorMenu::DrawInternalNonSecureTestMenu(std::string text)
{
	m_MenuLabelsMap[GSoftKeysWindowTemplate::eSOFT_L1] = std::string(" Yes");
	m_MenuLabelsMap[GSoftKeysWindowTemplate::eSOFT_L2] = std::string("");
	m_MenuLabelsMap[GSoftKeysWindowTemplate::eSOFT_L3] = std::string(" No");
	m_MenuLabelsMap[GSoftKeysWindowTemplate::eSOFT_L4] = std::string(" Previous\n Menu");

	m_MenuLabelsMap[GSoftKeysWindowTemplate::eSEGMENT_1] = "Internal Non Secure Test";
	m_MenuLabelsMap[GSoftKeysWindowTemplate::eSEGMENT_2] = "";
	m_MenuLabelsMap[GSoftKeysWindowTemplate::eSEGMENT_3] = "";
	m_MenuLabelsMap[GSoftKeysWindowTemplate::eSEGMENT_4] = "Did you see the black screen?";
	m_MenuLabelsMap[GSoftKeysWindowTemplate::eSEGMENT_5] = "";
	m_MenuLabelsMap[GSoftKeysWindowTemplate::eSEGMENT_6] = "";
	m_MenuLabelsMap[GSoftKeysWindowTemplate::eSEGMENT_7] = "";
	m_MenuLabelsMap[GSoftKeysWindowTemplate::eSEGMENT_8] = "";
	m_MenuLabelsMap[GSoftKeysWindowTemplate::eSEGMENT_9] = "";

	m_MenuLabelsMap[GSoftKeysWindowTemplate::eSOFT_R1] = "";
	m_MenuLabelsMap[GSoftKeysWindowTemplate::eSOFT_R2] = "";
	m_MenuLabelsMap[GSoftKeysWindowTemplate::eSOFT_R3] = "";
	m_MenuLabelsMap[GSoftKeysWindowTemplate::eSOFT_R4] = "";

	return DrawMenu();
}

bool GFeatureConnectorMenu::DrawTestResultMenu()
{
	GFeatureConnectorStatistics* oFCStats = GFeatureConnectorStatistics::GetInstance();

	m_MenuLabelsMap[GSoftKeysWindowTemplate::eSOFT_L1] = "";
	m_MenuLabelsMap[GSoftKeysWindowTemplate::eSOFT_L2] = "";
	m_MenuLabelsMap[GSoftKeysWindowTemplate::eSOFT_L3] = "";
	m_MenuLabelsMap[GSoftKeysWindowTemplate::eSOFT_L4] = std::string(" Previous\n Menu");

	char arycBuffer[64];

	m_MenuLabelsMap[GSoftKeysWindowTemplate::eSEGMENT_1] = "The result of Feature Connector test is:";
	m_MenuLabelsMap[GSoftKeysWindowTemplate::eSEGMENT_2] = "";
	m_MenuLabelsMap[GSoftKeysWindowTemplate::eSEGMENT_3] = "";

	snprintf(arycBuffer,sizeof(arycBuffer),"External: %s", oFCStats->GetExternalStatistics());
	m_MenuLabelsMap[GSoftKeysWindowTemplate::eSEGMENT_4] = arycBuffer;

	memset(arycBuffer,0,sizeof(arycBuffer));

	snprintf(arycBuffer,sizeof(arycBuffer),"Internal NonSecure: %s", oFCStats->GetInternalNsStatistics());
	m_MenuLabelsMap[GSoftKeysWindowTemplate::eSEGMENT_5] = arycBuffer;

	m_MenuLabelsMap[GSoftKeysWindowTemplate::eSEGMENT_6] = "";//arycBuffer;
	m_MenuLabelsMap[GSoftKeysWindowTemplate::eSEGMENT_7] = "";//arycBuffer;

	m_MenuLabelsMap[GSoftKeysWindowTemplate::eSEGMENT_9] = "";

	m_MenuLabelsMap[GSoftKeysWindowTemplate::eSOFT_R1] = "";
	m_MenuLabelsMap[GSoftKeysWindowTemplate::eSOFT_R2] = "";
	m_MenuLabelsMap[GSoftKeysWindowTemplate::eSOFT_R3] = "";
	m_MenuLabelsMap[GSoftKeysWindowTemplate::eSOFT_R4] = "";

	return DrawMenu();
}

void GFeatureConnectorMenu::HandleSoftKeyEvent(UINT8 KeyCode)
{
	switch (KeyCode)
	{
		case eL1:
			ProcessEventsReceived(eEventSoftKeyL1);
			break;

		case eL2:
			ProcessEventsReceived(eEventSoftKeyL2);
			break;
		case eL3:
			ProcessEventsReceived(eEventSoftKeyL3);
			break;
		case eL4:
			ProcessEventsReceived(eEventSoftKeyL4);
			break;
		case eR1:
			ProcessEventsReceived(eEventSoftKeyR1);
			break;
		case eR2:
			ProcessEventsReceived(eEventSoftKeyR2);
			break;
		case eR3:
			ProcessEventsReceived(eEventSoftKeyR3);
			break;
		case eR4:
			ProcessEventsReceived(eEventSoftKeyR4);
			break;

		default:
			break;

	}

}

void GFeatureConnectorMenu::NextState(eEvents e, eFCStatus s)
{
//	LOG( eDisplay, eDebug, "[current_status[%s] ==>> event[%s] ==>> next_status[%s]]\n", mst_arypcStatusLabel[m_SecStatus], mst_arypcEventLabel[e], mst_arypcStatusLabel[s] );
	m_eFCStatus = s;
}

void GFeatureConnectorMenu::ProcessEventsReceived(eEvents e)
{
	switch(m_eFCStatus)
	{
		case eFCStatusInitial:
		{
			switch (e)
			{
				case eEventSoftKeyL1:
					NextState(e, eFCStatusMedia);
					SwitchDisplay(eSPOT_PP_DY_SECUREMEDIACONNECTOR_EXTERNAL);
					m_pTimer->Start();
					//EnableSoftKeys();
					break;

				case eEventSoftKeyL2:
					NextState(e, eFCStatusMedia);
					SwitchDisplay(eSPOT_PP_DY_SECUREMEDIACONNECTOR_INTERNAL_NONSECURE);
					m_pTimer->Start();
					//EnableSoftKeys();
					break;

				case eEventSoftKeyL4:  // L4 - back to previous menu
					SwitchDisplay(eSPOT_PP_DY_SECUREMEDIACONNECTOR_INTERNAL_SECURE);
					m_pManager->QueueMenuPop();
					break;

				case eEventSoftKeyR4:  // R4 Go to Summary
					NextState(e, eFCStatusSummary);
					ClearScreen();
					DrawTestResultMenu();
					break;

				default:
					EnableSoftKeys();
					break;
			}
			break;
		}
		//---------------------------------------------------------------------------------


		case eFCStatusMedia:
		{
			switch (e)
			{
				case eEventTimeout:
				{

					NextState(e, eFCStatusCheckTest);
					m_pTimer->Stop();

					switch(m_eDisplayMode)
					{
						case eSPOT_PP_DY_SECUREMEDIACONNECTOR_EXTERNAL:

							DrawExternalMediaTestMenu("");
							SwitchDisplay(eSPOT_PP_DY_SECUREMEDIACONNECTOR_INTERNAL_SECURE);
							EnableSoftKeys();
							break;

						case eSPOT_PP_DY_SECUREMEDIACONNECTOR_INTERNAL_NONSECURE:
							DrawInternalNonSecureTestMenu("");
							SwitchDisplay(eSPOT_PP_DY_SECUREMEDIACONNECTOR_INTERNAL_SECURE);
							EnableSoftKeys();
							break;

					default:   // eSPOT_PP_DY_SECUREMEDIACONNECTOR_INTERNAL_SECURE:
						break;
					}
				break;
				}

				default:
					EnableSoftKeys();
					break;
			}
			break;
		}

		//---------------------------------------------------------------------------------

		case eFCStatusCheckTest:
		{
			switch(e)
			{
				case eEventSoftKeyL1:
				{
					NextState(e, eFCStatusSummary);
					if(m_eDisplayMode == eSPOT_PP_DY_SECUREMEDIACONNECTOR_EXTERNAL)
					{
						GFeatureConnectorStatistics::GetInstance()->SetExternalStatistics(GFeatureConnectorStatistics::eFCRPass);
					}
					else
					{
						GFeatureConnectorStatistics::GetInstance()->SetInternalNsStatistics(GFeatureConnectorStatistics::eFCRPass);
					}

					DrawTestResultMenu();
					break;
				}
				case eEventSoftKeyL3:
				{
					NextState(e, eFCStatusSummary);
					if(m_eDisplayMode == eSPOT_PP_DY_SECUREMEDIACONNECTOR_EXTERNAL)
					{
						GFeatureConnectorStatistics::GetInstance()->SetExternalStatistics(GFeatureConnectorStatistics::eFCRFail);
					}
					else
					{
						GFeatureConnectorStatistics::GetInstance()->SetInternalNsStatistics(GFeatureConnectorStatistics::eFCRFail);

					}

					DrawTestResultMenu();
					break;
				}
				case eEventSoftKeyL4:
				{
					NextState(e, eFCStatusInitial);
					ClearScreen();
					GetTagStrings(m_MenuLabelsMap);
					DrawScreen();
					EnableSoftKeys();
					break;
				}
				default:
					EnableSoftKeys();
					break;
			}
			break;
		}

		//======================================================================================================
		case eFCStatusSummary:
		{
			switch(e)
			{

				case eEventTimeout:
					NextState(eEventTimeout, eFCStatusInitial);
					ClearScreen();
					GetTagStrings(m_MenuLabelsMap);
					DrawScreen();
				break;

				case eEventSoftKeyL4:
					NextState(eEventSoftKeyL4, eFCStatusInitial);
					ClearScreen();
					GetTagStrings(m_MenuLabelsMap);
					DrawScreen();
					EnableSoftKeys();
				break;

				case eEventSoftKeyL1:
				case eEventSoftKeyL2:
				case eEventSoftKeyL3:
				case eEventSoftKeyR1:
				case eEventSoftKeyR2:
				case eEventSoftKeyR3:
				case eEventSoftKeyR4:
					EnableSoftKeys();
					//LOG(eDisplay, eTrace, "IGNORED EVENT [%s] received in [%s].\n", mst_arypcEventLabel[e], mst_arypcStatusLabel[m_SecStatus]);
				break;

				default:
					;
					//LOG(eDisplay, eError, "ERROR: INVALID EVENT [%s] received in [%s].\n", mst_arypcEventLabel[e], mst_arypcStatusLabel[m_SecStatus]);
				break;
			}
			break;
		}
		//======================================================================================================
	}
}

void GFeatureConnectorMenu::ProcessTimeout()
{
	ProcessEventsReceived(eEventTimeout);

}

bool GFeatureConnectorMenu::HandleSPOTMessage(GSPOTMessage* pMsg)
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
	      case eSPOT_PP_ANSWER:
	         switch (CmdCode)
	         {
	            case eSPOT_PP_DISPLAY:
	               switch (SubCmdCode)
	               {
	                  case eSPOT_PP_DY_SECUREMEDIACONNECTOR:
	                     //this->SetLabel(GSoftKeysWindowTemplate::eSEGMENT_7,(const char*)data);
	                     GConversion::LogBytesAsASCIIHexString(pMsg->GetData(), pMsg->GetDataLen());
	                     DrawMenu();
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


bool GFeatureConnectorMenu::SwitchDisplay(UINT8 u8Type)
{
	GSPOTMessage* msg = new GSPOTMessage(eSPOT_PP_CATEGORY,eSPOT_PP_DISPLAY, eSPOT_PP_DY_SECUREMEDIACONNECTOR);
	bool blret = false;
	if (msg)
	{
		switch(u8Type)
		{
			case eSPOT_PP_DY_SECUREMEDIACONNECTOR_INTERNAL_SECURE:
				msg->AppendByte(eSPOT_PP_DY_SECUREMEDIACONNECTOR_INTERNAL_SECURE);
				break;

			case eSPOT_PP_DY_SECUREMEDIACONNECTOR_INTERNAL_NONSECURE:
				msg->AppendByte(eSPOT_PP_DY_SECUREMEDIACONNECTOR_INTERNAL_NONSECURE);
				m_eDisplayMode=eSPOT_PP_DY_SECUREMEDIACONNECTOR_INTERNAL_NONSECURE;
				break;

			case eSPOT_PP_DY_SECUREMEDIACONNECTOR_EXTERNAL:
				msg->AppendByte(eSPOT_PP_DY_SECUREMEDIACONNECTOR_EXTERNAL);
				m_eDisplayMode=eSPOT_PP_DY_SECUREMEDIACONNECTOR_EXTERNAL;
				break;

			default:
			{
				delete msg;
				return blret;
			}
		}
			blret = m_pManager->SendCommandToSPOT(msg);
			delete msg;
	}
	return blret;
}
/**
 *
 * STATIC METHODS
 *
 */

int GFeatureConnectorMenu::GetTagStrings(GSoftKeysWindowTemplate::tStringTagMap & rMap)
{
	rMap[GSoftKeysWindowTemplate::eSEGMENT_1] = std::string("Feature Connector test");
	rMap[GSoftKeysWindowTemplate::eSEGMENT_2] = std::string(" Switch Display ");
	rMap[GSoftKeysWindowTemplate::eSEGMENT_3] = std::string("--->\n<---");

	rMap[GSoftKeysWindowTemplate::eSOFT_L1] = std::string(" External PC");
	rMap[GSoftKeysWindowTemplate::eSOFT_L2] = std::string(" Internal \nNonSecure");
	rMap[GSoftKeysWindowTemplate::eSOFT_L4] = std::string(" Previous\n Menu");
	rMap[GSoftKeysWindowTemplate::eSOFT_R4] = std::string("Summary ");

	return 0;
}

