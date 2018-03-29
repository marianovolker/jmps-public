/**
 * GGlobalSystemInformationUT.cpp
 *
 *  Created on: Feb 01 2013
 *      Author: gparis
 */

// GConversion
#include "Common/Util/GConversion.h"
#include "GGlobalSystemInformationUT.h"

GGlobalSystemInformation* GTestedSystemInformation::CreateGlobalSystemInformationToTest()
{
	return GGlobalSystemInformation::CreateInstance(NULL);
}

void GTestedSystemInformation::DestroyTestedGlobalSystemInformation()
{
	GGlobalSystemInformation::DestroyInstance();
}

GGlobalSystemInformationUT::GGlobalSystemInformationUT()
	: m_SysInfoToTest(NULL)
{
}

GGlobalSystemInformationUT::~GGlobalSystemInformationUT()
{
}

void GGlobalSystemInformationUT::SetUpTestCase()
{
}

void GGlobalSystemInformationUT::TearDownTestCase()
{
}

void GGlobalSystemInformationUT::SetUp()
{
	m_SysInfoToTest = GTestedSystemInformation::CreateGlobalSystemInformationToTest();
}

void GGlobalSystemInformationUT::TearDown()
{
	GTestedSystemInformation::DestroyTestedGlobalSystemInformation();
	m_SysInfoToTest = NULL;
}

TEST_F(GGlobalSystemInformationUT, TestHardwareInfoMessageReply)
{
	UINT8 aryu8MsgLayout[12];

	memset(aryu8MsgLayout, 0, sizeof(aryu8MsgLayout));
	GConversion::ASCIIHexStringToBytes(aryu8MsgLayout, "00 07 84 00 10 02 00");
	GSPOTMessage oBadMsg(aryu8MsgLayout);
	// Bad message application ID
	EXPECT_FALSE(GGlobalSystemInformation::isPinpadSysInfoMessageGroupReply(&oBadMsg));

	GConversion::ASCIIHexStringToBytes(aryu8MsgLayout, "00 07 81 00 20 02 00");
	GSPOTMessage oBadMsg1(aryu8MsgLayout);
	// Bad message command category ID
	EXPECT_FALSE(GGlobalSystemInformation::isPinpadSysInfoMessageGroupReply(&oBadMsg1));

	GConversion::ASCIIHexStringToBytes(aryu8MsgLayout, "00 08 81 00 10 01 00 00");
	GSPOTMessage oBadMsg2(aryu8MsgLayout);
	// Bad message command ID
	EXPECT_FALSE(GGlobalSystemInformation::isPinpadSysInfoMessageGroupReply(&oBadMsg2));

	GConversion::ASCIIHexStringToBytes(aryu8MsgLayout, "00 08 84 00 10 01 00 00");
	GSPOTMessage oBadMsg3(aryu8MsgLayout);
	// Bad message command ID
	EXPECT_FALSE(GGlobalSystemInformation::isOPTHardwareConfigMessageReply(&oBadMsg3));

	GConversion::ASCIIHexStringToBytes(aryu8MsgLayout, "00 08 81 00 10 03 00 00");
	GSPOTMessage oGoodMsg(aryu8MsgLayout);
	// Correct message application, category and command IDs
	EXPECT_TRUE(GGlobalSystemInformation::isPinpadSysInfoMessageGroupReply(&oGoodMsg));

	GConversion::ASCIIHexStringToBytes(aryu8MsgLayout, "00 08 84 00 10 02 00 00");
	GSPOTMessage oGoodMsg1(aryu8MsgLayout);
	// Correct message application, category and command IDs
	EXPECT_TRUE(GGlobalSystemInformation::isOPTHardwareConfigMessageReply(&oGoodMsg1));
}

