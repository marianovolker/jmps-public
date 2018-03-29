/*
 * GSPOTDashboardController.cpp
 *
 *  Created on: 26/09/2013
 *      Author: slipchak
 */

// GLogger
#include "Common/Util/DEBUGOUT.h"
#include "GSPOTDashboardController.h"

#include "SPOT/SystemInformation/GGlobalSystemInformation.h"
#include "wx/wx.h"
#include "wx/app.h"
#include "Gui/GWxApp.cpp"

#include "Services/CardReader/GCardReaderService.h"
#include "Services/Keypad/GKeypadService.h"
#include "Services/MainUnit/GMainUnitService.h"
#include "Services/SecurityModule/GSecurityModuleService.h"

GSPOTDashboardController::GSPOTDashboardController()
{
}

GSPOTDashboardController::~GSPOTDashboardController()
{
}

void GSPOTDashboardController::startWxApp(int ac, char **av)
{
	//Start wxWidgetApplication and the Frame for the dashboard

	MyApp* pApp = new MyApp();
	//m_pApp = pApp;
	wxApp::SetInstance(pApp);

	wxEntryStart(ac, av);

	pApp->SetController(this);

	pApp->OnInit();

	// you can create top level-windows here or in OnInit()
	// do your testing here

	pApp->OnRun();
	pApp->OnExit();
	wxEntryCleanup();
}

//get from service
std::string GSPOTDashboardController::UpdateSpotType()
{
	GGlobalSystemInformation* pSysInfo = GGlobalSystemInformation::GetInstance();
	if (pSysInfo)
	{
		if (pSysInfo->isM3Unit())
		{
			return "SPOT M3";
		}
		else if(pSysInfo->isM5Unit())
		{
			return "SPOT M5";
		}
		else if(pSysInfo->isVGDUnit())
		{
			return "M7";
		}
	}
	return "UNKNOWN";
}


////////////////////////////////////////
/*
 Get information from the Model : in this case the model is the system information and the
 services that hold the information of the last status of the SPOT queried.
 */
////////////////////////////////////////

////////////////////////////////////////
//       KEYPAD STATUS
////////////////////////////////////////

std::string GSPOTDashboardController::UpdateOtherStatusKP()
{
	return GKeypadService::GetService()->GetRulesProcessor().GetDescriptionList();
}

bool GSPOTDashboardController::UpdateOtherErrorsKP()
{
	return !GKeypadService::GetService()->GetRulesProcessor().IsOtherError();
}

bool GSPOTDashboardController::UpdateCommKP()
{
	return !GKeypadService::GetService()->GetRulesProcessor().IsOffline();
}

bool GSPOTDashboardController::UpdateDismountKP()
{
	return !GKeypadService::GetService()->GetRulesProcessor().IsDismounted();
}

bool GSPOTDashboardController::UpdateTamperKP()
{
	return !GKeypadService::GetService()->GetRulesProcessor().IsTampered();
}


////////////////////////////////////////
//       CARD READER STATUS
////////////////////////////////////////

std::string GSPOTDashboardController::UpdateOtherStatusCR()
{
	return GCardReaderService::GetService()->GetRulesProcessor().GetDescriptionList();
}

bool GSPOTDashboardController::UpdateOtherErrorsCR()
{
	return !GCardReaderService::GetService()->GetRulesProcessor().IsOtherError();
}

bool GSPOTDashboardController::UpdateCommCR()
{
	return !GCardReaderService::GetService()->GetRulesProcessor().IsOffline();
}

bool GSPOTDashboardController::UpdateDismountCR()
{
	return !GCardReaderService::GetService()->GetRulesProcessor().IsDismounted();
}

bool GSPOTDashboardController::UpdateTamperCR()
{
	return !GCardReaderService::GetService()->GetRulesProcessor().IsTampered();
}


////////////////////////////////////////
// DISPLAY (main unit & security) STATUS
////////////////////////////////////////

std::string GSPOTDashboardController::UpdateOtherStatusDY()
{
	std::string  sDesc( GSecurityModuleService::GetService()->GetRulesProcessor().GetDescriptionList() );

	sDesc += GMainUnitService::GetService()->GetRulesProcessor().GetDescriptionList();
	return sDesc;
}

