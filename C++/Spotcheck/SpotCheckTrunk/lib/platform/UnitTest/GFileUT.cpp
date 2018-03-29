// GFileUT.cpp

#include "GFileUT.h"
#include <GFile.h>
#include <errno.h>


const string CTestFileName("testfile.ut");
const char * CTestData("A1B2C3D4E5F6G7");


GFileUT::GFileUT()
{
}

GFileUT::~GFileUT()
{
}

void GFileUT::SetUp()
{
}

void GFileUT::TearDown()
{
}

// TestOpenClose

TEST_F(GFileUT, TestOpenClose)
{
	GFile TestFile(CTestFileName);
	bool  bRet(false);
	int   iRet(0);

	// Start with no file present.
	if (unlink(CTestFileName.c_str()) < 0)
	{
		EXPECT_EQ(errno,ENOENT);
	}

	// Expect readonly open/close to fail for non-existent file.
	bRet = TestFile.Open(eOpenReadOnly);
	EXPECT_EQ(bRet, false);
	bRet = TestFile.Close();
	EXPECT_EQ(bRet, false);

	// Expect readwrite open/close to fail for non-existent file.
	bRet = TestFile.Open(eOpenReadWrite);
	EXPECT_EQ(bRet, false);
	bRet = TestFile.Close();
	EXPECT_EQ(bRet, false);

	// Expect writeonly open/close to fail for non-existent file.
	bRet = TestFile.Open(eOpenWriteOnly);
	EXPECT_EQ(bRet, false);
	bRet = TestFile.Close();
	EXPECT_EQ(bRet, false);

	// Expect writenew open/close to succeed for non-existent file (file created).
	bRet = TestFile.Open(eOpenWriteNew);
	EXPECT_EQ(bRet, true);
	bRet = TestFile.Close();
	EXPECT_EQ(bRet, true);

	// Remove test file.
	iRet = unlink(CTestFileName.c_str());
	EXPECT_EQ(iRet, 0);

	// Expect writenew open/close to succeed for existing file.
	bRet = TestFile.Open(eOpenWriteNew);
	EXPECT_EQ(bRet, true);
	bRet = TestFile.Close();
	EXPECT_EQ(bRet, true);

	// remove test file.
	iRet = unlink(CTestFileName.c_str());
	EXPECT_EQ(iRet, 0);

	// Expect append open/close to succeed for non-existent file (file created).
	bRet = TestFile.Open(eOpenAppend);
	EXPECT_EQ(bRet, true);
	bRet = TestFile.Close();
	EXPECT_EQ(bRet, true);

	// Expect append open/close to succeed for existing file.
	bRet = TestFile.Open(eOpenAppend);
	EXPECT_EQ(bRet, true);
	bRet = TestFile.Close();
	EXPECT_EQ(bRet, true);

	// Remove test file.
	unlink(CTestFileName.c_str());
}

TEST_F(GFileUT, TestReadWrite)
{
	GFile TestFile(CTestFileName);
	char  TestBuf[100];
	bool  bRet(false);
	int   BytesWritten(0), BytesRead(0), iRet(0);
	int   TestDataLen(strlen(CTestData));

	// Create file with test text.
	bRet = TestFile.Open(eOpenWriteNew);
	EXPECT_EQ(bRet,true);
	bRet = TestFile.Write(CTestData,TestDataLen,BytesWritten);
	EXPECT_EQ(bRet,true);
	EXPECT_EQ(BytesWritten,TestDataLen);

	unsigned long ulSize(0);
	bRet = TestFile.Size(ulSize);
	EXPECT_EQ(bRet,true);
	EXPECT_EQ(TestDataLen,ulSize);

	bRet = TestFile.Close();
	EXPECT_EQ(bRet,true);

	// Read file and validate contents.
	bRet = TestFile.Open(eOpenReadOnly);
	EXPECT_EQ(bRet,true);
	bRet = TestFile.Read(TestBuf,sizeof(TestBuf),BytesRead);
	EXPECT_EQ(bRet,true);
	EXPECT_EQ(BytesRead,TestDataLen);
	iRet = strncmp(TestBuf,CTestData,TestDataLen);
	EXPECT_EQ(iRet,0);
	bRet = TestFile.Read(TestBuf,sizeof(TestBuf),BytesRead);
	EXPECT_EQ(bRet,true);
	EXPECT_EQ(BytesRead,0);
	bRet = TestFile.Close();
	EXPECT_EQ(bRet,true);


	// Remove test file.
	unlink(CTestFileName.c_str());
}
