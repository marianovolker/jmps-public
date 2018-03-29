#include "Menus/GAbsMenu.h"
#include "Menus/GMenuManager.h"
#include "SPOT/SPOTProtocolInterface/GSPOTDefs.h"

const int GAbsMenu::CiMaxUserLabels = GSoftKeysWindowTemplate::eSEGMENT_8 - GSoftKeysWindowTemplate::eSEGMENT_2 + 1;

GAbsMenu::GAbsMenu()
  :	m_blInitialDrawing(true), m_iWinID(-1),
	m_pWinTemplate(GSoftKeysWindowTemplate::GetSoleInstance()),
	m_pManager(GMenuManager::GetSoleInstance())
{
	m_MenuLabelsMap[GSoftKeysWindowTemplate::eTIMER]=  GSoftKeysWindowTemplate::m_pcTimerInitLabel;
}

GAbsMenu::~GAbsMenu()
{
}

/**
 * SetTitleLabel()
 * This method updates the title label on screen only
 * note: you should call UpdateUserLabels() method to draw the screen
 */
void GAbsMenu::SetTitleLabel(const char* pcLabel)
{
	m_MenuLabelsMap[GSoftKeysWindowTemplate::eSEGMENT_1] = pcLabel;

}

/**
 * UpdateUserLabels()
 * This method allow to update an arbitrary number (up to 7) of "user menu labels",
 * that is labels between eSEGMENT_2 and eSEGMENT_8 inclusive.
 * If the labels to be updated are not adjacent, then use null pointers. Any label
 * described by a null pointer is not taken into account and also is not counted
 * for the total number of labels.
 */
void GAbsMenu::UpdateUserLabels(int nLabels, const char* arypcLabels[])
{
	if (nLabels <= 0)
		return; // bad parameter

	bool blUpdate = false;
	int i = 0, iMaxLabels = (nLabels > CiMaxUserLabels ? CiMaxUserLabels : nLabels);

	while (i < iMaxLabels)
	{
		if (arypcLabels[i] == NULL)
		{
			++i;
			continue; // null label isn't taken into account and not counted for length
		}

		GSoftKeysWindowTemplate::eTAG_ID eKey;

		eKey = static_cast<GSoftKeysWindowTemplate::eTAG_ID>(i + GSoftKeysWindowTemplate::eSEGMENT_2);
		m_MenuLabelsMap[eKey] = arypcLabels[i++];
		blUpdate = true;
	}

	if (blUpdate)
		DrawScreen();
}


void GAbsMenu::ResetTimerLabel()
{
	m_MenuLabelsMap[GSoftKeysWindowTemplate::eTIMER] = GSoftKeysWindowTemplate::m_pcTimerInitLabel;
}


/**
 * DrawMenu()
 * Basic drawing functionality for all menus.
 * This method only prints the predefined tag segment label texts
 * on the menu window. Derived class can use this method as a
 * startup step in more complex screen drawing.
 */
bool GAbsMenu::DrawMenu(bool blDisablePost /* = false */)
{
	bool blretval = false;
	GSPOTMessage* msg = NULL;

	m_pWinTemplate->SetLabels(m_MenuLabelsMap, true);
	msg = m_pWinTemplate->GetUpdateTextMessage(m_blInitialDrawing);
	m_blInitialDrawing = false;

	if (msg)
	{
		// we don't expect a response from this message so we can enable the pinpad/softkeys
		if (m_pManager->SendCommandToSPOT(msg))
		{
			// we know the initial message displayed so we are ok to return success
			blretval = true;
			if (!blDisablePost)
				PostDrawingAction();
		}

		delete msg;
	}

	return blretval;
}

bool  GAbsMenu::DrawTimer()
{
	bool blretval = false;
	GSPOTMessage* msg = NULL;

	m_pWinTemplate->SetLabels(m_MenuLabelsMap, true);
	msg = m_pWinTemplate->GetUpdateTextTagMessage((UINT) GSoftKeysWindowTemplate::eTIMER);
	m_blInitialDrawing = false;

	if (msg)
	{
			// we don't expect a response from this message so we can enable the pinpad/softkeys
		if (m_pManager->SendCommandToSPOT(msg))
		{
				// we know the initial message displayed so we are ok to return success
				blretval = true;
		}

		delete msg;
	}

	return blretval;

}


void GAbsMenu::PostDrawingAction()
{
	EnableSoftKeys();
}

using namespace GSpotDefs;

/**
 * SetLabel()
 * Set any tag to a specific string.
 */
void GAbsMenu::SetLabel(GSoftKeysWindowTemplate::eTAG_ID eTagId, const char* szLabel)
{
	m_MenuLabelsMap[eTagId] = std::string(szLabel);
}

bool GAbsMenu::EnableSoftKeys(bool blEnableAuxKeysToo /* = false */)
{
	bool retval = false;

	if (m_pManager)
	{
		GSPOTMessage* msg = NULL;
		UINT8 keymask[32];

		// now try to enable the keys
		for (int i = 0; i < 32; i++)
		{
			if( (i >= eL1) && (i <= eR4) )
				keymask[i] = 0x07; // 0x07 means enable, light and beep on press (bitmask)
			else if( blEnableAuxKeysToo && (i >= eP1) && (i <= eP10) )
				keymask[i] = 0x07;
			else
				keymask[i] = 0x00;
		}

		// build the message
		msg = new GSPOTMessage(eSPOT_OPT_CATEGORY, eSPOT_OPT_TERMIO, eSPOT_OPT_IO_KEYENABLE);

		if (msg)
		{
			msg->Append(keymask, sizeof(keymask));

			retval = m_pManager->SendCommandToSPOT(msg);
			delete msg;
		}
	}
	return retval;
}

bool GAbsMenu::HandleInactiveTimeEvent(UINT32 tLeft)
{

	char newLabels[128] = {'\0'};
	snprintf(newLabels,sizeof(newLabels),GSoftKeysWindowTemplate::m_pcTimerLabelKey,tLeft);
	SetLabel(GSoftKeysWindowTemplate::eTIMER, newLabels);
	DrawTimer();


		return false;
}