TEST_F(GGlobalSystemInformationUT, TestSoftwareInfoMessageReply)
{
	UINT8 aryu8MsgLayout[12];

	memset(aryu8MsgLayout, 0, sizeof(aryu8MsgLayout));
	GConversion::ASCIIHexStringToBytes(aryu8MsgLayout, "00 07 84 00 10 03 00");
	GSPOTMessage oBadMsg(aryu8MsgLayout);
	// Bad message application ID
	EXPECT_FALSE(GGlobalSystemInformation::isPinpadSysInfoMessageGroupReply(&oBadMsg));

	GConversion::ASCIIHexStringToBytes(aryu8MsgLayout, "00 07 81 00 20 03 00");
	GSPOTMessage oBadMsg1(aryu8MsgLayout);
	// Bad message command category ID
	EXPECT_FALSE(GGlobalSystemInformation::isPinpadSysInfoMessageGroupReply(&oBadMsg1));

	GConversion::ASCIIHexStringToBytes(aryu8MsgLayout, "00 08 81 00 10 01 00 00");
	GSPOTMessage oBadMsg2(aryu8MsgLayout);
	// Bad message command ID
	EXPECT_FALSE(GGlobalSystemInformation::isPinpadSysInfoMessageGroupReply(&oBadMsg2));

	GConversion::ASCIIHexStringToBytes(aryu8MsgLayout, "00 08 81 00 10 02 00 00");
	GSPOTMessage oGoodMsg(aryu8MsgLayout);
	// Correct message application, category and command IDs
	EXPECT_TRUE(GGlobalSystemInformation::isPinpadSysInfoMessageGroupReply(&oGoodMsg));
}

TEST_F(GGlobalSystemInformationUT, TestBadParseHrwInfoReply)
{
	UINT8 aryu8MessageLayout[12];

	memset(aryu8MessageLayout, 0, sizeof(aryu8MessageLayout));
	GConversion::ASCIIHexStringToBytes(aryu8MessageLayout, "00 07 81 00 10 02 00");
	GSPOTMessage oBadMsg(aryu8MessageLayout);
	EXPECT_EQ(0, m_SysInfoToTest->ParseHrwInfoReply(&oBadMsg)); // reply message too short

	GConversion::ASCIIHexStringToBytes(aryu8MessageLayout, "00 08 81 00 10 02 01 01");
	GSPOTMessage oBadMsg1(aryu8MessageLayout);
	EXPECT_EQ(0, m_SysInfoToTest->ParseHrwInfoReply(&oBadMsg1)); // ACK error

	GConversion::ASCIIHexStringToBytes(aryu8MessageLayout, "00 08 81 00 10 02 00 00");
	GSPOTMessage oBadMsg2(aryu8MessageLayout);
	EXPECT_EQ(0, m_SysInfoToTest->ParseHrwInfoReply(&oBadMsg2)); // no Hw info items
}

TEST_F(GGlobalSystemInformationUT, TestBadParseSwfInfoReply)
{
	UINT8 aryu8MessageLayout[12];

	memset(aryu8MessageLayout, 0, sizeof(aryu8MessageLayout));
	GConversion::ASCIIHexStringToBytes(aryu8MessageLayout, "00 07 81 00 10 03 00");
	GSPOTMessage oBadMsg(aryu8MessageLayout);
	EXPECT_EQ(0, m_SysInfoToTest->ParseHrwInfoReply(&oBadMsg)); // reply message too short

	GConversion::ASCIIHexStringToBytes(aryu8MessageLayout, "00 08 81 00 10 03 01 01");
	GSPOTMessage oBadMsg1(aryu8MessageLayout);
	EXPECT_EQ(0, m_SysInfoToTest->ParseHrwInfoReply(&oBadMsg1)); // ACK error

	GConversion::ASCIIHexStringToBytes(aryu8MessageLayout, "00 08 81 00 10 03 00 00");
	GSPOTMessage oBadMsg2(aryu8MessageLayout);
	EXPECT_EQ(0, m_SysInfoToTest->ParseHrwInfoReply(&oBadMsg2)); // no Sw info items
}


