#include <cstdio>
#include <cstring>

#include "GTrindManager.h"

#include "Common/Util/DEBUGOUT.h"
#include "Services/Trind/GTrindService.h"
#include "Menus/Trind/GTrindMenu.h"



GTrindManager::GTrindManager(GTrindMenu *pMenu) :
				m_pCRMenu(pMenu), m_Status(eShowInitial), m_pCRService(NULL)
{
}

GTrindManager::~GTrindManager()
{
}

// called after costruction
void GTrindManager::Initialize()
{
	m_pCRService = GTrindService::GetService();

	if (m_pCRService)
	{
		m_pCRService->AttachManager(this);
		EnableService();
	}

}

// called before destruction
void GTrindManager::Dispose()
{
	m_pCRService = GTrindService::GetService();
	if (m_pCRService)
	{
		DisableService();
		m_pCRService->DetachManager();
	}

}

void GTrindManager::ShowTagRead()
{
	if(ChangeStateTo(eShowTagRead))
		m_pCRMenu->ShowTagRead();
}

void GTrindManager::ShowStatsScreen()
{
	if(ChangeStateTo(eShowStats))
		m_pCRMenu->ShowStatsScreen();
}

void GTrindManager::IncrementTagCount(int result )
{
	if(result)
		m_pCRMenu->IncrementTagCount(GTrindMenu::eTrPass);
	else
		m_pCRMenu->IncrementTagCount(GTrindMenu::eTrFail);
}

void GTrindManager::SetLEDTestResult(bool On)
{
	if(On)
	{
		m_pCRMenu->SetLEDTestResult(GTrindMenu::eTRPass);
		m_pCRService->UpdateLEDStatistics(GTrindServiceStatistics::eTRPass);
	}
	else
	{
		m_pCRMenu->SetLEDTestResult(GTrindMenu::eTRFail);
		m_pCRService->UpdateLEDStatistics(GTrindServiceStatistics::eTRFail);
	}
}


bool GTrindManager::DrawTrindLEDCheckTestMenu(std::string text)
{
	if(ChangeStateTo(eShowLEDCheck))
	return m_pCRMenu->DrawTrindLEDCheckTestMenu(text);

	return false;
}


// check possible state changes
bool GTrindManager::ChangeStateTo(eStatus windowStatus)
{

	bool bShowScreen = false;

	switch (windowStatus)
	{
		case eShowInitial:
		{
				SetStatus(eShowInitial);
		}
		break;

		case eShowTagRead:

			if (m_Status == eShowInitial || m_Status == eShowStats)
			{
				//change state first just in case.
				SetStatus(eShowTagRead);
			}
		break;

		case eShowStats:

			if (m_Status == eShowInitial || m_Status == eShowTagRead || m_Status == eShowLEDCheck)
			{
				//change state first just in case.
				SetStatus(eShowStats);
			}
			else
			{
				LOG(eTrind, eWarning, "ChangeStateTo() invalid change to eShowStats.\n");
				return false; //wrong state change

			}
			break;


		case eShowLEDCheck:
			if (m_Status == eShowInitial)
			{
				SetStatus(eShowLEDCheck);
			}
			else
			{
				LOG(eTrind, eWarning, "ChangeStateTo() invalid change to eShowLEDCheck.\n");
				return false; //wrong state change
			}
			break;

	}

	return true;
}
