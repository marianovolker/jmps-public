#include <Common/ConfigFile/GConfigFile.h>
#include "GConfigFileUT.h"
#include <list>
#include <string>
#include "GFile.h"

using namespace GConfigTypes;


TEST(GConfigFileUT, Load)
{
   // Create an instance of a GConfigFile, parsing the specified file
   GConfigFile ConfigFile("TestFiles/TestFile.xml", false);

   EXPECT_EQ(ConfigFile.Load(), eConfigOk);
}


TEST(GConfigFileUT, GetValue)
{
   // Create an instance of a GConfigFile, parsing the specified file
   GConfigFile ConfigFile("TestFiles/TestFile.xml", false);

   EXPECT_EQ(ConfigFile.Load(), eConfigOk);

   // Test existance of each of the nodes in the file
   std::string Value;
   ConfigFile.GetValue("/Root/Name", Value);
   EXPECT_STREQ(Value.c_str(), "Filename.xml");

   ConfigFile.GetValue("/Root/Purpose", Value);
   EXPECT_STREQ(Value.c_str(), "Unit Test");

   ConfigFile.GetValue("/Root/type", Value);
   EXPECT_STREQ(Value.c_str(), "XML");

   ConfigFile.GetValue("/Root/deeper/Level2", Value);
   EXPECT_STREQ(Value.c_str(), "Yes");

   ConfigFile.GetValue("/Root/deeper/deeperstill/Level3", Value);
   EXPECT_STREQ(Value.c_str(), "Yes");

   // Make sure we get an error when requesting a non existant value
   EXPECT_EQ(ConfigFile.GetValue("/Root/You/Dont/Exist", Value), eConfigItemDoesNotExist);
}


TEST(GConfigFileUT, XMLPathExists)
{
   // Create an instance of a GConfigFile, parsing the specified file
   GConfigFile ConfigFile("TestFiles/TestFile.xml", false);

   EXPECT_EQ(ConfigFile.Load(), eConfigOk);

   // Check for the existance of each of the XML Paths in the file
   EXPECT_TRUE (ConfigFile.XMLPathExists("/Root/Name"));
   EXPECT_TRUE (ConfigFile.XMLPathExists("/Root/Purpose"));
   EXPECT_TRUE (ConfigFile.XMLPathExists("/Root/type"));
   EXPECT_TRUE (ConfigFile.XMLPathExists("/Root/deeper/Level2"));
   EXPECT_TRUE (ConfigFile.XMLPathExists("/Root/deeper/deeperstill/Level3"));

   // Make sure we don't get a false positive when requesting a non existant path
   EXPECT_FALSE(ConfigFile.XMLPathExists("/Root/NotHere"));
}



TEST(GConfigFileUT, SetValue)
{
   // Create an instance of a GConfigFile, parsing the specified file
   GConfigFile ConfigFile("TestFiles/TestFile.xml", false);

   EXPECT_EQ(ConfigFile.Load(), eConfigOk);

   std::string Value;

   // Change a value, read it back and compare the expected and actuals
   ConfigFile.SetValue("/Root/Name", "Newname.xml");
   ConfigFile.GetValue("/Root/Name", Value);
   EXPECT_STREQ(Value.c_str(), "Newname.xml");

   // Change a value, read it back and compare the expected and actuals
   ConfigFile.SetValue("/Root/Purpose", "Test");
   ConfigFile.GetValue("/Root/Purpose", Value);
   EXPECT_STREQ(Value.c_str(), "Test");

   // Change a value, read it back and compare the expected and actuals
   ConfigFile.SetValue("/Root/type", "File");
   ConfigFile.GetValue("/Root/type", Value);
   EXPECT_STREQ(Value.c_str(), "File");
}



TEST(GConfigFileUT, Save)
{
   const char* tempConfigFile = "TestFiles/TempFile.xml";
   // Generate temporary config file
   GSystem::CopyFile("TestFiles/TestFile.xml", tempConfigFile);

   // Create an instance of a GConfigFile, parsing the specified file
   GConfigFile ConfigFile(tempConfigFile, false);

   EXPECT_EQ(ConfigFile.Load(), eConfigOk);

   // Change some values
   ConfigFile.SetValue("/Root/Name", "Newname.xml");
   ConfigFile.SetValue("/Root/Purpose", "Test");
   ConfigFile.SetValue("/Root/type", "File");

   // Save the changes to the file
   ConfigFile.Save();

   // Reload the config file to pick up the changes
   ConfigFile.Load();
   std::string Value;

   // Compare the values changes to expected values
   ConfigFile.GetValue("/Root/Name", Value);
   EXPECT_STREQ(Value.c_str(), "Newname.xml");

   // Compare the values changes to expected values
   ConfigFile.GetValue("/Root/Purpose", Value);
   EXPECT_STREQ(Value.c_str(), "Test");

   // Compare the values changes to expected values
   ConfigFile.GetValue("/Root/type", Value);
   EXPECT_STREQ(Value.c_str(), "File");

   GSystem::RemoveFile(tempConfigFile);
}



TEST(GConfigFileUT, GetIndex)
{
   // Create an instance of a GConfigFile, parsing the specified file
   GConfigFile ConfigFile("TestFiles/TestFile.xml", false);

   EXPECT_EQ(ConfigFile.Load(), eConfigOk);

   // Get the index of this config file
   std::list<std::string> List = ConfigFile.GetIndex();

   // Create the expected results
   std::list<std::string> ExpectedList;
   ExpectedList.push_back("/Root/Name");
   ExpectedList.push_back("/Root/Purpose");
   ExpectedList.push_back("/Root/type");
   ExpectedList.push_back("/Root/deeper/Level2");
   ExpectedList.push_back("/Root/deeper/deeperstill/Level3");

   // Create and initialize iterators for both lists
   std::list<std::string>::iterator ActItr = List.begin();
   std::list<std::string>::iterator ExpItr = ExpectedList.begin();

   // Step through both lists and compare expected to actual
   for(;ActItr != List.end() && ExpItr != ExpectedList.end();ActItr++, ExpItr++)
   {
      EXPECT_STREQ((*ExpItr).c_str(), (*ActItr).c_str());
   }
}