TEST_F(GGlobalSystemInformationUT, TestGoodParseHrwInfoReply_M3)
{
	UINT8 aryu8MsgLayout[160];

	memset(aryu8MsgLayout, 0, sizeof(aryu8MsgLayout));
	GConversion::ASCIIHexStringToBytes(aryu8MsgLayout,
	            "009C81001002000F01103030303230363131303136323132343910103236"
	            "3834313237323030303030303534110241334001A4430500F00140104210"
	            "4D43463533327800000000000000000041040F0F00002201012010323644"
	            "433230363230303030303044372102413332018233040703000034105A55"
	            "2D3138373354543234330000000030103133304135303236373330343330"
	            "313731023030");

	GSPOTMessage oGoodMsg(aryu8MsgLayout);
	EXPECT_EQ(15, m_SysInfoToTest->ParseHrwInfoReply(&oGoodMsg)); // 15 Hw info items

	SPOT_HW_INFO& rSPOTInfo = m_SysInfoToTest->GetSPOTInfo();
	EXPECT_EQ(0, strcmp("0002061101621249", rSPOTInfo.m_pcSerialNumber));

	SM_HW_INFO& rSMInfo = m_SysInfoToTest->GetSecModuleInfo();
	EXPECT_EQ(0, strcmp("2684127200000054", rSMInfo.m_pcSerialNumber));
	EXPECT_EQ(0x41, rSMInfo.m_aryu8HwrID[0]);
	EXPECT_EQ(0x33, rSMInfo.m_aryu8HwrID[1]);

	DY_HW_INFO& rDYInfo = m_SysInfoToTest->GetDisplayInfo();
	EXPECT_EQ(0xA4, rDYInfo.m_u8DevType);
	EXPECT_EQ(0x0F, rDYInfo.m_aryu8DevFunctions[0]);
	EXPECT_EQ(0x0F, rDYInfo.m_aryu8DevFunctions[1]);
	EXPECT_EQ(0x00, rDYInfo.m_aryu8DevFunctions[2]);
	EXPECT_EQ(0x00, rDYInfo.m_aryu8DevFunctions[3]);
	EXPECT_EQ(0, strcmp("MCF532x", rDYInfo.m_pcDevModel));
	EXPECT_EQ(320, rDYInfo.m_DevHwrConfig.m_u16Xres);
	EXPECT_EQ(240, rDYInfo.m_DevHwrConfig.m_u16Yres);
	EXPECT_EQ(16, rDYInfo.m_DevHwrConfig.m_u8ColorDepth);

	KB_HW_INFO& rKBInfo = m_SysInfoToTest->GetKeypadInfo();
	EXPECT_EQ(0, strcmp("26DC2062000000D7", rKBInfo.m_pcSerialNumber));
	EXPECT_EQ(0x41, rKBInfo.m_aryu8HwrID[0]);
	EXPECT_EQ(0x33, rKBInfo.m_aryu8HwrID[1]);
	EXPECT_EQ(1, rKBInfo.m_u8DevType);

	CR_HW_INFO& rCRInfo = m_SysInfoToTest->GetCardReaderInfo();
	EXPECT_EQ(0, strcmp("130A502673043017", rCRInfo.m_pcSerialNumber));
	EXPECT_EQ(0x30, rCRInfo.m_aryu8HwrID[0]);
	EXPECT_EQ(0x30, rCRInfo.m_aryu8HwrID[1]);
	EXPECT_EQ(0x82, rCRInfo.m_u8DevType);
	EXPECT_EQ(0x07, rCRInfo.m_aryu8DevFunctions[0]);
	EXPECT_EQ(0x03, rCRInfo.m_aryu8DevFunctions[1]);
	EXPECT_EQ(0x00, rCRInfo.m_aryu8DevFunctions[2]);
	EXPECT_EQ(0x00, rCRInfo.m_aryu8DevFunctions[3]);
	EXPECT_EQ(0, strcmp("ZU-1873TT243", rCRInfo.m_pcDevModel));
}

