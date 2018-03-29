/*!
 * © 2013 Gilbarco Inc.
 * Confidential and Proprietary
 *
 * @file GBarcodeScannerManager.h
 * @author Mariano Volker
 * @date Jul 17 2013
 * @copyright © 2013 Gilbarco Inc. Confidential and Propietary
 */

#ifndef GBarcodeScannerManager_h
#define GBarcodeScannerManager_h 1

#include "Services/BarcodeScanner/GBarcodeScannerService.h"
#include "Services/BarcodeScanner/GBarcodeScannerServiceStatistics.h"

const int BCS_LEGEND_TEMPLATE_LEN 	= 40;
const int BCS_LEGEND_MAX_LEN 		= 64;
const int BCS_TOUT_INITIAL_DELAY   	= 50; // 50 mSec

class GBarcodeScannerMenu;

class GBarcodeScannerManager
{
	public:

		enum eStatus
		{
			eStatusInitial,
			eStatusTestResult,
			eStatusError,
			eSatusStatistics,
			eLastStatus = (eSatusStatistics + 1)
		};

		enum eEvents
		{
			eEventSoftKeyL1 = 0,
			eEventSoftKeyL2,
			eEventSoftKeyL3,
			eEventSoftKeyL4,
			eEventSoftKeyR1,
			eEventSoftKeyR2,
			eEventSoftKeyR3,
			eEventSoftKeyR4,
			eEventRead,
			eEventPrinted,

			eEventTimeout,
			eEventError,

			eLastEvent = (eEventError + 1)
		};

		struct sDataEvent
		{
			UINT8 data;
			UINT16 size;
		};

		GBarcodeScannerManager(GBarcodeScannerMenu *);
		virtual ~GBarcodeScannerManager();

		void ProcessEventReceived(eEvents e, sDataEvent* pDataEvent = NULL);
		void NotifyDataResult(GBarcodeScannerServiceStatistics::eTypeStatistics eResult);
		void NotifyBarcodeScannerInPermanentError();

		inline void EnableDevice();
		inline void DisableDevice();
		void GoToTestResultScreen();
		void PrintBarcodeScreen();
		void GoToStatistics();

		void Initialize();
		void Dispose();

		inline enum eStatus GetStatus() { return m_Status; }

	private:

		void NextState(eEvents e, eStatus s);
		const char* GetSimpleDescription(GBarcodeScannerServiceStatistics::eTypeStatistics eResult);
		void processData(GBarcodeScannerServiceStatistics::eTypeStatistics eResult);
		void CreateBarcodeSample();
		void TemporizedPrintBarcodeSample();

		inline void SetStatus(eStatus windowStatus) { m_Status = windowStatus; };

		static const char * mst_arypcStatusLabel[eLastStatus];
		static const char * mst_arypcEventLabel[eLastEvent];

		static char mst_arycLegendTemplate[BCS_LEGEND_TEMPLATE_LEN];
		char m_cMagStripeLegend[BCS_LEGEND_MAX_LEN];

		GBarcodeScannerMenu* m_pBCSMenu;
		GBarcodeScannerService* m_pBCSService;
		GTimer* m_pTimer;
		GSimpleCommand<GBarcodeScannerManager> m_TOutCommand;
		enum eStatus m_Status;
		std::string m_strBarcode;
};

inline void GBarcodeScannerManager::EnableDevice()
{
	if (m_pBCSService)
	{
		m_pBCSService->EnableDevice();
	}
}

inline void GBarcodeScannerManager::DisableDevice()
{
	if (m_pBCSService)
	{
		m_pBCSService->DisableDevice();
	}
}

#endif // GBarcodeScannerManager
