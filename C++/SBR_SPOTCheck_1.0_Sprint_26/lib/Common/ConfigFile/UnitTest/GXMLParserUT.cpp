#include <Common/ConfigFile/GXMLParser.h>
#include <Common/ConfigFile/GConfigItem.h>
#include "GXMLParserUT.h"
#include <list>
#include <string>

using namespace GConfigTypes;


TEST(GXMLParser, Load)
{
	// Define the file path to the input data
	std::string File("TestFiles/TestFile.xml");

	// Create an empty list of GConfigItems
	std::list<GConfigItem> ConfigItemList;

	// Parse the input file
	GXMLParser Parser(File, ConfigItemList);

	// Load and parse the input file
	EXPECT_EQ(Parser.Load(), eConfigOk);

	// Make sure the parser returned 5 GConfigItems, matching the number of items in the file
	ASSERT_EQ(ConfigItemList.size(), 5);

	// Create an iterator to the list of GConfigItems and start it at the beginning
	std::list<GConfigItem>::iterator Itr = ConfigItemList.begin();

	// Check the first node in the file
	EXPECT_STREQ((*Itr).GetName().c_str(), "Name");
	EXPECT_STREQ((*Itr).GetValue().c_str(), "Filename.xml");
	EXPECT_STREQ((*Itr).GetXMLPath().c_str(), "/Root");

	// This attribute doesn't exist, expect an exception to be thrown

	std::string sValueReturn;
	EXPECT_EQ((*Itr).GetAttribute("binary",sValueReturn), eConfigError);
	Itr++;

	// Check the second node in the file
	EXPECT_STREQ((*Itr).GetName().c_str(), "Purpose");
	EXPECT_STREQ((*Itr).GetValue().c_str(), "Unit Test");
	EXPECT_STREQ((*Itr).GetXMLPath().c_str(), "/Root");

	// This attribute doesn't exist, expect an exception to be thrown
	EXPECT_EQ((*Itr).GetAttribute("binary",sValueReturn), eConfigError);

	// Advance the iterator
	Itr++;

	// Check the third node in the file
	EXPECT_STREQ((*Itr).GetName().c_str(), "type");
	EXPECT_STREQ((*Itr).GetValue().c_str(), "XML");
	EXPECT_STREQ((*Itr).GetXMLPath().c_str(), "/Root");

	// This attribute does exist, check the returned value
	EXPECT_EQ((*Itr).GetAttribute("binary",sValueReturn), eConfigOk);
	EXPECT_STREQ(sValueReturn.c_str(), "false");

	// This attribute doesn't exist, expect an exception to be thrown
	EXPECT_EQ((*Itr).GetAttribute("BINARY",sValueReturn), eConfigError);

	// Advance the iterator
	Itr++;

	// Check the fourth node in the file
	EXPECT_STREQ((*Itr).GetName().c_str(), "Level2");
	EXPECT_STREQ((*Itr).GetValue().c_str(), "Yes");
	EXPECT_STREQ((*Itr).GetXMLPath().c_str(), "/Root/deeper");

	// This attribute doesn't exist, expect an exception to be thrown
	EXPECT_EQ((*Itr).GetAttribute("binary",sValueReturn), eConfigError);

	// Advance the iterator
	Itr++;

	// Check the fifth node in the file
	EXPECT_STREQ((*Itr).GetName().c_str(), "Level3");
	EXPECT_STREQ((*Itr).GetValue().c_str(), "Yes");
	EXPECT_STREQ((*Itr).GetXMLPath().c_str(), "/Root/deeper/deeperstill");

	// This attribute doesn't exist, expect an exception to be thrown
	EXPECT_EQ((*Itr).GetAttribute("binary",sValueReturn), eConfigError);
}

