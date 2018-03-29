/*
 * GKeypadServiceUT.cpp
 *
 *  Created on: Feb 18 2013
 *  Author: Mariano Volker
 */
#include <cstring>

#include "GKeypadServiceUT.h"

#include "Common/Util/GConversion.h"
#include "Services/Keypad/GKeypadService.h"
#include "Menus/Keypad/GKeypadManager.h"
#include "SPOT/SystemInformation/GGlobalSystemInformation.h"
#include "Services/Resource/GResourcesService.h"
class GSPOTSession;

using namespace GSpotDefs;
using namespace std;


int  SPOTCheck_init(int ac, char **av)     // Only this can call CreateInstance()
{
	GGlobalSystemInformation::CreateInstance(NULL);
	GResourcesService::CreateInstance(NULL);
	return 0;
}

int  SPOTCheck_exit(GSPOTSession *pSession)// Only this can call DestroyInstance()
{
	GGlobalSystemInformation::DestroyInstance();
	GResourcesService::DestroyInstance();
	return 0;
}


GKeypadServiceUT::GKeypadServiceUT()
	: m_pService(NULL)
{
	SPOTCheck_init(0,NULL);
}

GKeypadServiceUT::~GKeypadServiceUT()
{
	SPOTCheck_exit(NULL);
}

void GKeypadServiceUT::SetUp()
{
	m_pService = GKeypadService::CreateInstance(NULL);
	m_pManager = new GKeypadManager(NULL);
	m_pManager->Initialize();
}

void GKeypadServiceUT::TearDown()
{
	GKeypadService::DestroyInstance();
}


TEST_F(GKeypadServiceUT, TestKeypadEnableDisableBySystemNotificationEvent)
{
	ASSERT_TRUE( m_pService != NULL);

	m_pService->ProcessSystemStatusNotification(0x00, 0x80, 0x00);
	EXPECT_TRUE( m_pService->GetStatus() == GKeypadService::eDisabled );

	m_pService->ProcessSystemStatusNotification(0x01, 0x80, 0x00);
	EXPECT_TRUE( m_pService->GetStatus() == GKeypadService::eEnabled );

	m_pService->ProcessSystemStatusNotification(0x02, 0x80, 0x00);
	EXPECT_TRUE( m_pService->GetStatus() == GKeypadService::eBusy );

	m_pService->ProcessSystemStatusNotification(0x01, 0x80, 0x02);
	EXPECT_TRUE( m_pService->GetStatus() == GKeypadService::eKPError );
}

TEST_F(GKeypadServiceUT, TestKeypadCANCELKeyPressed)
{
	ASSERT_TRUE( m_pService != NULL);

	UINT8  aryu8KeypadMsg[48];

	memset(aryu8KeypadMsg, 0, sizeof(aryu8KeypadMsg));
	GConversion::ASCIIHexStringToBytes(aryu8KeypadMsg, "00 16 81 00 40 01 00 00 01 0C 00 00 00 00 00 00 00 00 00 00 00 00");

	GSPOTMessage oMsg(aryu8KeypadMsg);

	m_pService->ProcessKeypadReply(&oMsg);

	EXPECT_TRUE( m_pService->GetKeyStatistics((UINT8)GKeypadService::eKeyCancel) == 1 );

	m_pService->ProcessKeypadReply(&oMsg);

	EXPECT_TRUE( m_pService->GetKeyStatistics((UINT8)GKeypadService::eKeyCancel) == 2 );
}

TEST_F(GKeypadServiceUT, TestKeypadENTERKeyPressed)
{
	ASSERT_TRUE( m_pService != NULL);

	UINT8  aryu8KeypadMsg[48];

	memset(aryu8KeypadMsg, 0, sizeof(aryu8KeypadMsg));
	GConversion::ASCIIHexStringToBytes(aryu8KeypadMsg, "00 16 81 00 40 01 00 00 01 0D 00 00 00 00 00 00 00 00 00 00 00 00");

	GSPOTMessage oMsg(aryu8KeypadMsg);

	m_pService->ProcessKeypadReply(&oMsg);

	EXPECT_TRUE( m_pService->GetKeyStatistics((UINT8)GKeypadService::eKeyEnter) == 1 );

	m_pService->ProcessKeypadReply(&oMsg);

	EXPECT_TRUE( m_pService->GetKeyStatistics((UINT8)GKeypadService::eKeyEnter) == 2 );

	m_pService->ProcessKeypadReply(&oMsg);

	EXPECT_TRUE( m_pService->GetKeyStatistics((UINT8)GKeypadService::eKeyEnter) == 3 );
}

TEST_F(GKeypadServiceUT, TestKeypadCLEARKeyPressed)
{
	ASSERT_TRUE( m_pService != NULL);

	UINT8  aryu8KeypadMsg[48];

	memset(aryu8KeypadMsg, 0, sizeof(aryu8KeypadMsg));
	GConversion::ASCIIHexStringToBytes(aryu8KeypadMsg, "00 16 81 00 40 01 00 00 01 0E 00 00 00 00 00 00 00 00 00 00 00 00");

	GSPOTMessage oMsg(aryu8KeypadMsg);

	m_pService->ProcessKeypadReply(&oMsg);

	EXPECT_TRUE( m_pService->GetKeyStatistics((UINT8)GKeypadService::eKeyClear) == 1 );

	m_pService->ProcessKeypadReply(&oMsg);

	EXPECT_TRUE( m_pService->GetKeyStatistics((UINT8)GKeypadService::eKeyClear) == 2 );

	m_pService->ProcessKeypadReply(&oMsg);

	EXPECT_TRUE( m_pService->GetKeyStatistics((UINT8)GKeypadService::eKeyClear) == 3 );
}

TEST_F(GKeypadServiceUT, TestKeypadDigit4KeyPressed)
{
	ASSERT_TRUE( m_pService != NULL);

	UINT8  aryu8KeypadMsg[48];

	memset(aryu8KeypadMsg, 0, sizeof(aryu8KeypadMsg));
	GConversion::ASCIIHexStringToBytes(aryu8KeypadMsg, "00 16 81 00 40 01 00 00 01 34 00 00 00 00 00 00 00 00 00 00 00 00");

	GSPOTMessage oMsg(aryu8KeypadMsg);

	m_pService->ProcessKeypadReply(&oMsg);

	EXPECT_TRUE( m_pService->GetKeyStatistics((UINT8)GKeypadService::eKey04) == 1 );

	m_pService->ProcessKeypadReply(&oMsg);

	EXPECT_TRUE( m_pService->GetKeyStatistics((UINT8)GKeypadService::eKey04) == 2 );

	m_pService->ProcessKeypadReply(&oMsg);

	EXPECT_TRUE( m_pService->GetKeyStatistics((UINT8)GKeypadService::eKey04) == 3 );
}


