#include <cstdio>
#include <cstring>

#include "GCardReaderManager.h"

#include "Common/Util/DEBUGOUT.h"
#include "Services/CardReader/GCardReaderService.h"
#include "Menus/CardReader/GCardReaderMenu.h"

char GCardReaderManager::mst_arycLegendTemplate[CLR_LEGEND_TEMPLATE_LEN] = "Track%d: %s";

char GCardReaderManager::mst_arycLegendChipTemplate[CLR_LEGEND_TEMPLATE_LEN] = "Chip:    %s";


char GCardReaderManager::mst_arycMagStripeLegendTemplate[CLR_LEGEND_TEMPLATE_LEN] = "Mag Stripe: %s";

GCardReaderManager::GCardReaderManager(GCardReaderMenu *pMenu) :
				m_pCRMenu(pMenu), m_Status(eShowInitial), m_pCRService(NULL), m_bACardWasRead(false)
{
}

GCardReaderManager::~GCardReaderManager()
{
}

// called after costruction
void GCardReaderManager::Initialize()
{
	m_pCRService = GCardReaderService::GetService();

	if (m_pCRService)
	{
		m_pCRService->AttachManager(this);
		EnableDevice();
	}

	memset(m_arycLegends[eTrack1], 0, CLR_LEGEND_MAX_LEN );
	memset(m_arycLegends[eTrack2], 0, CLR_LEGEND_MAX_LEN );
	memset(m_arycLegends[eTrack3], 0, CLR_LEGEND_MAX_LEN );
	memset(m_arycLegends[eTrack4], 0, CLR_LEGEND_MAX_LEN );

	memset(m_cMagStripeLegend, 0, sizeof(m_cMagStripeLegend));
	memset(m_cChipCardLegend, 0, sizeof(m_cMagStripeLegend));

}

// called before destruction
void GCardReaderManager::Dispose()
{
	m_pCRService = GCardReaderService::GetService();
	if (m_pCRService)
	{
		DisableDevice();
		m_pCRService->DetachManager();
	}

}

void GCardReaderManager::NotifyCardInserted()
{
	LOG(eCardReader, eDebug, "GCardReaderManager::NotifyCardInserted(). MAGSTRIPE INSERTED\n");

	if (m_pCRMenu)
	{
		SetStatus(eShowPleaseRemoveCard);
		m_pCRMenu->ClearScreen();
		m_pCRMenu->ShowCardInsertNotification();
	}
}

void GCardReaderManager::NotifyChipCardInserted()
{

	LOG(eCardReader, eDebug, "GCardReaderManager::NotifyCHIPCARD-Inserted(). CHIP INSERTED\n");

	m_bACardWasRead = true;

	if (m_pCRMenu)
	{
		SetStatus(eShowChipDetected);
		m_pCRMenu->ClearScreen();
		m_pCRMenu->ShowChipCardDetected();
	}
}

void GCardReaderManager::NotifyCardRemoved(TRACK_INFO* pTracks)
{
	// CR manager's statistics update goes here

	LOG(eCardReader, eDebug, "GCardReaderManager::NotifyCardRemoved(). CARD REMOVED\n");


	if (ChangeStateTo(eShowTestResult))
	{

		memset(m_arycLegends, 0, sizeof(m_arycLegends));

		sprintf(&m_arycLegends[eTrack1][0], mst_arycLegendTemplate, 1,
		                GetTrackSimpleDescription(&pTracks[eTrack1]));

		sprintf(&m_arycLegends[eTrack2][0], mst_arycLegendTemplate, 2,
		                GetTrackSimpleDescription(&pTracks[eTrack2]));

		sprintf(&m_arycLegends[eTrack3][0], mst_arycLegendTemplate, 3,
		                GetTrackSimpleDescription(&pTracks[eTrack3]));

		sprintf(&m_arycLegends[eTrack4][0], mst_arycLegendChipTemplate, GetTrackSimpleDescription(&pTracks[eTrack4]));

		sprintf(&m_cMagStripeLegend[0], mst_arycMagStripeLegendTemplate, GetMagstripeSimpleDescription(pTracks));
		sprintf(&m_cChipCardLegend[0], "%s", m_arycLegends[eTrack4]);

		LOG(eCardReader, eInfo, "GCardReaderManager::NotifyCardRemoved(). Printing simple description for tracks\n");
		LOG(eCardReader, eInfo, "%s\n", m_arycLegends[eTrack1]);
		LOG(eCardReader, eInfo, "%s\n", m_arycLegends[eTrack2]);
		LOG(eCardReader, eInfo, "%s\n", m_arycLegends[eTrack3]);
		LOG(eCardReader, eInfo, "%s\n", m_arycLegends[eTrack4]);

		m_bACardWasRead = true;

		if (m_pCRMenu)
		{
			m_pCRMenu->ClearScreen();
			m_pCRMenu->ShowCardRemoveNotification(m_cMagStripeLegend, m_cChipCardLegend);
		}
	}

}

