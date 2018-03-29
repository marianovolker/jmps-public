/*
 * GCardReaderManagerUT.cpp
 *
 *  Created on: Feb 18 2013
 *  Author: Guillermo Paris
 */
#include <cstring>

#include "GCardReaderManagerUT.h"

#include "SPOT/SPOTProtocolInterface/GSPOTDefs.h"

using namespace GSpotDefs;
using namespace std;

GTestedCardReaderManager::GTestedCardReaderManager()
:
				GCardReaderManager(NULL) // we don't use the menu in the test yet.
{
}

GTestedCardReaderManager::~GTestedCardReaderManager()
{
}

GCardReaderManagerUT::GCardReaderManagerUT()
:
				m_pManager(NULL)
{
}

GCardReaderManagerUT::~GCardReaderManagerUT()
{
}

void GCardReaderManagerUT::SetUp()
{
	m_pManager = new GTestedCardReaderManager();
}

void GCardReaderManagerUT::TearDown()
{

	if (m_pManager)
	{
		delete m_pManager;
	}
}

TEST_F(GCardReaderManagerUT, TestStates)
{

	ASSERT_TRUE(m_pManager != NULL);

	EXPECT_TRUE(m_pManager->GetStatus() == GCardReaderManager::eShowInitial);

	m_pManager->ChangeStateTo(GCardReaderManager::eShowChipDetected);

	EXPECT_TRUE(m_pManager->GetStatus() == GCardReaderManager::eShowChipDetected);

	m_pManager->ChangeStateTo(GCardReaderManager::eShowPleaseRemoveCard);

	EXPECT_TRUE(m_pManager->GetStatus() == GCardReaderManager::eShowPleaseRemoveCard);

	m_pManager->ChangeStateTo(GCardReaderManager::eShowDetails);

	//should not be possible to change to  state
	EXPECT_FALSE(m_pManager->GetStatus() == GCardReaderManager::eShowDetails);

	m_pManager->ChangeStateTo(GCardReaderManager::eShowTestResult);

	EXPECT_TRUE(m_pManager->GetStatus() == GCardReaderManager::eShowTestResult);

	m_pManager->ChangeStateTo(GCardReaderManager::eShowDetails);

	EXPECT_TRUE(m_pManager->GetStatus() == GCardReaderManager::eShowDetails);

	//go to results again
	m_pManager->ChangeStateTo(GCardReaderManager::eShowTestResult);

	EXPECT_TRUE(m_pManager->GetStatus() == GCardReaderManager::eShowTestResult);

}