bool GSPOTDashboardController::UpdateOtherErrorsDY()
{
	if( GSecurityModuleService::GetService()->GetRulesProcessor().IsOtherError() )
		return false;

	return !GMainUnitService::GetService()->GetRulesProcessor().IsOtherError();
}

bool GSPOTDashboardController::UpdateCommDY()
{
	if( GSecurityModuleService::GetService()->GetRulesProcessor().IsOffline() )
		return false;

	return !GMainUnitService::GetService()->GetRulesProcessor().IsOffline();
}

bool GSPOTDashboardController::UpdateDismountDY()
{
	if( GSecurityModuleService::GetService()->GetRulesProcessor().IsDismounted() )
		return false;// It should never happen with the new rules after Oct 22 2013 (HSTOOLS-1230)

	return !GMainUnitService::GetService()->GetRulesProcessor().IsDismounted();
}

bool GSPOTDashboardController::UpdateTamperDY()
{
	if( GSecurityModuleService::GetService()->GetRulesProcessor().IsTampered() )
		return false;

	return !GMainUnitService::GetService()->GetRulesProcessor().IsTampered();
}


////////////////////////////////////////
//      SPOT Serial Number
////////////////////////////////////////

std::string GSPOTDashboardController::UpdateSpotSerial()
{
	char * pcValue = GGlobalSystemInformation::GetInstance()->GetSPOTInfo().m_pcSerialNumber;

	if (pcValue)
	{
		LOG(eUnCategorized, eInfo, "Info: SPOT serial number: [%s]\n", pcValue);

		return pcValue;
	}
	else
	{
		return "n/a";
	}

}


////////////////////////////////////////
//    Firmware version from devices
////////////////////////////////////////

std::string GSPOTDashboardController::UpdateFirmwareKP()
{

	const char * pcValue = GGlobalSystemInformation::GetInstance()->GetFirmwareInfo(eKPFwrVersion);
	if (pcValue)
	{
		LOG(eUnCategorized, eInfo, "Info: Keypad firmware: [%s]\n", pcValue);

		return pcValue;
	}
	else
	{
		return "n/a";
	}

}

std::string GSPOTDashboardController::UpdateFirmwareCR()
{

	const char * pcValue = GGlobalSystemInformation::GetInstance()->GetFirmwareInfo(eCRFwrVersion);
	if (pcValue)
	{
		LOG(eUnCategorized, eInfo, "Info: Card reader firmware: [%s]\n", pcValue);

		return pcValue;
	}
	else
	{
		return "n/a";
	}

}

std::string GSPOTDashboardController::UpdateFirmwareDY()
{

	char * pcValue = GGlobalSystemInformation::GetInstance()->GetSecModuleInfo().m_pcSerialNumber;
	if (pcValue)
	{
		LOG(eUnCategorized, eInfo, "Info: Display - Sec module firmware: [%s]\n", pcValue);

		return pcValue;
	}
	else
	{
		return "n/a";
	}

}


////////////////////////////////////////
//    Serial number from devices
////////////////////////////////////////

std::string GSPOTDashboardController::UpdateSerialKP()
{

	if( ! GKeypadService::GetService()->GetRulesProcessor().IsOffline() )
	{
		char * pcValue = GGlobalSystemInformation::GetInstance()->GetKeypadInfo().m_pcSerialNumber;
		if (pcValue)
		{
			LOG(eUnCategorized, eInfo, "Info: Keypad serial: [%s]\n", pcValue);

			return pcValue;
		}
	}

	return "n/a";

}

std::string GSPOTDashboardController::UpdateSerialCR()
{

	if( ! GCardReaderService::GetService()->GetRulesProcessor().IsOffline() )
	{
		char * pcValue = GGlobalSystemInformation::GetInstance()->GetCardReaderInfo().m_pcSerialNumber;
		if (pcValue)
		{
			LOG(eUnCategorized, eInfo, "Info: Card reader serial: [%s]\n", pcValue);

			return pcValue;
		}
	}

	return "n/a";

}

std::string GSPOTDashboardController::UpdateSerialDY()
{

	const char * pcValue = GGlobalSystemInformation::GetInstance()->GetFirmwareInfo(eSecFwrVersion);
	if (pcValue)
	{
		LOG(eUnCategorized, eInfo, "Info: Display - Sec module serial: [%s]\n", pcValue);

		return pcValue;
	}
	else
	{
		return "n/a";
	}

}