void GCardReaderManager::GoToTestResultScreen()
{
	if (ChangeStateTo(eShowTestResult))
	{
		if (m_pCRMenu)
		{
			m_pCRMenu->ClearScreen();
			m_pCRMenu->ShowCardRemoveNotification(m_cMagStripeLegend, m_cChipCardLegend);
		}
	}

}

void GCardReaderManager::GoToDetailsScreen()
{
	if (ChangeStateTo(eShowDetails))
	{
		if (m_pCRMenu)
		{
			m_pCRMenu->ClearScreen();
			m_pCRMenu->ShowDetailsScreen(m_arycLegends[eTrack1], m_arycLegends[eTrack2], m_arycLegends[eTrack3], m_arycLegends[eTrack4]);
		}
	}

}

void GCardReaderManager::GoToStatistics()
{
	if (ChangeStateTo(eShowStats))
	{
		if (m_pCRMenu)
		{
			m_pCRMenu->ClearScreen();
			m_pCRMenu->ShowStatsScreen();
		}
	}

}

// check possible state changes
bool GCardReaderManager::ChangeStateTo(eStatus windowStatus)
{

	bool bShowScreen = false;

	switch (windowStatus)
	{
		case eShowInitial:
			if (m_Status != eShowInitial)  //only possible from devices menu
				return false; //wrong state change
			else
				SetStatus(eShowInitial);

		break;

		case eShowChipDetected:

			if (m_Status == eShowInitial)
			{
				//change state first just in case.
				SetStatus(eShowChipDetected);
			}
			else if (m_Status == eShowPleaseRemoveCard)
			{
				//change state first just in case.
				SetStatus(eShowChipDetected);

				LOG(eCardReader, eWarning, "ChangeStateTo() to eShowChipDetected. Got MAGSTRIPE before chip\n");

			}
			else
				return false; //wrong state change

		break;

		case eShowPleaseRemoveCard:

			if (m_Status == eShowInitial)
			{
				//change state first just in case.
				SetStatus(eShowPleaseRemoveCard);
			}
			else if (m_Status == eShowChipDetected)
			{
				//change state first just in case.
				SetStatus(eShowPleaseRemoveCard);

			}
			else
			{
				LOG(eCardReader, eWarning, "ChangeStateTo() invalid change to eShowPleaseRemoveCard.\n");
				return false; //wrong state change

			}

		break;

		case eShowTestResult:

			if (m_Status == eShowInitial || m_Status == eShowPleaseRemoveCard || m_Status == eShowDetails || m_Status == eShowStats)
			{
				if (m_Status != eShowTestResult) // if this is a new state I have to change to the new one
				{
					//change state first just in case.
					SetStatus(eShowTestResult);
				}
			}
			else
				return false; //wrong state change

		break;

		case eShowDetails:

			if (m_Status == eShowTestResult)
			{
					//change state first just in case.
					SetStatus(eShowDetails);
			}
			else
				return false; //wrong state change

		break;

		case eShowStats:

			if (m_Status == eShowChipDetected || m_Status == eShowPleaseRemoveCard)
			{
				return false;
			}
			else
			{
				SetStatus(eShowStats);
			}

		break;

	}

	return true;
}

const char* GCardReaderManager::GetTrackSimpleDescription(TRACK_INFO* pTrack)
{

	if (pTrack->m_u8Present == 0)
	{
		//this means that the track was not present at all, or the hardware didn't supported that type of track
		//for example it doesn't have a chip reader.
		return "Not present";

	}
	else
	{
		switch (pTrack->m_u8ErrorCode)
		{
			case 0:
				//"good";
				return "PASS";

			case 1:
				// "not encoded";
			case 0x20:
				// "no data";
				return "Not present";

			case 2:
				//	 "SS ERROR";
			case 4:
				// "ES ERROR";
			case 8:
				// "LRC ERROR";
			case 0x10:
				// "URC ERROR";
			case 0x40:
				// "jitter ERROR";
			case 0x80:
				// "generic ERROR";
			default:
				return "FAIL";

		}
	}

}

const char* GCardReaderManager::GetMagstripeSimpleDescription(TRACK_INFO* pTracks)
{



	// one fail means all fail.
	if (strcmp(GetTrackSimpleDescription(&pTracks[eTrack1]), "FAIL") == 0 ||
	                strcmp(GetTrackSimpleDescription(&pTracks[eTrack2]), "FAIL") == 0 ||
	                strcmp(GetTrackSimpleDescription(&pTracks[eTrack3]), "FAIL") == 0
	                                )
	{
		return "FAIL";

	} //all not present means  Not present.
	else if (strcmp(GetTrackSimpleDescription(&pTracks[eTrack1]), "Not present") == 0 &&
	                strcmp(GetTrackSimpleDescription(&pTracks[eTrack2]), "Not present") == 0 &&
	                strcmp(GetTrackSimpleDescription(&pTracks[eTrack3]), "Not present") == 0
	                                )
	{

		return "Not Present";

	}
	else
	{
		return "PASS";
	}

}

