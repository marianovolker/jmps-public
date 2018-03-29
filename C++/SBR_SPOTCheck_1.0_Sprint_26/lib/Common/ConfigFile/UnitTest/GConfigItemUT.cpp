#include <Common/ConfigFile/GConfigItem.h>
#include "GConfigItemUT.h"
#include <map>
#include <string>

using namespace GConfigTypes;


TEST(GConfigItemUT, GetAttribute)
{
	// Create an attributes container, keys are all uppercase because the GXMLParser class
	// converts all read keys to upper case prior to storing them.
	std::map<std::string, std::string> Attributes;
	Attributes["ATTR1"] = "Value 1";
	Attributes["ATTR2"] = "Value 2";
	Attributes["ATTR3"] = "Value 3";

	// Default Values for Name, Value and XML Path
	std::string Name("SomeNode");
	std::string Value("SomeValue");
	std::string XMLPath("/unitTest/Node");

	// Create a GConfigItem with the previously defined information
	GConfigItem MyConfigItem(Name, Value, XMLPath, Attributes);

	std::string sValueReturn;

	// Retrieve each attribute making sure the returned value matches the expected
	EXPECT_EQ(MyConfigItem.GetAttribute("ATTR1",sValueReturn), eConfigOk);
	EXPECT_STREQ(sValueReturn.c_str(), "Value 1");
	EXPECT_EQ(MyConfigItem.GetAttribute("ATTR2",sValueReturn), eConfigOk);
	EXPECT_STREQ(sValueReturn.c_str(), "Value 2");
	EXPECT_EQ(MyConfigItem.GetAttribute("ATTR3",sValueReturn), eConfigOk);
	EXPECT_STREQ(sValueReturn.c_str(), "Value 3");

	sValueReturn = "";

	// Request an attribute with it's name using the wrong case, make sure it throws an exception
	EXPECT_EQ(MyConfigItem.GetAttribute("Attr3",sValueReturn), eConfigError);

	// Request an attribute that does not exist, make sure it throws an exception
	EXPECT_EQ(MyConfigItem.GetAttribute("ATTR4",sValueReturn), eConfigError);
}



TEST(GConfigItemUT, GetName)
{
   // Default Attributes
   std::map<std::string, std::string> Attributes;

   // Default Values for Name, Value and XML Path
   std::string Name("SomeNode");
   std::string Value("SomeValue");
   std::string XMLPath("/unitTest/Node");

   // Create a GConfigItem with the previously defined information
   GConfigItem MyConfigItem(Name, Value, XMLPath, Attributes);

   // Make sure the name returned is the name we set
   EXPECT_STREQ(MyConfigItem.GetName().c_str(), "SomeNode");
}

TEST(GConfigItemUT, GetValue)
{
   // Default Attributes
   std::map<std::string, std::string> Attributes;

   // Default Values for Name, Value and XML Path
   std::string Name("SomeNode");
   std::string Value("SomeValue");
   std::string XMLPath("/unitTest/Node");

   // Create a GConfigItem with the previously defined information
   GConfigItem MyConfigItem(Name, Value, XMLPath, Attributes);

   // Make sure the Value returned is the value we set
   EXPECT_STREQ(MyConfigItem.GetValue().c_str(), "SomeValue");
}

TEST(GConfigItemUT, GetXMLPath)
{
   // Default Attributes
   std::map<std::string, std::string> Attributes;

   // Default Values for Name, Value and XML Path
   std::string Name("SomeNode");
   std::string Value("SomeValue");
   std::string XMLPath("/unitTest/Node");

   // Create a GConfigItem with the previously defined information
   GConfigItem MyConfigItem(Name, Value, XMLPath, Attributes);

   // Make sure the XMLPath returned is the path we set
   EXPECT_STREQ(MyConfigItem.GetXMLPath().c_str(), "/unitTest/Node");
}



TEST(GConfigItemUT, SetValue)
{
   // Default Attributes
   std::map<std::string, std::string> Attributes;

   // Default Values for Name, Value and XML Path
   std::string Name("SomeNode");
   std::string Value("SomeValue");
   std::string XMLPath("/unitTest/Node");

   // Create a GConfigItem with the previously defined information
   GConfigItem MyConfigItem(Name, Value, XMLPath, Attributes);

   // Make sure the value returned is the value we set
   EXPECT_STREQ(MyConfigItem.GetValue().c_str(), "SomeValue");

   // Set a new value
   MyConfigItem.SetValue("NewValue");

   // Make sure the new value returned is the new value we set
   EXPECT_STREQ(MyConfigItem.GetValue().c_str(), "NewValue");
}

