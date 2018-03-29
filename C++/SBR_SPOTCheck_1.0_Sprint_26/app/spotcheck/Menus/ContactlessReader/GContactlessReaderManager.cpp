#include <cstdio>
#include <cstring>

#include "GContactlessReaderManager.h"

#include "Common/Util/DEBUGOUT.h"
#include "Services/ContactlessReader/GContactlessReaderService.h"
#include "Menus/ContactlessReader/GContactlessReaderMenu.h"

char GContactlessReaderManager::mst_arycLegendTemplate[CLR_LEGEND_TEMPLATE_LEN] = "Result: %s";

const char * GContactlessReaderManager::mst_arypcStatusLabel[] =
{
	"eStatusInitial",
	"eStatusTestResult",
	"eStatusError",
	"eSatusStatistics"
};
const char * GContactlessReaderManager::mst_arypcEventLabel[] =
{
	"eEventSoftKeyL1",
	"eEventSoftKeyL2",
	"eEventSoftKeyL3",
	"eEventSoftKeyL4",

	"eEventSoftKeyR1",
	"eEventSoftKeyR2",
	"eEventSoftKeyR3",
	"eEventSoftKeyR4",
	"eEventRead",

	"eEventTimeout",
	"eEventError"
};

GContactlessReaderManager::GContactlessReaderManager(GContactlessReaderMenu *pMenu) :
				m_pCLRMenu(pMenu), m_Status(eStatusInitial), m_pCLRService(NULL)
{
}

GContactlessReaderManager::~GContactlessReaderManager()
{
}

// called after costruction
void GContactlessReaderManager::Initialize()
{


	m_pCLRService = GContactlessReaderService::GetService();

	if (m_pCLRService)
	{
		m_pCLRService->AttachManager(this);
		EnableDevice();
	}

	memset(m_cMagStripeLegend, 0, sizeof(m_cMagStripeLegend));


}

// called before destruction
void GContactlessReaderManager::Dispose()
{
	m_pCLRService = GContactlessReaderService::GetService();
	if (m_pCLRService)
	{
		DisableDevice();
		m_pCLRService->DetachManager();
	}
}

void GContactlessReaderManager::NextState(eEvents e, eStatus s)
{
	LOG(eContactlessReader, eDebug, "[Manager][current_status[%s] ==>> event[%s] ==>> next_status[%s]]\n", mst_arypcStatusLabel[m_Status], mst_arypcEventLabel[e], mst_arypcStatusLabel[s]);
	m_Status = s;
}

void GContactlessReaderManager::GoToTestResultScreen()
{
	if (m_pCLRMenu)
	{
		m_pCLRMenu->ClearScreen();
		m_pCLRMenu->ShowResultsScreen(m_cMagStripeLegend);
	}
}

void GContactlessReaderManager::GoToStatistics()
{
	if (m_pCLRMenu)
	{
		m_pCLRMenu->ClearScreen();
		m_pCLRMenu->ShowStatsScreen();
	}
}

void GContactlessReaderManager::NotifyCardDataResult(GContactlessReaderServiceStatistics::eTypeStatistics peResult)
{
	sDataEvent* pData = new sDataEvent;
	pData->data = (UINT8) peResult;
	pData->size = sizeof(UINT8);

	ProcessEventReceived(eEventRead, pData);
}

void GContactlessReaderManager::NotifyContactlessReaderInPermanentError()
{
	memset(m_cMagStripeLegend, 0, sizeof(m_cMagStripeLegend));
	snprintf(&m_cMagStripeLegend[0], sizeof(m_cMagStripeLegend), "FATAL ERROR at contactless reader!!!");
	ProcessEventReceived(eEventError, NULL);
}

void GContactlessReaderManager::ProcessEventReceived(eEvents e, sDataEvent* pDataEvent)
{
	switch (m_Status)
	{
		//======================================================================================================
		case eStatusInitial:
		{
			switch (e)
			{
				case eEventSoftKeyL4:
				{
					NextState(eEventSoftKeyL4, eStatusInitial);
					DisableDevice();
					m_pCLRMenu->Quit();
				}
				break;

				case eEventSoftKeyL3:
				{
					NextState(eEventSoftKeyR4, eSatusStatistics);
					DisableDevice();
					GoToStatistics();
				}
				break;

				case eEventSoftKeyL1:
				case eEventSoftKeyL2:
				case eEventSoftKeyR1:
				case eEventSoftKeyR2:
				case eEventSoftKeyR3:
				case eEventSoftKeyR4:
					LOG(eContactlessReader, eTrace, "IGNORED EVENT [%s] received in [%s].\n", mst_arypcEventLabel[e], mst_arypcStatusLabel[m_Status]);
				break;

				case eEventRead:
				{
					NextState(eEventRead, eStatusInitial);
					if(pDataEvent != NULL)
					{
						processData((GContactlessReaderServiceStatistics::eTypeStatistics)(pDataEvent->data));
					}
					GoToTestResultScreen();
				}
				break;

				case eEventError:
				{
					NextState(eEventError, eStatusInitial);
				}
				break;

				default:
					LOG(eContactlessReader, eError, "ERROR: INVALID EVENT [%s] received in [%s].\n", mst_arypcEventLabel[e], mst_arypcStatusLabel[m_Status]);
				break;
			}
		}
		break;
		//======================================================================================================

		//======================================================================================================
		case eSatusStatistics:
		{
			switch (e)
			{
				case eEventSoftKeyL4:
				{
					NextState(eEventSoftKeyL4, eStatusInitial);
					EnableDevice();
					GoToTestResultScreen();
				}
				break;

				case eEventError:
				{
					NextState(eEventError, eSatusStatistics);
				}
				break;

				default:
					LOG(eContactlessReader, eError, "ERROR: INVALID EVENT [%s] received in [%s].\n", mst_arypcEventLabel[e], mst_arypcStatusLabel[m_Status]);
				break;
			}
		}
		break;

		//======================================================================================================
		case eStatusError:
		{
			switch (e)
			{
				default:
					LOG(eContactlessReader, eError, "ERROR: event [%s] received in eStatusError. The Display device is in unrecoverable state.\n", mst_arypcEventLabel[e]);
					NextState(e, eStatusError);
				break;
			}
		}
		break;
		//======================================================================================================
	}

	if(pDataEvent != NULL)
	{
		delete pDataEvent;
		pDataEvent = NULL;
	}
}

const char* GContactlessReaderManager::GetSimpleDescription(GContactlessReaderServiceStatistics::eTypeStatistics eResult)
{

	if ( eResult == GContactlessReaderServiceStatistics::ePass )
	{
		return "PASS";
	}
	else
	{
		return "FAIL";
	}
}

void GContactlessReaderManager::processData(GContactlessReaderServiceStatistics::eTypeStatistics eResult)
{
	memset(m_cMagStripeLegend, 0, sizeof(m_cMagStripeLegend));

	snprintf(&m_cMagStripeLegend[0], sizeof(m_cMagStripeLegend), mst_arycLegendTemplate, GetSimpleDescription(eResult));

	LOG(eCardReader, eInfo, "GContactlessReaderManager::processTrackData(). Printing simple description for tracks\n");
}
