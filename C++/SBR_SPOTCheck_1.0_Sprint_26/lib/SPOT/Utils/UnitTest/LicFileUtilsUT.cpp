/*
 * © 2013 Gilbarco Inc.
 * Confidential and Proprietary
 *
 * GLicFileUtilsUT.h
 * Created on: July 30 2013
 * Author: pcrespo
 */

#include "LicFileUtilsUT.h"
#include <SPOT/Utils/GFileDownloadRequestHelperM3.h>
 #include <SPOT/Utils/GFileDownloadRequestHelperM5.h>
#include <unistd.h>
#include <string>
#include <ostream>
#include <sys/stat.h>

using namespace std;

GLicFileUtilsUT::GLicFileUtilsUT()
{
}

GLicFileUtilsUT::~GLicFileUtilsUT()
{
}

void GLicFileUtilsUT::SetUpTestCase()
{
}

void GLicFileUtilsUT::TearDownTestCase()
{
}

void GLicFileUtilsUT::SetUp()
{
}

void GLicFileUtilsUT::TearDown()
{
}

//there are not lic files present
TEST_F(GLicFileUtilsUT, TestDownloadRequestHelperDevelopmentM3)
{
	string strResourceFile("TestFiles/ResourceOnly/resource_example_m3.bin");
	string strPPN("00022001");

	//get the file size
	struct stat st;
	stat(strResourceFile.c_str(), &st);
	UINT32 u32Size = st.st_size;

	GFileDownloadRequestHelperM3 oDownloadRequestHelper;

	EXPECT_TRUE(oDownloadRequestHelper.ProcessResourceFile(strResourceFile.c_str(), strPPN.c_str()));

	EXPECT_EQ(oDownloadRequestHelper.GetFileId(), 31);

	EXPECT_EQ(oDownloadRequestHelper.GetFileSize(), (u32Size - oDownloadRequestHelper.GetResmarkSize()));
	
	//test files has been hacked with this value
	UINT32 u32TestCRC = 0x01010101;
	EXPECT_EQ(oDownloadRequestHelper.GetCRC(), u32TestCRC);
	
	EXPECT_EQ(oDownloadRequestHelper.GetAuthenType(), 2);
	
	//.lic file is not present then the auth is gotten from resource itself
	UINT32 u32TestAuth = 0x02020202;
	EXPECT_EQ(oDownloadRequestHelper.GetAuthentication(), u32TestAuth);

	EXPECT_STREQ(oDownloadRequestHelper.GetDescription(), "");
}

//lic file preset
TEST_F(GLicFileUtilsUT, TestDownloadRequestHelperProductionLicM3)
{
	string strResourceFile("TestFiles/LicFilePresent/resource_example_m3.bin");

	//this is base ppn + 1 for .lic file
	string strPPN("00022001");

	//get the file size
	struct stat st;
	stat(strResourceFile.c_str(), &st);
	UINT32 u32Size = st.st_size;

	GFileDownloadRequestHelperM3 oDownloadRequestHelper;

	EXPECT_TRUE(oDownloadRequestHelper.ProcessResourceFile(strResourceFile.c_str(), strPPN.c_str()));

	EXPECT_EQ(oDownloadRequestHelper.GetFileId(), 31);

	EXPECT_EQ(oDownloadRequestHelper.GetFileSize(), (u32Size - oDownloadRequestHelper.GetResmarkSize()));

	//test files has been hacked with this value
	UINT32 u32TestCRC = 0x01010101;
	EXPECT_EQ(oDownloadRequestHelper.GetCRC(), u32TestCRC);

	EXPECT_EQ(oDownloadRequestHelper.GetAuthenType(), 2);

	//.lic file is present and it has been hacked with this value on the record for ppn 00022001
	UINT32 u32TestAuth = 0x03030303;
	EXPECT_EQ(oDownloadRequestHelper.GetAuthentication(), u32TestAuth);

	EXPECT_STREQ(oDownloadRequestHelper.GetDescription(), "");
}

//lic file preset
TEST_F(GLicFileUtilsUT, TestDownloadRequestHelperProductionLic0M3)
{
	string strResourceFile("TestFiles/LicFilePresent/resource_example_m3.bin");

	//this is base ppn + 2 for .lic0 file
	string strPPN("01601538");

	//get the file size
	struct stat st;
	stat(strResourceFile.c_str(), &st);
	UINT32 u32Size = st.st_size;

	GFileDownloadRequestHelperM3 oDownloadRequestHelper;

	EXPECT_TRUE(oDownloadRequestHelper.ProcessResourceFile(strResourceFile.c_str(), strPPN.c_str()));

	EXPECT_EQ(oDownloadRequestHelper.GetFileId(), 31);

	EXPECT_EQ(oDownloadRequestHelper.GetFileSize(), (u32Size - oDownloadRequestHelper.GetResmarkSize()));

	//test files has been hacked with this value
	UINT32 u32TestCRC = 0x01010101;
	EXPECT_EQ(oDownloadRequestHelper.GetCRC(), u32TestCRC);

	EXPECT_EQ(oDownloadRequestHelper.GetAuthenType(), 2);

	//.lic0 file is present and it has been hacked with this value on the record for ppn 01601538
	UINT32 u32TestAuth = 0x04040404;
	EXPECT_EQ(oDownloadRequestHelper.GetAuthentication(), u32TestAuth);

	EXPECT_STREQ(oDownloadRequestHelper.GetDescription(), "");
}

TEST_F(GLicFileUtilsUT, TestDownloadRequestHelperDevelopmentM5)
{
	string strResourceFile("TestFiles/ResourceOnly/resource_example_0_m5.bin");

	//get the file size
	struct stat st;
	stat(strResourceFile.c_str(), &st);
	UINT32 u32Size = st.st_size;

	UINT8 aryDummyAuthentication[GFileDownloadRequestHelperM5::CAuthLen];
	memset(aryDummyAuthentication, '1', GFileDownloadRequestHelperM5::CAuthLen);

	GFileDownloadRequestHelperM5 oDownloadRequestHelper;

	EXPECT_TRUE(oDownloadRequestHelper.ProcessResourceFile(strResourceFile.c_str()));

	EXPECT_EQ(oDownloadRequestHelper.GetFileId(), 1);

	EXPECT_EQ(oDownloadRequestHelper.GetFileSize(), (u32Size - oDownloadRequestHelper.GetResmarkSize()));
	
	//test files has been hacked with this value
	UINT32 u32TestCRC = 0x01010101;
	EXPECT_EQ(oDownloadRequestHelper.GetCRC(), u32TestCRC);
	
	EXPECT_EQ(oDownloadRequestHelper.GetAuthenType(), 0);

	UINT8 aryAuthentication[GFileDownloadRequestHelperM5::CAuthLen];
	ASSERT_TRUE(oDownloadRequestHelper.GetAuthentication(aryAuthentication, sizeof(aryAuthentication)) != NULL);

	EXPECT_EQ(memcmp(aryDummyAuthentication, aryAuthentication, GFileDownloadRequestHelperM5::CAuthLen), 0);

	EXPECT_STREQ("scr_img_01.txt", oDownloadRequestHelper.GetDescription());
}
