/*!
 * © 2013 Gilbarco Inc.
 * Confidential and Proprietary
 *
 * @file GCardReaderManager.h
 * @author Guillermo Paris
 * @date Feb 13 2013
 * @copyright © 2013 Gilbarco Inc. Confidential and Propietary
 */

#ifndef GCardReaderManager_h
#define GCardReaderManager_h 1

#include "Services/CardReader/GCardReaderService.h"


const int CLR_LEGEND_TEMPLATE_LEN	= 40;
const int CLR_LEGEND_MAX_LEN 		= 64;

class GCardReaderMenu;


class GCardReaderManager
{
public:
	GCardReaderManager(GCardReaderMenu *);
	virtual              ~GCardReaderManager();

	enum eStatus
	{
		eShowInitial,			//show initial window
		eShowChipDetected,		//show chip detected window
		eShowPleaseRemoveCard,  //show please remove card window
		eShowTestResult,		//show simplified results window
		eShowDetails,		    //show detailed results window
		eShowStats				//show stats menu with
	};

	inline STATUS        GetDeviceStatus();
	inline void          EnableDevice();
	inline void          DisableDevice();
	void          		 NotifyCardInserted();
	void                 NotifyChipCardInserted();

	virtual void         NotifyCardRemoved(TRACK_INFO*);
	void                 GoToTestResultScreen();
	void                 GoToDetailsScreen();
	void 				 GoToStatistics();

	void                 Initialize();
	void                 Dispose();

	inline enum eStatus  GetStatus() {return m_Status;};
	bool                 ChangeStateTo(eStatus windowStatus);

	inline bool isACardWasRead()  {	return m_bACardWasRead;	};



private:

	inline 	void SetStatus(eStatus windowStatus) {m_Status = windowStatus;};
	const char*  GetTrackSimpleDescription(TRACK_INFO* pTrack);
	const char*  GetMagstripeSimpleDescription(TRACK_INFO* pTracks);

	static char          mst_arycLegendTemplate[CLR_LEGEND_TEMPLATE_LEN];
	static char          mst_arycLegendChipTemplate[CLR_LEGEND_TEMPLATE_LEN];
	GCardReaderMenu*     m_pCRMenu;
	GCardReaderService*  m_pCRService;
	enum eStatus         m_Status;
	char                 m_arycLegends[eTotalTracks][CLR_LEGEND_MAX_LEN];
	char                 m_cMagStripeLegend[CLR_LEGEND_MAX_LEN];
	char                 m_cChipCardLegend[CLR_LEGEND_MAX_LEN];
	static char          mst_arycMagStripeLegendTemplate[CLR_LEGEND_TEMPLATE_LEN];
	bool 				 m_bACardWasRead;

};


inline STATUS GCardReaderManager::GetDeviceStatus()
{
	return m_pCRService->GetStatus();
}

inline void GCardReaderManager::EnableDevice()
{
	m_pCRService->EnableDevice();
}

inline void GCardReaderManager::DisableDevice()
{
	m_pCRService->DisableDevice();
}





#endif // GCardReaderManager

