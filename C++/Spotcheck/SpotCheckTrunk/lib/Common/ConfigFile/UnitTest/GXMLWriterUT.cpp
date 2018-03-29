#include <GSystem.h>
#include <Common/ConfigFile/GXMLParser.h>
#include <Common/ConfigFile/GXMLWriter.h>
#include <Common/ConfigFile/GConfigItem.h>
#include "GXMLWriterUT.h"
#include <list>
#include <string>

using namespace GConfigTypes;

// In order to test the write capabilities of the GXMLWriter class it was necessary to create a
// function to compare to lists of GConfigItems. 
bool Equal(std::list<GConfigItem>& Left, std::list<GConfigItem> Right)
{
	// Create iterators for an inner and outer loop
	std::list<GConfigItem>::iterator InnerLoop;
	std::list<GConfigItem>::iterator OuterLoop;

	// Return value
	bool b_ReturnValue   = true;

	// Make sure the lists are the right size
	if(Left.size() == Right.size())
	{
		// For each loop we need to know if we found a match, if we don't, the lists aren't equal
		bool b_FoundThisLoop = false;

		// Loop through the first list
		for(OuterLoop=Left.begin();OuterLoop!=Left.end();OuterLoop++)
		{
			// Reset the found flag
			b_FoundThisLoop = false;

			// Loop through the second list
			for(InnerLoop=Right.begin();InnerLoop!=Right.end();InnerLoop++)
			{
				// Check to see if the current objects are equal
				if(*OuterLoop == *InnerLoop)
				{
					// Match was found set the flag
					b_FoundThisLoop = true;

					// We don't need to look for another match, these objects are unique
					break;
				}
			}

			// Make sure we've found something
			if(!b_FoundThisLoop)
			{
				// We didn't find anything, these lists are not equal, we can return now
				b_ReturnValue = false;
				break;
			}
		}
	}
	else
	{
		// The lists were different sizes and therefore not equal
		b_ReturnValue = false;
	}

	// Return the value
	return b_ReturnValue;
}



TEST(GXMLWriterUT, Save)
{
	// Name of the input file
	std::string File1("TestFiles/TestFile.xml");

	// List of Config Items from the input file
	std::list<GConfigItem> ConfigItemList1;

	// Parse the input file
	GXMLParser Parser1(File1, ConfigItemList1);

	// Load and parse the input file
	EXPECT_EQ(Parser1.Load(), eConfigOk);

	// Check there are 5 config items in the input file
	ASSERT_EQ(ConfigItemList1.size(), 5);

	// Name of the output file
	std::string File2("TestFiles/WriteTest.xml");

	// Create the writer
	GXMLWriter Writer(File2);

	// Iterator to the config items from the input file
	std::list<GConfigItem>::iterator Itr;

	// Loop through the list
	for(Itr = ConfigItemList1.begin();Itr != ConfigItemList1.end();Itr++)
	{
	// Add the config items to the writer
		Writer.AddConfigItem(*Itr);
	}

	// Save the file
	Writer.Save();

	// List of config items from the output file
	std::list<GConfigItem> ConfigItemList2;

	// Parse the output file
	GXMLParser Parser2(File2, ConfigItemList2);

	// Load and parse the input file
	EXPECT_EQ(Parser2.Load(), eConfigOk);

	// Check there are 5 config items in the input file
	EXPECT_EQ(ConfigItemList2.size(), 5);

	// Test our equality routine
	EXPECT_TRUE(Equal(ConfigItemList1, ConfigItemList1));

	// Verify the original file is the same as the written file
	EXPECT_TRUE(Equal(ConfigItemList1, ConfigItemList2));

	// Clean up after ourselves, delete the written file
	EXPECT_TRUE(GSystem::RemoveFile(File2.c_str()));
}

