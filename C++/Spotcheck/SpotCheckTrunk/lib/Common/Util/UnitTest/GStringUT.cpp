
#include "gtest/gtest.h"

#include "Common/Util/GString.h"

#include <string>
using namespace std;

TEST(GString, ShouldReturnCorrectString)
{
	GString testedString("xxxx");

	EXPECT_EQ(string("xxxx"), testedString.GetString());
}


TEST(GString, ShouldReturnCorrectLength)
{
	GString testedString("xxxx");

	EXPECT_EQ(4, testedString.GetLength());
}

TEST(GString, ShouldReturnCorrectLengthWhenCopy)
{
	GString otherString("xxxx");
	GString testedString = otherString;

	EXPECT_EQ(4, testedString.GetLength());
}

TEST(GString, ShouldReturnCorrectLengthWhenCopyCtor)
{
	GString otherString("xxxx");
	GString testedString(otherString);

	EXPECT_EQ(4, testedString.GetLength());
}


TEST(GString, ShouldReturnCorrectStringWhenCopyCtor)
{
	GString otherString("xxxx");
	GString testedString(otherString);

	EXPECT_EQ(4, testedString.GetLength());
	EXPECT_EQ(string("xxxx"), testedString.GetString());
	EXPECT_EQ(otherString, testedString.GetString());
}


TEST(GString, ShouldReturnCorrectLengthWhenCopyFromMemory)
{
	char otherStringFromMemory[] = "xxxx";
	GString testedString;
	testedString.MemCopy((UINT8*)otherStringFromMemory, strlen(otherStringFromMemory));

	//Don't know why original code does add one char. However, the remark in the MemCopy is trying to really scare the reader so leave it as is :)
	EXPECT_EQ(5, testedString.GetLength());
}

TEST(GString, ShouldReturnCorrectStringWhenCopyFromMemory)
{
	char otherStringFromMemory[] = "xxxx";
	GString testedString;
	testedString.MemCopy((UINT8*)otherStringFromMemory, strlen(otherStringFromMemory));

	EXPECT_EQ(string(otherStringFromMemory), testedString.GetString());
}

TEST(GString, ShouldReturnTrueWhenComparedToOtherSameString)
{
	const char* otherString = "xxxx";
	GString testedString("xxxx");

	EXPECT_EQ(otherString, testedString);
	EXPECT_TRUE(testedString == otherString);
}



