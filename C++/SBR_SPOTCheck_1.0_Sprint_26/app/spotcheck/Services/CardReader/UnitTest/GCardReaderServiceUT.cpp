/*
 * GCardReaderServiceUT.cpp
 *
 *  Created on: Feb 18 2013
 *  Author: Guillermo Paris
 */
#include <cstring>

#include "SPOT/SystemInformation/GGlobalSystemInformation.h"
#include "GCardReaderServiceUT.h"

#include "Common/Util/GConversion.h"
#include "Services/CardReader/GCardReaderService.h"

class GSPOTSession;


using namespace GSpotDefs;
using namespace std;

const int CR_SAMPLE_TRK_LEN = 16;


int  SPOTCheck_init(int ac, char **av)     // Only this can call CreateInstance()
{
	GGlobalSystemInformation::CreateInstance(NULL);
	return 0;
}

int  SPOTCheck_exit(GSPOTSession *pSession)// Only this can call DestroyInstance()
{
	GGlobalSystemInformation::DestroyInstance();
	return 0;
}


GCardReaderServiceUT::GCardReaderServiceUT()
	: m_pService(NULL)
{
	SPOTCheck_init(0, NULL);
}

GCardReaderServiceUT::~GCardReaderServiceUT()
{
	SPOTCheck_exit(NULL);
}

void GCardReaderServiceUT::SetUp()
{
	m_pService = GCardReaderService::CreateInstance(NULL);
}

void GCardReaderServiceUT::TearDown()
{
	GCardReaderService::DestroyInstance();
}

TEST_F(GCardReaderServiceUT, TestCardCycle)
{
	UINT8  aryu8CardReaderMsg[48];

	ASSERT_TRUE( m_pService != NULL);

	memset(aryu8CardReaderMsg, 0, 48);
	EXPECT_TRUE( m_pService->GetStatus() == GCardReaderService::eStatusInitial ); // initial status

	m_pService->EnableDevice();

	m_pService->ProcessSystemStatusNotification(0x01, 0x81, 0);
	EXPECT_TRUE( m_pService->GetStatus() == GCardReaderService::eStatusCardInserted ); // Card Inserted status

	GConversion::ASCIIHexStringToBytes(aryu8CardReaderMsg,
		"00 26 81 00 20 03 00 03 01 00 00 08 47 69 6C 62 61 72 63 6F 02 00 00 06 56 65 65 64 65 72 04 00 00 04 52 6F 6F 74");

	GSPOTMessage oMsg(aryu8CardReaderMsg);
	m_pService->ProcessCardReaderReply(&oMsg);
	EXPECT_TRUE( m_pService->GetStatus() == GCardReaderService::eStatusDataArrived ); // Data Arrived status

	ASSERT_STREQ( (const char *)m_pService->GetTrackInfo(eTrack1).m_aryu8Data, "Gilbarco" );
	ASSERT_STREQ( (const char *)m_pService->GetTrackInfo(eTrack2).m_aryu8Data, "Veeder" );
	ASSERT_STREQ( (const char *)m_pService->GetTrackInfo(eTrack3).m_aryu8Data, "Root" );

	m_pService->ProcessSystemStatusNotification(0x01, 0x80, 0);
	EXPECT_TRUE( m_pService->GetStatus() == GCardReaderService::eStatusCardOut ); // initial Card Out status
}

TEST_F(GCardReaderServiceUT, TestChipCardCycle)
{
	UINT8  aryu8CardReaderMsg[48];
	UINT8  aryu8ATR[6];

	ASSERT_TRUE( m_pService != NULL );

	memset(aryu8CardReaderMsg, 0, 48);
	EXPECT_TRUE( m_pService->GetStatus() == GCardReaderService::eStatusInitial ); // initial status

	m_pService->EnableDevice();

	m_pService->ProcessSystemStatusNotification(0x01, 0x83, 0);
	EXPECT_TRUE( m_pService->GetStatus() == GCardReaderService::eStatusChipCardInserted ); // Card Inserted status


	GConversion::ASCIIHexStringToBytes(aryu8CardReaderMsg,
		"00 1E 81 00 20 03 00 04 01 01 00 00 02 01 00 00 04 01 00 00 08 00 00 06 3B 62 00 00 81 82");

	GSPOTMessage oMsg(aryu8CardReaderMsg);
	m_pService->ProcessCardReaderReply(&oMsg);
	EXPECT_TRUE( m_pService->GetStatus() == GCardReaderService::eStatusATRDataArrived ); // Data Arrived status

	GConversion::ASCIIHexStringToBytes(aryu8ATR, "3B 62 00 00 81 82");
	EXPECT_TRUE( memcmp(m_pService->GetTrackInfo(eTrack4).m_aryu8Data, aryu8ATR, sizeof(aryu8ATR)) == 0 );

	m_pService->ProcessSystemStatusNotification(0x01, 0x83, 0);

	GConversion::ASCIIHexStringToBytes(aryu8CardReaderMsg,
		"00 26 81 00 20 03 00 03 01 00 00 08 47 69 6C 62 61 72 63 6F 02 00 00 06 56 65 65 64 65 72 04 00 00 04 52 6F 6F 74");

	GSPOTMessage oMsg2(aryu8CardReaderMsg);
	m_pService->ProcessCardReaderReply(&oMsg2);
	EXPECT_TRUE( m_pService->GetStatus() == GCardReaderService::eStatusDataArrived ); // Data Arrived status

	ASSERT_STREQ( (const char *)m_pService->GetTrackInfo(eTrack1).m_aryu8Data, "Gilbarco" );
	ASSERT_STREQ( (const char *)m_pService->GetTrackInfo(eTrack2).m_aryu8Data, "Veeder" );
	ASSERT_STREQ( (const char *)m_pService->GetTrackInfo(eTrack3).m_aryu8Data, "Root" );


	m_pService->ProcessSystemStatusNotification(0x01, 0x80, 0);
	EXPECT_TRUE( m_pService->GetStatus() == GCardReaderService::eStatusCardOut ); // initial Card Out status
}


