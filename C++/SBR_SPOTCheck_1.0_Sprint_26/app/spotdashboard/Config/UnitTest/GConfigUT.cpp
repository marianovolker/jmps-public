#include "Config/SPOTDashboardIni.h"

#include "SPOT/SPOTProtocolInterface/GAckCodesPinpadApplications.h"
#include "SPOT/SPOTProtocolInterface/GSpotSysMainPinpadApplications.h"
#include "SPOT/SPOTProtocolInterface/GSpotSysSecurityModulePinpadApplications.h"
#include "SPOT/SPOTProtocolInterface/GSpotSysCardReaderPinpadApplications.h"
#include "SPOT/SPOTProtocolInterface/GSpotSysKeyboardPinpadApplications.h"

#include "GConfigUT.h"



using namespace std;

GConfigUT::GConfigUT()
{
}

GConfigUT::~GConfigUT()
{
}

void GConfigUT::SetUp()
{
}

void GConfigUT::TearDown()
{
}

TEST_F(GConfigUT, TestConfigIni)
{
	SPOTDashboardIni::CreateInstance();
	SPOTDashboardIni::GetSoleInstance()->SetINIFilename("ini/SPOTDashboard_UT.ini");
	ASSERT_TRUE( SPOTDashboardIni::GetSoleInstance()->Initialize() );

	ASSERT_TRUE( (strcmp(GAckCodesPinpadApplications::GetOrCreateInstance()->GetDescription(0x00),"This is a test 01")==0) );

	ASSERT_TRUE( (strcmp(GSpotSysMainPinpadApplications::GetOrCreateInstance()->GetStatesDescription(0x01),"This is a test 02")==0) );
	ASSERT_TRUE( (strcmp(GSpotSysMainPinpadApplications::GetOrCreateInstance()->GetStateExtensionsDescription(0x00),"This is a test 03")==0) );
	ASSERT_TRUE( (strcmp(GSpotSysMainPinpadApplications::GetOrCreateInstance()->GetErrorsDescription(0x00),"This is a test 04")==0) );

	ASSERT_TRUE( (strcmp(GSpotSysSecurityModulePinpadApplications::GetOrCreateInstance()->GetStatesDescription(0x00),"This is a test 05")==0) );
	ASSERT_TRUE( (strcmp(GSpotSysSecurityModulePinpadApplications::GetOrCreateInstance()->GetStateExtensionsDescription(0x80),"This is a test 06")==0) );
	ASSERT_TRUE( (strcmp(GSpotSysSecurityModulePinpadApplications::GetOrCreateInstance()->GetErrorsDescription(0x01),"This is a test 07")==0) );

	ASSERT_TRUE( (strcmp(GSpotSysKeyboardPinpadApplications::GetOrCreateInstance()->GetStatesDescription(0x00),"This is a test 08")==0) );
	ASSERT_TRUE( (strcmp(GSpotSysKeyboardPinpadApplications::GetOrCreateInstance()->GetStateExtensionsDescription(0x80),"This is a test 09")==0) );
	ASSERT_TRUE( (strcmp(GSpotSysKeyboardPinpadApplications::GetOrCreateInstance()->GetErrorsDescription(0x01),"This is a test 10")==0) );

	ASSERT_TRUE( (strcmp(GSpotSysCardReaderPinpadApplications::GetOrCreateInstance()->GetStatesDescription(0x00),"This is a test 11")==0) );
	ASSERT_TRUE( (strcmp(GSpotSysCardReaderPinpadApplications::GetOrCreateInstance()->GetStateExtensionsDescription(0x01),"This is a test 12")==0) );
	ASSERT_TRUE( (strcmp(GSpotSysCardReaderPinpadApplications::GetOrCreateInstance()->GetErrorsDescription(0x01),"This is a test 13")==0) );

	SPOTDashboardIni::Destroy();
}
