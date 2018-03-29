/*
 * GContactlessReaderServiceUT.cpp
 *
 *  Created on: Jul 8 2013
 *  Author: Mariano Volker
 */
#include <cstring>

#include "GContactlessReaderServiceUT.h"

#include "Common/Util/GConversion.h"
#include "Services/ContactlessReader/GContactlessReaderService.h"

class GSPOTSession;


using namespace GSpotDefs;
using namespace std;

const int CR_SAMPLE_TRK_LEN = 16;


GContactlessReaderServiceUT::GContactlessReaderServiceUT()
	: m_pService(NULL)
{
}

GContactlessReaderServiceUT::~GContactlessReaderServiceUT()
{
}

void GContactlessReaderServiceUT::SetUp()
{
	m_pService = GContactlessReaderService::CreateInstance(NULL);
}

void GContactlessReaderServiceUT::TearDown()
{
	GContactlessReaderService::DestroyInstance();
}

void GContactlessReaderServiceUT::ConvertToDecimal(UINT8* pui8Buffer, UINT16 u16lenclp, std::string& strOut)
{
	UINT16 u16Len = ((u16lenclp * 3) + 1);

	char* pcDataAux = new char[u16Len];

	memset(pcDataAux, 0x00, u16Len);

	GConversion::BytesToASCIIHexString(pcDataAux, pui8Buffer, u16lenclp);

	std::string str;

	for (int i = 0; i < u16Len; i++)
	{
		if (pcDataAux[i] != ' ') str += pcDataAux[i];
	}

	//printf("u16Len: [%d], ASCIIHex: [%s], str.size(): [%d]\n", u16Len, str.c_str(), str.size());

	char* pu8OutDecimalBuffer = new char[u16Len];

	memset(pu8OutDecimalBuffer, 0x00, u16Len);

	GConversion::ConvertHexBufferToDecimalBuffer((const UINT8*) str.c_str(), (str.size() - 1), (UINT8*) pu8OutDecimalBuffer);

	//printf("Decimal: [%s]\n", pu8OutDecimalBuffer);

	strOut.assign(pu8OutDecimalBuffer);

	delete[] pcDataAux;
}

TEST_F(GContactlessReaderServiceUT, TestCardCycle)
{
	UINT8  aryu8CardReaderMsg[190];

	ASSERT_TRUE( m_pService != NULL);

	memset(aryu8CardReaderMsg, 0, 190);
	EXPECT_TRUE( m_pService->GetStatus() == GContactlessReaderService::eStatusInitial );

	m_pService->EnableDevice();

	EXPECT_TRUE( m_pService->GetStatus() == GContactlessReaderService::eStatusWaitingRFCardData );

	GConversion::ASCIIHexStringToBytes(aryu8CardReaderMsg,
		"00 70 84 00 50 04 00 2F 02 01 00 3C 42 35 32 35 36 38 33 30 30 36 30 30 30 30 30 32 33 5E 53 4D 49 54 48 2F 4A 4F 48 4E 5E 30 35 30 38 31 30 31 33 33 37 38 39 33 33 33 30 30 30 32 32 32 32 32 30 30 30 31 31 31 31 30 02 00 25 35 32 35 36 38 33 30 30 36 30 30 30 30 30 32 33 3D 30 35 30 38 31 30 31 39 30 30 30 39 39 30 30 30 33 32 31 30");

	GSPOTMessage oMsg(aryu8CardReaderMsg);
	m_pService->ProcessContactlessReaderReply(&oMsg);
	EXPECT_TRUE( m_pService->GetStatus() == GContactlessReaderService::eStatusDataArrived ); // Data Arrived status

	std::string strTrack1;
	ConvertToDecimal(m_pService->GetTrackInfo(GContactlessReaderService::eTrack1).m_aryu8Data,
					 m_pService->GetTrackInfo(GContactlessReaderService::eTrack1).m_u16Length,
					 strTrack1);
	std::string strTrack2;
	ConvertToDecimal(m_pService->GetTrackInfo(GContactlessReaderService::eTrack2).m_aryu8Data,
					 m_pService->GetTrackInfo(GContactlessReaderService::eTrack2).m_u16Length,
					 strTrack2);

	ASSERT_STREQ( strTrack1.c_str(), "B5256830060000023^SMITH/JOHN^0508101337893330002222200011110" );
	ASSERT_STREQ( strTrack2.c_str(), "5256830060000023=05081019000990003210" );

	EXPECT_TRUE( m_pService->GetStatus() == GContactlessReaderService::eStatusDataArrived );

	m_pService->ProcessSystemStatusNotification(0x03, 0x00, 0x00);
	EXPECT_TRUE( m_pService->GetStatus() == GContactlessReaderService::eStatusWaitingRFCardData );
}

