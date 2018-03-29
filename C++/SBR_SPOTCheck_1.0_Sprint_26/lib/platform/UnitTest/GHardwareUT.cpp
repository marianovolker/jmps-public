// GHardwareUT.cpp

#include "GHardwareUT.h"
#include "GHardware.h"


GHardwareUT::GHardwareUT()
{
}

GHardwareUT::~GHardwareUT()
{
}

void GHardwareUT::SetUp()
{
}

void GHardwareUT::TearDown()
{
}

// Test Case 128.1 - The system identifies the correct side id based on the configured value.

TEST_F(GHardwareUT, TC_128_1)
{
	bool bRet(false);
	GHardwareTypes::eSideId Id(GHardwareTypes::eSideUnknown);

	bRet = GHardware::SetSideId(GHardwareTypes::eSideMaster);
	EXPECT_EQ(bRet,true);
	Id = GHardware::GetSideId();
	EXPECT_EQ(Id,GHardwareTypes::eSideMaster);

	bRet = GHardware::SetSideId(GHardwareTypes::eSideSlave);
	EXPECT_EQ(bRet,true);
	Id = GHardware::GetSideId();
	EXPECT_EQ(Id,GHardwareTypes::eSideSlave);

	bRet = GHardware::SetSideId(GHardwareTypes::eSideUnknown);
	EXPECT_EQ(bRet,true);
	Id = GHardware::GetSideId();
	EXPECT_EQ(Id,GHardwareTypes::eSideUnknown);
}

// Test Case 128.2 - The system identifies the correct door state based on the configured value.

TEST_F(GHardwareUT, TC_128_2)
{
	bool bRet(false);
	GHardwareTypes::eSwitchState State(GHardwareTypes::eSwitchStateUnknown);

	bRet = GHardware::SetDoorSwitchState(GHardwareTypes::eSwitchOpen);
	EXPECT_EQ(bRet,true);
	State = GHardware::GetDoorSwitchState();
	EXPECT_EQ(State, GHardwareTypes::eSwitchOpen);

	bRet = GHardware::SetDoorSwitchState(GHardwareTypes::eSwitchClosed);
	EXPECT_EQ(bRet,true);
	State = GHardware::GetDoorSwitchState();
	EXPECT_EQ(State, GHardwareTypes::eSwitchClosed);

	bRet = GHardware::SetDoorSwitchState(GHardwareTypes::eSwitchStateUnknown);
	EXPECT_EQ(bRet,true);
	State = GHardware::GetDoorSwitchState();
	EXPECT_EQ(State, GHardwareTypes::eSwitchStateUnknown);
}

