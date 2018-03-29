/*!
 * © 2015 Gilbarco Inc.
 * Confidential and Proprietary
 *
 * @file GTrindManager.h
 * @author Sayyed Mohammad
 * @date Feb 10 2015
 * @copyright © 2015 Gilbarco Inc. Confidential and Propietary
 */

#ifndef GTrindManager_h
#define GTrindManager_h 1

#include "Services/Trind/GTrindService.h"


class GTrindMenu;

class GTrindManager
{
public:
	GTrindManager(GTrindMenu *);
	virtual              ~GTrindManager();

	enum eStatus
	{
		eShowInitial,			//show initial window
		eShowLEDCheck,
		eShowTagRead,
		eShowStats, //show stats menu with
		eShowError,
	};

	//inline STATUS        GetServiceStatus();
	inline void          EnableService();
	inline void          DisableService();
	inline void          EnableLED();
	inline void          DisableLED();
	void          		 NotifyCardInserted();
	void                 NotifyChipCardInserted();
	void                 NotifyProcessingCard();

	void                 ShowTagRead();
	void                 ShowStatsScreen();
	void                 IncrementTagCount(int result);
	void                 SetLEDTestResult(bool On);
	bool                 DrawTrindLEDCheckTestMenu(std::string);

	void                 Initialize();
	void                 Dispose();

	inline enum eStatus  GetStatus() {return m_Status;};
	bool                 ChangeStateTo(eStatus windowStatus);




private:

	inline 	void SetStatus(eStatus windowStatus) {m_Status = windowStatus;};

	GTrindMenu*     m_pCRMenu;
	GTrindService*  m_pCRService;
	enum eStatus         m_Status;

};

/*
inline STATUS GTrindManager::GetServiceStatus()
{
	//return m_pCRService->GetStatus();
	return 0;
}*/

inline void GTrindManager::EnableService()
{
	LOG(eTrind, eDebug,"GTrindManager::EnableService()\n");
	m_pCRService->Enable();


}

inline void GTrindManager::DisableService()
{
	m_pCRService->Disable();
}


inline void GTrindManager::EnableLED()
{
	LOG(eTrind, eDebug,"GTrindManager::EnableLED()\n");
	m_pCRService->LEDRequestOn();

}

inline void GTrindManager::DisableLED()
{
	LOG(eTrind, eDebug,"GTrindManager::DisableLED()\n");
	m_pCRService->LEDRequestOff();


}




#endif // GTrindManager

