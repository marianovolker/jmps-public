/*
 * GKeypadManagerUT.cpp
 *
 *  Author: Guillermo Paris
 *  Created on: Feb 18 2013
 *  Modified Jul 23 2013
 */
#include <cstring>

#include "SPOT/SPOTProtocolInterface/GSPOTDefs.h"
#include "SPOT/SystemInformation/GGlobalSystemInformation.h"
#include "Services/Display/GDisplayService.h"
#include "GKeypadManagerUT.h"
#include "Services/Resource/GResourcesService.h"

using namespace GSpotDefs;
using namespace std;


GTestedKeypadManager::GTestedKeypadManager()
:GKeypadManager(NULL) // we don't use the menu in this test
{
}

GTestedKeypadManager::~GTestedKeypadManager()
{
}


int SPOTCheck_init(int ac, char **av) // For various singletons having this as a "friend"
{
	GSPOTSession* p_NullSession = NULL; // no session, no message to SPOT in UT

	GDisplayService::CreateInstance(p_NullSession);
	GGlobalSystemInformation::CreateInstance(p_NullSession);
	return 0;
}


GKeypadManagerUT::GKeypadManagerUT()
	: m_pManager(NULL)
{
}

GKeypadManagerUT::~GKeypadManagerUT()
{
}

void GKeypadManagerUT::SetUp()
{
	SPOTCheck_init(0, NULL);

	GResourcesService::CreateInstance(NULL);

	m_pManager = new GTestedKeypadManager();
}

void GKeypadManagerUT::TearDown()
{

	if(m_pManager)
	{
		delete m_pManager;
	}
}


TEST_F(GKeypadManagerUT, TestStates)
{
	ASSERT_TRUE( m_pManager != NULL);

	EXPECT_TRUE(m_pManager->GetMenuStatus() == GKeypadManager::eShowInitial);

	m_pManager->ProposeChangeStateTo(GKeypadManager::eShowTest);

	EXPECT_TRUE(m_pManager->GetMenuStatus() == GKeypadManager::eShowTest);

	m_pManager->ProposeChangeStateTo(GKeypadManager::eShowStats);

	// It should not change from Test to Statistics state.
	EXPECT_TRUE(m_pManager->GetMenuStatus() == GKeypadManager::eShowTest);

	m_pManager->ProposeChangeStateTo(GKeypadManager::eShowInitial);

	EXPECT_TRUE(m_pManager->GetMenuStatus() == GKeypadManager::eShowInitial);

	m_pManager->ProposeChangeStateTo(GKeypadManager::eShowStats);

	EXPECT_TRUE(m_pManager->GetMenuStatus() == GKeypadManager::eShowStats);

	m_pManager->ProposeChangeStateTo(GKeypadManager::eShowInitial);

	EXPECT_TRUE(m_pManager->GetMenuStatus() == GKeypadManager::eShowInitial);
}


