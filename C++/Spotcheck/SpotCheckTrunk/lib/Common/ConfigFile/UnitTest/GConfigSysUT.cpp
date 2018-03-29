#include <GSystem.h>
#include <GFile.h>
#include <Common/ConfigFile/GConfigSys.h>
#include "GConfigSysUT.h"
#include <list>
#include <string>

using namespace GConfigTypes;


void GConfigSysUT::SetUp()
{
}


void GConfigSysUT::TearDown()
{
}


TEST_F(GConfigSysUT, TestDefaultWorkingDirs)
{
	GConfigSys::RemoveSoleInstance();

	GConfigSys *pConfigSys = GConfigSys::GetSoleInstance();
	EXPECT_NE(pConfigSys, (GConfigSys *)NULL);

	EXPECT_STREQ(GConfigSys::CDefaultWorkingDir.c_str(), pConfigSys->GetOptDir().c_str());
	EXPECT_STREQ(GConfigSys::CDefaultWorkingDir.c_str(), pConfigSys->GetEtcDir().c_str());
	EXPECT_STREQ(GConfigSys::CDefaultWorkingDir.c_str(), pConfigSys->GetVarDir().c_str());

	GConfigSys::RemoveSoleInstance();
}


TEST_F(GConfigSysUT, TestSpecifiedWorkingDirs)
{
	GConfigSys::RemoveSoleInstance();

	GConfigSys *pConfigSys = GConfigSys::GetSoleInstance("TestFiles/Good/RunConfig.xml");
	EXPECT_NE(pConfigSys, (GConfigSys *)NULL);

	EXPECT_STREQ("./TestFiles/Good/opt/", pConfigSys->GetOptDir().c_str());
	EXPECT_STREQ("./TestFiles/Good/etc/", pConfigSys->GetEtcDir().c_str());
	EXPECT_STREQ("./TestFiles/Good/var/", pConfigSys->GetVarDir().c_str());

	GConfigSys::RemoveSoleInstance();
}


TEST_F(GConfigSysUT, GetValue)
{
	GConfigSys::RemoveSoleInstance();

	GConfigSys *pConfigSys = GConfigSys::GetSoleInstance("TestFiles/Good/RunConfig.xml");
	EXPECT_NE(pConfigSys, (GConfigSys *)NULL);

	double dActualValue;
	double dExpectedValue(3.14159);
	EXPECT_EQ(pConfigSys->GetValue("/File1/float",  dActualValue), eConfigOk);
	EXPECT_DOUBLE_EQ(dExpectedValue,dActualValue);
	EXPECT_EQ(pConfigSys->GetValue("/File1/float1",  dActualValue), eConfigOk);
	EXPECT_DOUBLE_EQ(dExpectedValue, dActualValue);

	INT32 s32ActualValue;
	INT32 s32ExpectedValue(-1073741824);
	EXPECT_EQ(pConfigSys->GetValue("/File1/int",  s32ActualValue), eConfigOk);
	EXPECT_EQ(s32ExpectedValue, s32ActualValue);
	EXPECT_EQ(pConfigSys->GetValue("/File1/int1",  s32ActualValue), eConfigOk);
	EXPECT_EQ(s32ExpectedValue, s32ActualValue);

	UINT32 u32ActualValue;
	UINT32 u32ExpectedValue(4294901760UL);
	EXPECT_EQ(pConfigSys->GetValue("/File1/uint", u32ActualValue), eConfigOk);
	EXPECT_EQ(u32ExpectedValue, u32ActualValue);
	EXPECT_EQ(pConfigSys->GetValue("/File1/uint1", u32ActualValue), eConfigOk);
	EXPECT_EQ(u32ExpectedValue, u32ActualValue);

	bool bActualValue;
	EXPECT_EQ(pConfigSys->GetValue("/File1/bool", bActualValue), eConfigOk);
	EXPECT_TRUE(bActualValue);
	EXPECT_EQ(pConfigSys->GetValue("/File1/bool1", bActualValue), eConfigOk);
	EXPECT_TRUE(bActualValue);
	EXPECT_EQ(pConfigSys->GetValue("/File1/bool2", bActualValue), eConfigOk);
	EXPECT_TRUE(bActualValue);
	EXPECT_EQ(pConfigSys->GetValue("/File1/bool3", bActualValue), eConfigOk);
	EXPECT_TRUE(bActualValue);
	EXPECT_EQ(pConfigSys->GetValue("/File1/bool4", bActualValue), eConfigOk);
	EXPECT_TRUE(bActualValue);
	EXPECT_EQ(pConfigSys->GetValue("/File1/bool5", bActualValue), eConfigOk);
	EXPECT_FALSE(bActualValue);
	EXPECT_EQ(pConfigSys->GetValue("/File1/bool6", bActualValue), eConfigOk);
	EXPECT_FALSE(bActualValue);
	EXPECT_EQ(pConfigSys->GetValue("/File1/bool7", bActualValue), eConfigOk);
	EXPECT_FALSE(bActualValue);
	EXPECT_EQ(pConfigSys->GetValue("/File1/bool8", bActualValue), eConfigOk);
	EXPECT_FALSE(bActualValue);

	GConfigSys::RemoveSoleInstance();
}