TEST_F(GGlobalSystemInformationUT, TestGoodParseHrwInfoReply_M5)
{
	UINT8 aryu8MsgLayout[160];

	memset(aryu8MsgLayout, 0, sizeof(aryu8MsgLayout));
	GConversion::ASCIIHexStringToBytes(aryu8MsgLayout,
	            "009C81001002000F01103030303230363131313537333132303210103236"
	            "4138413335323030303030304642110241334001A4430501E00280104210"
	            "4F4D415033357878000000000000000041040F0F00002201012010393233"
	            "373031313700411308547821252102303032018233040703000034105A55"
	            "2D3138373354543234330000000030103131303835323435383830373239"
	            "313131023030");

	GSPOTMessage oGoodMsg(aryu8MsgLayout);
	EXPECT_EQ(15, m_SysInfoToTest->ParseHrwInfoReply(&oGoodMsg)); // 15 Hw info items

	SPOT_HW_INFO& rSPOTInfo = m_SysInfoToTest->GetSPOTInfo();
	EXPECT_EQ(0, strcmp("0002061115731202", rSPOTInfo.m_pcSerialNumber));

	SM_HW_INFO& rSMInfo = m_SysInfoToTest->GetSecModuleInfo();
	EXPECT_EQ(0, strcmp("26A8A352000000FB", rSMInfo.m_pcSerialNumber));
	EXPECT_EQ(0x41, rSMInfo.m_aryu8HwrID[0]);
	EXPECT_EQ(0x33, rSMInfo.m_aryu8HwrID[1]);

	DY_HW_INFO& rDYInfo = m_SysInfoToTest->GetDisplayInfo();
	EXPECT_EQ(0xA4, rDYInfo.m_u8DevType);
	EXPECT_EQ(0x0F, rDYInfo.m_aryu8DevFunctions[0]);
	EXPECT_EQ(0x0F, rDYInfo.m_aryu8DevFunctions[1]);
	EXPECT_EQ(0x00, rDYInfo.m_aryu8DevFunctions[2]);
	EXPECT_EQ(0x00, rDYInfo.m_aryu8DevFunctions[3]);
	EXPECT_EQ(0, strcmp("OMAP35xx", rDYInfo.m_pcDevModel));
	EXPECT_EQ(640, rDYInfo.m_DevHwrConfig.m_u16Xres);
	EXPECT_EQ(480, rDYInfo.m_DevHwrConfig.m_u16Yres);
	EXPECT_EQ(16, rDYInfo.m_DevHwrConfig.m_u8ColorDepth);

	KB_HW_INFO& rKBInfo = m_SysInfoToTest->GetKeypadInfo();
	EXPECT_EQ(0, strcmp("92370117", rKBInfo.m_pcSerialNumber));
	EXPECT_EQ(0x30, rKBInfo.m_aryu8HwrID[0]);
	EXPECT_EQ(0x30, rKBInfo.m_aryu8HwrID[1]);
	EXPECT_EQ(1, rKBInfo.m_u8DevType);

	CR_HW_INFO& rCRInfo = m_SysInfoToTest->GetCardReaderInfo();
	EXPECT_EQ(0, strcmp("1108524588072911", rCRInfo.m_pcSerialNumber));
	EXPECT_EQ(0x30, rCRInfo.m_aryu8HwrID[0]);
	EXPECT_EQ(0x30, rCRInfo.m_aryu8HwrID[1]);
	EXPECT_EQ(0x82, rCRInfo.m_u8DevType);
	EXPECT_EQ(0x07, rCRInfo.m_aryu8DevFunctions[0]);
	EXPECT_EQ(0x03, rCRInfo.m_aryu8DevFunctions[1]);
	EXPECT_EQ(0x00, rCRInfo.m_aryu8DevFunctions[2]);
	EXPECT_EQ(0x00, rCRInfo.m_aryu8DevFunctions[3]);
	EXPECT_EQ(0, strcmp("ZU-1873TT243", rCRInfo.m_pcDevModel));
}

