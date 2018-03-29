#include <Common/ConfigFile/GConfigUtils.h>
#include "GConfigUtilsUT.h"
#include <list>
#include <string>



TEST(GConfigUtilsUT, StringSplit)
{
   // Container to hold string parts
   std::list<std::string> StringParts;

   // Input string to split
   std::string InputString("/this/is/a/test/string");

   // Delimter character to split string on
   char s8Delim('/');

   // Split string using method under test
   GConfigUtils::StringSplit(InputString, s8Delim, StringParts);

   // Iterator to string parts
   std::list<std::string>::iterator StringPartsItr;

   // Array of expected values
   std::string ExpectedOutput[5] = {"this","is","a","test","string"};

   // Array counter
   unsigned int u32Counter(0);

   // Loop through the string parts container
   for(StringPartsItr=StringParts.begin();StringPartsItr!=StringParts.end();StringPartsItr++)
   {
      // Each part in the container should match it's corresponding part in the array
      EXPECT_STREQ((*StringPartsItr).c_str(), ExpectedOutput[u32Counter++].c_str());
   }
}



TEST(GConfigUtilsUT, CombinedString)
{
   // Container of string parts to combine
   std::list<std::string> StringParts;

   // Delimiter character to place between string parts
   char s8Delim('/');

   // Add string parts to input container
   StringParts.push_back("this");
   StringParts.push_back("is");
   StringParts.push_back("a");
   StringParts.push_back("test");
   StringParts.push_back("string");

   // Combine the input string parts using the method under test
   std::string Result(GConfigUtils::CombinedString(StringParts, s8Delim));

   // Expected output string
   const std::string ExpectedResult("/this/is/a/test/string");

   // The expected result should match the actual result
   EXPECT_STREQ(ExpectedResult.c_str(), Result.c_str());
}



TEST(GConfigUtilsUT, LTrim)
{
   // Define the input strings for this test
   std::string Input1   ("   LTrim   ");
   std::string Input2   ("LTrim   ");
   std::string Input3   ("   LTrim");
   
   // Define the expected output from the method under test
   std::string Expected1("LTrim   ");
   std::string Expected2("LTrim   ");
   std::string Expected3("LTrim");

   // Check expected vs actuals
   EXPECT_STREQ(GConfigUtils::LTrim(Input1).c_str(), Expected1.c_str());
   EXPECT_STREQ(GConfigUtils::LTrim(Input2).c_str(), Expected2.c_str());
   EXPECT_STREQ(GConfigUtils::LTrim(Input3).c_str(), Expected3.c_str());
}



TEST(GConfigUtilsUT, RTrim)
{
   // Define the input strings for this test
   std::string Input1   ("   RTrim   ");
   std::string Input2   ("RTrim   ");
   std::string Input3   ("   RTrim");
   
   // Define the expected output from the method under test
   std::string Expected1("   RTrim");
   std::string Expected2("RTrim");
   std::string Expected3("   RTrim");

   // Check expected vs actuals
   EXPECT_STREQ(GConfigUtils::RTrim(Input1).c_str(), Expected1.c_str());
   EXPECT_STREQ(GConfigUtils::RTrim(Input2).c_str(), Expected2.c_str());
   EXPECT_STREQ(GConfigUtils::RTrim(Input3).c_str(), Expected3.c_str());
}



TEST(GConfigUtilsUT, Trim)
{
   // Define the input strings for this test
   std::string Input1   ("   Trim   ");
   std::string Input2   ("Trim   ");
   std::string Input3   ("   Trim");
   
   // Define the expected output from the method under test
   std::string Expected1("Trim");
   std::string Expected2("Trim");
   std::string Expected3("Trim");

   // Check expected vs actuals
   EXPECT_STREQ(GConfigUtils::Trim(Input1).c_str(), Expected1.c_str());
   EXPECT_STREQ(GConfigUtils::Trim(Input2).c_str(), Expected2.c_str());
   EXPECT_STREQ(GConfigUtils::Trim(Input3).c_str(), Expected3.c_str());
}