TEST_F(GConfigSysUT, SetValue)
{
	GConfigSys::RemoveSoleInstance();

	GSystem::CopyFile("TestFiles/Good/etc/config/File2.xml", "TestFiles/Good/var/config/File2.xml");

	GConfigSys *pConfigSys = GConfigSys::GetSoleInstance("TestFiles/Good/RunConfig.xml");
	EXPECT_NE(pConfigSys, (GConfigSys *)NULL);

	// Test SetValue(bool)
	bool bActualValue;
	bool bOldValue(true);
	EXPECT_EQ(pConfigSys->GetValue("/File2/bool", bActualValue), eConfigOk);
	EXPECT_EQ(bOldValue, bActualValue);
	bool bNewValue(false);
	EXPECT_EQ(pConfigSys->SetValue("/File2/bool", bNewValue), eConfigOk);
	EXPECT_EQ(pConfigSys->GetValue("/File2/bool", bActualValue), eConfigOk);
	EXPECT_EQ(bNewValue, bActualValue);

	// Test SetValue(int)
	INT32 s32ActualValue;
	INT32 s32OldValue(-12345);
	EXPECT_EQ(pConfigSys->GetValue("/File2/int", s32ActualValue), eConfigOk);
	EXPECT_EQ(s32OldValue, s32ActualValue);
	INT32 s32NewValue(-67890);
	EXPECT_EQ(pConfigSys->SetValue("/File2/int", s32NewValue), eConfigOk);
	EXPECT_EQ(pConfigSys->GetValue("/File2/int", s32ActualValue), eConfigOk);
	EXPECT_EQ(s32NewValue, s32ActualValue);

	// Test SetValue(uint)
	UINT32 u32ActualValue;
	UINT32 u32OldValue(12345);
	EXPECT_EQ(pConfigSys->GetValue("/File2/uint", u32ActualValue), eConfigOk);
	EXPECT_EQ(u32OldValue, u32ActualValue);
	UINT32 u32NewValue(67890);
	EXPECT_EQ(pConfigSys->SetValue("/File2/uint", u32NewValue), eConfigOk);
	EXPECT_EQ(pConfigSys->GetValue("/File2/uint", u32ActualValue), eConfigOk);
	EXPECT_EQ(u32NewValue, u32ActualValue);

	// Test SetValue(float)
	double dActualValue;
	double dOldValue(1.2345);
	EXPECT_EQ(pConfigSys->GetValue("/File2/float", dActualValue), eConfigOk);
	EXPECT_DOUBLE_EQ(dOldValue, dActualValue);
	double dNewValue(6.7890);
	EXPECT_EQ(pConfigSys->SetValue("/File2/float", dNewValue), eConfigOk);
	EXPECT_EQ(pConfigSys->GetValue("/File2/float", dActualValue), eConfigOk);
	EXPECT_DOUBLE_EQ(dNewValue, dActualValue);

	// Test SetValue(string)
	std::string ActualValue;
	std::string OldValue("Alpha");
	EXPECT_EQ(pConfigSys->GetValue("/File2/string", ActualValue), eConfigOk);
	EXPECT_STREQ(OldValue.c_str(), ActualValue.c_str());
	std::string NewValue("Omega");
	EXPECT_EQ(pConfigSys->SetValue("/File2/string", NewValue), eConfigOk);
	EXPECT_EQ(pConfigSys->GetValue("/File2/string", ActualValue), eConfigOk);
	EXPECT_STREQ(NewValue.c_str(), ActualValue.c_str());

	// Test SetValue(string) with invalid tag.
	EXPECT_EQ(pConfigSys->SetValue("/File2/invalid", NewValue), eConfigCanNotChangeNonDefault);

	GConfigSys::RemoveSoleInstance();
}