TEST_F(GGlobalSystemInformationUT, TestGoodParseSftInfoReply_M5)
{
	UINT8 aryu8MsgLayout[512];

	memset(aryu8MsgLayout, 0, sizeof(aryu8MsgLayout));
	GConversion::ASCIIHexStringToBytes(aryu8MsgLayout,
			"01 B0 81 00 10 03 00 10 02 20 4F 53 20 30 32 2E 30 32 00 00"
			"00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00"
			"00 00 03 20 50 49 4E 50 41 44 20 30 32 2E 30 34 00 00 00 00"
			"00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 04 20 00 00"
			"00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00"
			"00 00 00 00 00 00 00 00 00 00 05 20 4F 50 54 20 30 32 2E 30"
			"32 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00"
			"00 00 00 00 06 20 53 45 54 55 50 20 30 32 2E 30 32 00 00 00"
			"00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 07 20"
			"00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00"
			"00 00 00 00 00 00 00 00 00 00 00 00 08 20 00 00 00 00 00 00"
			"00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00"
			"00 00 00 00 00 00 09 20 43 52 49 4E 44 42 49 4F 53 20 39 39"
			"2E 39 39 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00"
			"0A 20 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00"
			"00 00 00 00 00 00 00 00 00 00 00 00 00 00 0D 10 64 64 30 31"
			"39 35 00 00 00 00 00 00 00 00 00 00 0C 10 53 47 30 31 30 33"
			"00 00 00 00 00 00 00 00 00 00 0B 10 52 44 30 31 32 2D 31 30"
			"30 32 30 34 5F 31 0D 0A 0E 10 00 00 00 00 00 00 00 00 00 00"
			"00 00 00 00 00 00 10 16 00 00 00 00 00 00 00 00 00 00 00 00"
			"00 00 00 00 00 00 00 00 00 00 11 11 41 6D 65 72 69 63 61 2F"
			"4E 65 77 5F 59 6F 72 6B 0A 12 01 00");

	GSPOTMessage oGoodMsg(aryu8MsgLayout);
	EXPECT_EQ(12, m_SysInfoToTest->ParseSwfInfoReply(&oGoodMsg)); // 12 Sw info items

	EXPECT_STREQ( "OS 02.02",        m_SysInfoToTest->GetSoftwareInfo(eOSVersion) );
	EXPECT_STREQ( "PINPAD 02.04",    m_SysInfoToTest->GetSoftwareInfo(eROMFs1Version) );
	EXPECT_STREQ( "OPT 02.02",       m_SysInfoToTest->GetSoftwareInfo(eROMFs3Version) );
	EXPECT_STREQ( "SETUP 02.02",     m_SysInfoToTest->GetSoftwareInfo(eROMFs4Version) );
	EXPECT_STREQ( "CRINDBIOS 99.99", m_SysInfoToTest->GetSoftwareInfo(eROMFs7Version) );
	EXPECT_STREQ( "", m_SysInfoToTest->GetSoftwareInfo(eROMFs2Version) );
	EXPECT_STREQ( "", m_SysInfoToTest->GetSoftwareInfo(eROMFs5Version) );
	EXPECT_STREQ( "", m_SysInfoToTest->GetSoftwareInfo(eROMFs6Version) );
	EXPECT_STREQ( "", m_SysInfoToTest->GetSoftwareInfo(eROMFs8Version) );

	EXPECT_STREQ( "RD012-100204_1\r", m_SysInfoToTest->GetFirmwareInfo(eCRFwrVersion) );
	EXPECT_STREQ( "SG0103", m_SysInfoToTest->GetFirmwareInfo(eKPFwrVersion) );
	EXPECT_STREQ( "dd0195", m_SysInfoToTest->GetFirmwareInfo(eSecFwrVersion) );
}