TEST_F(GContactlessReaderServiceUT, TestCardCycleWithStatistics)
{
	UINT8  aryu8CardReaderMsg[190];

	ASSERT_TRUE( m_pService != NULL);

	memset(aryu8CardReaderMsg, 0, 190);
	EXPECT_TRUE( m_pService->GetStatus() == GContactlessReaderService::eStatusInitial );

	m_pService->EnableDevice();

	EXPECT_TRUE( m_pService->GetStatus() == GContactlessReaderService::eStatusWaitingRFCardData );

	GConversion::ASCIIHexStringToBytes(aryu8CardReaderMsg,
		"00 70 84 00 50 04 00 2F 02 01 00 3C 42 35 32 35 36 38 33 30 30 36 30 30 30 30 30 32 33 5E 53 4D 49 54 48 2F 4A 4F 48 4E 5E 30 35 30 38 31 30 31 33 33 37 38 39 33 33 33 30 30 30 32 32 32 32 32 30 30 30 31 31 31 31 30 02 00 25 35 32 35 36 38 33 30 30 36 30 30 30 30 30 32 33 3D 30 35 30 38 31 30 31 39 30 30 30 39 39 30 30 30 33 32 31 30");

	GSPOTMessage oMsg(aryu8CardReaderMsg);
	m_pService->ProcessContactlessReaderReply(&oMsg);
	EXPECT_TRUE( m_pService->GetStatus() == GContactlessReaderService::eStatusDataArrived );

	EXPECT_TRUE( m_pService->GetSessionStatistics().GetTotalWaves() == 1 );
	EXPECT_TRUE( m_pService->GetSessionStatistics().GetStatistics(GContactlessReaderServiceStatistics::ePass) == 1 );

	m_pService->ProcessSystemStatusNotification(0x03, 0x00, 0x00);
	EXPECT_TRUE( m_pService->GetStatus() == GContactlessReaderService::eStatusWaitingRFCardData );

	m_pService->ProcessSystemStatusNotification(0x04, 0x00, 0x00);
	EXPECT_TRUE( m_pService->GetStatus() == GContactlessReaderService::eStatusWaitingRFCardData );

	GConversion::ASCIIHexStringToBytes(aryu8CardReaderMsg, "00 09 84 00 50 04 05 42 00");

	GSPOTMessage oMsg1(aryu8CardReaderMsg);
	m_pService->ProcessContactlessReaderReply(&oMsg1);
	EXPECT_TRUE( m_pService->GetStatus() == GContactlessReaderService::eStatusDataArrived );

	EXPECT_TRUE( m_pService->GetSessionStatistics().GetTotalWaves() == 2 );
	EXPECT_TRUE( m_pService->GetSessionStatistics().GetStatistics(GContactlessReaderServiceStatistics::ePass) == 1 );
	EXPECT_TRUE( m_pService->GetSessionStatistics().GetStatistics(GContactlessReaderServiceStatistics::eFail) == 1 );

	m_pService->ProcessSystemStatusNotification(0x03, 0x00, 0x00);
	EXPECT_TRUE( m_pService->GetStatus() == GContactlessReaderService::eStatusWaitingRFCardData );

	m_pService->ProcessSystemStatusNotification(0x04, 0x00, 0x00);
	EXPECT_TRUE( m_pService->GetStatus() == GContactlessReaderService::eStatusWaitingRFCardData );

	m_pService->DisableDevice();
	EXPECT_TRUE( m_pService->GetStatus() == GContactlessReaderService::eStatusInitial );

}
