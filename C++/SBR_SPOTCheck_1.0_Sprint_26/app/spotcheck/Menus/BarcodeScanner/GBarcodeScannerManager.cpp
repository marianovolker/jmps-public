#include <cstdio>
#include <cstring>

#include "GBarcodeScannerManager.h"

#include "Common/Util/DEBUGOUT.h"
#include "Common/Util/GConversion.h"
#include "Services/BarcodeScanner/GBarcodeScannerService.h"
#include "Menus/BarcodeScanner/GBarcodeScannerMenu.h"

#include "Services/Printer/GPrinterService.h"

char GBarcodeScannerManager::mst_arycLegendTemplate[BCS_LEGEND_TEMPLATE_LEN] = "Result: %s";

const char * GBarcodeScannerManager::mst_arypcStatusLabel[] =
{
	"eStatusInitial",
	"eStatusTestResult",
	"eStatusError",
	"eSatusStatistics"
};
const char * GBarcodeScannerManager::mst_arypcEventLabel[] =
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
	"eEventPrinted",

	"eEventTimeout",
	"eEventError"
};

GBarcodeScannerManager::GBarcodeScannerManager(GBarcodeScannerMenu *pMenu) :
				m_pBCSMenu(pMenu), m_Status(eStatusInitial), m_pBCSService(NULL),
				m_pTimer(NULL), m_TOutCommand(this, &GBarcodeScannerManager::TemporizedPrintBarcodeSample), m_strBarcode("")
{
	// this is a ONE SHOT timer / thread
	m_pTimer = new GTimer(&m_TOutCommand, BCS_TOUT_INITIAL_DELAY, true, false);
}

GBarcodeScannerManager::~GBarcodeScannerManager()
{
}

// called after costruction
void GBarcodeScannerManager::Initialize()
{
	m_pBCSService = GBarcodeScannerService::GetService();

	if (m_pBCSService)
	{
		m_pBCSService->AttachManager(this);
		EnableDevice();
	}

	CreateBarcodeSample();

	memset(m_cMagStripeLegend, 0, sizeof(m_cMagStripeLegend));
}

// called before destruction
void GBarcodeScannerManager::Dispose()
{
	m_pBCSService = GBarcodeScannerService::GetService();
	if (m_pBCSService)
	{
		DisableDevice();
		m_pBCSService->DetachManager();
	}
}

void GBarcodeScannerManager::NextState(eEvents e, eStatus s)
{
	LOG(eBarcode, eDebug, "[Manager][current_status[%s] ==>> event[%s] ==>> next_status[%s]]\n", mst_arypcStatusLabel[m_Status], mst_arypcEventLabel[e], mst_arypcStatusLabel[s]);
	m_Status = s;
}

void GBarcodeScannerManager::PrintBarcodeScreen()
{
	if (m_pBCSMenu)
	{
		m_pBCSMenu->ClearScreen();
		m_pBCSMenu->ShowBarcodeSamplePrintingScreen();
	}
}

void GBarcodeScannerManager::GoToTestResultScreen()
{
	if (m_pBCSMenu)
	{
		m_pBCSMenu->ClearScreen();
		m_pBCSMenu->ShowResultsScreen(m_cMagStripeLegend);
	}
}

void GBarcodeScannerManager::GoToStatistics()
{
	if (m_pBCSMenu)
	{
		m_pBCSMenu->ClearScreen();
		m_pBCSMenu->ShowStatsScreen();
	}
}

void GBarcodeScannerManager::NotifyDataResult(GBarcodeScannerServiceStatistics::eTypeStatistics peResult)
{
	sDataEvent* pData = new sDataEvent;
	pData->data = (UINT8) peResult;
	pData->size = sizeof(UINT8);

	ProcessEventReceived(eEventRead, pData);
}

void GBarcodeScannerManager::NotifyBarcodeScannerInPermanentError()
{
	memset(m_cMagStripeLegend, 0, sizeof(m_cMagStripeLegend));
	snprintf(&m_cMagStripeLegend[0], sizeof(m_cMagStripeLegend), "FATAL ERROR at barcodeScanner!!!");
	ProcessEventReceived(eEventError, NULL);
}

