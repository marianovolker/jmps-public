#include <cstdio>
#include <string>

#include "Common/Util/DEBUGOUT.h"
#include "GKeypadMenu.h"
#include "Menus/Keypad/GKeypadManager.h"
#include "SPOT/SystemInformation/GGlobalSystemInformation.h"


GKeypadMenu::GKeypadMenu()
{
	m_iWinID = eWinId_KeypadMenu;

	//TODO: Eliminate this when repository is working OK
	GetTagStrings(m_MenuLabelsMap);
}


/**
 *
 * VIRTUAL METHODS
 *
 */

GKeypadMenu::~GKeypadMenu()
{
}

void GKeypadMenu::Initialize()
{
	m_pKBManager = new GKeypadManager(this);
	m_pKBManager->Initialize();
}

bool GKeypadMenu::DrawMenu( bool blDisablePost /* = false */ )
{
	return GAbsKeyboardMenu::DrawMenu( blDisablePost );
}

void GKeypadMenu::HandleSoftKeyEvent(UINT8 KeyCode)
{
	return GAbsKeyboardMenu::HandleSoftKeyEvent(KeyCode);
}


/**
 *
 * STATIC METHODS
 *
 */

int GKeypadMenu::GetTagStrings(GSoftKeysWindowTemplate::tStringTagMap & rMap)
{
	int                       iTags    = GAbsKeyboardMenu::GetTagStrings(rMap);
	GGlobalSystemInformation* pSysInfo = GGlobalSystemInformation::GetInstance();
	KB_HW_INFO&               rKBInfo  = pSysInfo->GetKeypadInfo();
	char                      arycBuffer[32];

	// initialize map of tag-id/label pairs for initial window creation
	rMap[GSoftKeysWindowTemplate::eSEGMENT_1] = std::string(GKeypadManager::GetMenuTitle());

	sprintf(arycBuffer, "Hardware ID:  %02X%02X  ", int(rKBInfo.m_aryu8HwrID[0]), int(rKBInfo.m_aryu8HwrID[1]));
	rMap[GSoftKeysWindowTemplate::eSEGMENT_2] = std::string(arycBuffer);

	sprintf(arycBuffer, "Type:  %02X  ", int(rKBInfo.m_u8DevType));
	rMap[GSoftKeysWindowTemplate::eSEGMENT_3] = std::string(arycBuffer);

	sprintf(arycBuffer, "S/N:  %s  ", rKBInfo.m_pcSerialNumber);
	rMap[GSoftKeysWindowTemplate::eSEGMENT_4] = std::string(arycBuffer);

	return 4 + iTags;
}
