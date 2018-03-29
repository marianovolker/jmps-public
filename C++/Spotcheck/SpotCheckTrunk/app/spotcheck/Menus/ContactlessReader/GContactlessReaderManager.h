/*!
 * © 2013 Gilbarco Inc.
 * Confidential and Proprietary
 *
 * @file GContactlessReaderManager.h
 * @author Guillermo Paris
 * @date Feb 13 2013
 * @copyright © 2013 Gilbarco Inc. Confidential and Propietary
 */

#ifndef GContactlessReaderManager_h
#define GContactlessReaderManager_h 1

#include "Services/ContactlessReader/GContactlessReaderService.h"
#include "Services/ContactlessReader/GContactlessReaderServiceStatistics.h"

const int CLR_LEGEND_TEMPLATE_LEN 	= 40;
const int CLR_LEGEND_MAX_LEN 		= 64;

class GContactlessReaderMenu;

class GContactlessReaderManager
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

			eEventTimeout,
			eEventError,

			eLastEvent = (eEventError + 1)
		};

		struct sDataEvent
		{
			UINT8 data;
			UINT16 size;
		};

		GContactlessReaderManager(GContactlessReaderMenu *);
		virtual ~GContactlessReaderManager();

		void ProcessEventReceived(eEvents e, sDataEvent* pDataEvent = NULL);
		void NotifyCardDataResult(GContactlessReaderServiceStatistics::eTypeStatistics eResult);
		void NotifyContactlessReaderInPermanentError();

		inline void EnableDevice();
		inline void DisableDevice();
		void GoToTestResultScreen();
		void GoToStatistics();

		void Initialize();
		void Dispose();

		inline enum eStatus GetStatus() { return m_Status; }

	private:

		void NextState(eEvents e, eStatus s);
		const char* GetSimpleDescription(GContactlessReaderServiceStatistics::eTypeStatistics eResult);
		void processData(GContactlessReaderServiceStatistics::eTypeStatistics eResult);

		inline void SetStatus(eStatus windowStatus)
		{
			m_Status = windowStatus;
		};

		static const char * mst_arypcStatusLabel[eLastStatus];
		static const char * mst_arypcEventLabel[eLastEvent];

		static char mst_arycLegendTemplate[CLR_LEGEND_TEMPLATE_LEN];
		char m_cMagStripeLegend[CLR_LEGEND_MAX_LEN];

		GContactlessReaderMenu* m_pCLRMenu;
		GContactlessReaderService* m_pCLRService;
		enum eStatus m_Status;

};

inline void GContactlessReaderManager::EnableDevice()
{
	if (m_pCLRService)
	{
		m_pCLRService->EnableDevice();
	}
}

inline void GContactlessReaderManager::DisableDevice()
{
	if (m_pCLRService)
	{
		m_pCLRService->DisableDevice();
	}
}

#endif // GContactlessReaderManager
