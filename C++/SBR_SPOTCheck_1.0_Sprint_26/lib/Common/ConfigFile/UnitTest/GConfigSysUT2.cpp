#include <Common/ConfigFile/GConfigSys.h>
#include <GSystem.h>
#include <GFile.h>
#include "GConfigSysUT2.h"
#include <list>
#include <string>

using namespace GConfigTypes;


TEST(GConfigSysUT2, CorruptFileMain)
{
	string overridesBackupFilename = "TestFiles/CorruptMain/var/config/overrides.bak";
	string overridesXml = "TestFiles/CorruptMain/var/config/overrides.xml";
	EXPECT_TRUE(GSystem::CopyFile("TestFiles/CorruptMain/var/config/overrides.backup.test",overridesBackupFilename));

	EXPECT_TRUE(GSystem::CopyFile("TestFiles/CorruptMain/var/config/overrides.main.test",overridesXml));

	GConfigSys* pConfigSys = GConfigSys::GetSoleInstance("TestFiles/CorruptMain/RunConfig.xml");

	UINT32 u32ActualValue;
	UINT32 u32ExpectedValue(40);
	EXPECT_EQ(pConfigSys->GetValue("/config/aaa", u32ActualValue), eConfigOk);
	EXPECT_EQ(u32ActualValue, u32ExpectedValue);

	GConfigSys::RemoveSoleInstance();

	// Clean up after ourselves, delete the written file
	EXPECT_TRUE(GSystem::RemoveFile(overridesBackupFilename.c_str()));
	EXPECT_TRUE(GSystem::RemoveFile(overridesXml.c_str()));
}


TEST(GConfigSysUT2, CorruptFileBoth)
{
	string overridesBackupFilename = "TestFiles/CorruptBoth/var/config/overrides.bak";
	string overridesXml = "TestFiles/CorruptBoth/var/config/overrides.xml";

	EXPECT_TRUE(GSystem::CopyFile("TestFiles/CorruptBoth/var/config/overrides.backup.test",overridesBackupFilename));

	EXPECT_TRUE(GSystem::CopyFile("TestFiles/CorruptBoth/var/config/overrides.main.test",overridesXml));

	GConfigSys* pConfigSys = GConfigSys::GetSoleInstance("TestFiles/CorruptBoth/RunConfig.xml");

	UINT32 u32ActualValue;
	UINT32 u32ExpectedValue(10);
	EXPECT_EQ(pConfigSys->GetValue("/config/aaa", u32ActualValue), eConfigOk);
	EXPECT_EQ(u32ExpectedValue, u32ActualValue);

	GConfigSys::RemoveSoleInstance();

	// Clean up after ourselves, delete the written file
	EXPECT_TRUE(GSystem::RemoveFile(overridesBackupFilename.c_str()));
}

TEST(GConfigSysUT2, DiskWriteError)
{
	GConfigSys* pConfigSys = GConfigSys::GetSoleInstance("TestFiles/DiskError/RunConfig.xml");
// TODO: Still need to figure out which is the intention of this UT
//
	UINT32 u32ActualValue;
	UINT32 u32ExpectedValue(15);
	EXPECT_EQ(pConfigSys->SetValue("/config/aaa", u32ExpectedValue), eConfigFileWriteError);
	EXPECT_EQ(pConfigSys->GetValue("/config/aaa", u32ActualValue), eConfigOk);
	EXPECT_EQ(u32ActualValue, u32ExpectedValue);

	GConfigSys::RemoveSoleInstance();


	pConfigSys = GConfigSys::GetSoleInstance("TestFiles/DiskError/RunConfig.xml");

	EXPECT_EQ(pConfigSys->GetValue("/config/aaa", u32ActualValue), eConfigOk);
	EXPECT_NE(u32ActualValue, u32ExpectedValue);

	GConfigSys::RemoveSoleInstance();
}