void GBarcodeScannerManager::CreateBarcodeSample()
{
	LOG(eBarcode, eTrace, "GBarcodeScannerManager::CreateBarcodeSample()\n");

	m_strBarcode = "1B 24 00 00 ";
	m_strBarcode+= "1B 64 20 ";
	m_strBarcode+= "0A 0A ";

	// ** Barcode example **.
	m_strBarcode+= "2A 2A 20 42 61 72 63 6F 64 65 20 65 78 61 6D 70 6C 65 20 2A 2A ";
	m_strBarcode+= "0A 0A ";

	//UPCA (123456789012):
	m_strBarcode+= "55 50 43 41 20 28 31 32 33 34 35 36 37 38 39 30 31 32 29 3A 0A ";
	m_strBarcode+= "1D 65 03 09 ";
	m_strBarcode+= "1D 68 5A ";
	m_strBarcode+= "1D 6B 41 0C ";
	m_strBarcode+= "31 32 33 34 35 36 37 38 39 30 31 32 ";
	m_strBarcode+= "0A 0A ";

	//UPCA (012000007897):
	m_strBarcode+= "55 50 43 41 20 28 30 31 32 30 30 30 30 30 37 38 39 37 29 3A 0A ";
	m_strBarcode+= "1D 65 03 09 ";
	m_strBarcode+= "1D 68 5A ";
	m_strBarcode+= "1D 6B 42 0C ";
	m_strBarcode+= "30 31 32 30 30 30 30 30 37 38 39 37 ";
	m_strBarcode+= "0A 0A ";

	//EAN13 (1234567890128):
	m_strBarcode+= "45 41 4E 31 33 20 28 31 32 33 34 35 36 37 38 39 30 31 32 38 29 3A 0A ";
	m_strBarcode+= "1D 65 03 09 ";
	m_strBarcode+= "1D 68 5A ";
	m_strBarcode+= "1D 6B 43 0D ";
	m_strBarcode+= "31 32 33 34 35 36 37 38 39 30 31 32 38 ";
	m_strBarcode+= "0A 0A ";

	//EAN8 (12345670):
	m_strBarcode+= "45 41 4E 38 20 28 31 32 33 34 35 36 37 30 29 3A 0A ";
	m_strBarcode+= "1D 65 03 09 ";
	m_strBarcode+= "1D 68 5A ";
	m_strBarcode+= "1D 6B 44 08 ";
	m_strBarcode+= "31 32 33 34 35 36 37 30 ";
	m_strBarcode+= "0A 0A ";

	//CODE39 (*F00273):
	m_strBarcode+= "43 4F 44 45 33 39 20 28 2A 46 30 30 32 37 33 29 3A 0A ";
	m_strBarcode+= "1D 65 02 06 ";
	m_strBarcode+= "1D 68 5A ";
	m_strBarcode+= "1D 6B 45 07 ";
	m_strBarcode+= "2A 46 30 30 32 37 33 ";
	m_strBarcode+= "0A 0A ";

	m_strBarcode+= "2A 2A 2A 2A 2A 2A 2A 2A 2A 2A 2A 2A 2A 2A 2A 2A 2A ";
	m_strBarcode+= "0A 0A ";

	m_strBarcode+= "1B 45 ";
	m_strBarcode+= "1B 43 ";
}

void GBarcodeScannerManager::TemporizedPrintBarcodeSample()
{
	LOG(eBarcode, eTrace, "GBarcodeScannerManager::TemporizedPrintBarcodeSample()\n");

	LOG(eBarcode, eTrace, "[%s]\n", m_strBarcode.c_str());

	UINT8* pBuffer = new UINT8[m_strBarcode.length()];
	UINT32 lenOfBuffer = 0;
	lenOfBuffer = GConversion::ASCIIHexStringToBytes(pBuffer, m_strBarcode.c_str());
	if( GPrinterService::GetService()->PrintText(lenOfBuffer, (const char *)pBuffer) )
	{
		processData(GBarcodeScannerServiceStatistics::ePass);
	}
	else
	{
		processData(GBarcodeScannerServiceStatistics::eFail);
	}

	ProcessEventReceived(eEventPrinted, NULL);

	delete[] pBuffer;
}

void GBarcodeScannerManager::ProcessEventReceived(eEvents e, sDataEvent* pDataEvent)
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
					m_pBCSMenu->Quit();
				}
				break;

				case eEventSoftKeyL3:
				{
					NextState(eEventSoftKeyR4, eSatusStatistics);
					DisableDevice();
					GoToStatistics();
				}
				break;

				case eEventSoftKeyL2:
				{
					NextState(eEventSoftKeyL2, eStatusInitial);
					PrintBarcodeScreen();
					m_pTimer->Start();
				}
				break;

				case eEventPrinted:
				{
					NextState(eEventPrinted, eStatusInitial);
					m_pTimer->Stop();
					GoToTestResultScreen();
				}
				break;

				case eEventSoftKeyL1:
				case eEventSoftKeyR1:
				case eEventSoftKeyR2:
				case eEventSoftKeyR3:
				case eEventSoftKeyR4:
					LOG(eBarcode, eTrace, "IGNORED EVENT [%s] received in [%s].\n", mst_arypcEventLabel[e], mst_arypcStatusLabel[m_Status]);
				break;

				case eEventRead:
				{
					NextState(eEventRead, eStatusInitial);
					if(pDataEvent != NULL)
					{
						processData((GBarcodeScannerServiceStatistics::eTypeStatistics)(pDataEvent->data));
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
					LOG(eBarcode, eError, "ERROR: INVALID EVENT [%s] received in [%s].\n", mst_arypcEventLabel[e], mst_arypcStatusLabel[m_Status]);
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
					LOG(eBarcode, eError, "ERROR: INVALID EVENT [%s] received in [%s].\n", mst_arypcEventLabel[e], mst_arypcStatusLabel[m_Status]);
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
					LOG(eBarcode, eError, "ERROR: event [%s] received in eStatusError. The Display device is in unrecoverable state.\n", mst_arypcEventLabel[e]);
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

const char* GBarcodeScannerManager::GetSimpleDescription(GBarcodeScannerServiceStatistics::eTypeStatistics eResult)
{

	if ( eResult == GBarcodeScannerServiceStatistics::ePass )
	{
		return "PASS";
	}
	else
	{
		return "FAIL";
	}
}

void GBarcodeScannerManager::processData(GBarcodeScannerServiceStatistics::eTypeStatistics eResult)
{
	memset(m_cMagStripeLegend, 0, sizeof(m_cMagStripeLegend));

	snprintf(&m_cMagStripeLegend[0], sizeof(m_cMagStripeLegend), mst_arycLegendTemplate, GetSimpleDescription(eResult));

	LOG(eBarcode, eInfo, "GBarcodeScannerManager::processData(). Printing simple description for tracks\n");
}
