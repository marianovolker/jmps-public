#include "TestGPersonality.h"

#include "utils/personality/src/GPersonality.h"

#include "utils/logger/src/GLogger.h"
#include "utils/logger/src/GLogConfig.h"
#include "utils/logger/src/GLogManager.h"

void TestGPersonality::initTestCase()
{
  	GLogConfig* poConfig;
  	GLogger* poLogger;

  	// Logger StartUp
  	poConfig = new GLogConfig(GLogConfig::eBoth, GLogConfig::eLevel1, "./DisplayManager.log");
	poLogger = new GLogger(poConfig);
	GLogManager::AddLogger("DisplayManager.log", poLogger, false);

	LOG_LEVEL3("Starting Unit Test...");
}

void TestGPersonality::cleanupTestCase()
{
	LOG_LEVEL4("cleanupTestCase()");

}

void TestGPersonality::GPersonalityDisplayManagerPropertiesFileInit()
{
	LOG_LEVEL4("GPersonalityDisplayManagerPropertiesFileInit()");

	try
	{
		GPersonality oPersonality;
		oPersonality.Init();
		QVERIFY(true);
	}
	catch(GException & e)
	{
		LOG_LEVEL1( QString("GPersonalityDisplayManagerPropertiesFileInit::Exception [") + e.GetMessage() + "]" );
		QVERIFY(false);
	}
}

void TestGPersonality::GPersonality320x240Init()
{
	LOG_LEVEL4("GPersonality320x240Init()");

	try
	{
		GPersonality oPersonality;
		oPersonality.Init("320x240","exxon.ruby");
		QVERIFY(true);
	}
	catch(GException & e)
	{
		LOG_LEVEL1( QString("GPersonality320x240Init::Exception [") + e.GetMessage() + "]" );
		QVERIFY(false);
	}
}

void TestGPersonality::GPersonality640x480Init()
{
	LOG_LEVEL4("GPersonality640x480Init()");
	
	try
	{
		GPersonality oPersonality;
		oPersonality.Init("640x480","exxon.ruby");
		QVERIFY(true);
	}
	catch(GException & e)
	{
		LOG_LEVEL1( QString("GPersonality640x480Init::Exception [") + e.GetMessage() + "]" );
		QVERIFY(false);
	}
}

void TestGPersonality::GPersonalityFallbackInit()
{
	LOG_LEVEL4("GPersonalityFallbackInit()");

	try
	{
		GPersonality oPersonality;
		oPersonality.Init("320x240","whatever");
		QVERIFY(true);
	}
	catch(GException & e)
	{
		LOG_LEVEL1( QString("GPersonalityFallbackInit::Exception [") + e.GetMessage() + "]" );
		QVERIFY(false);
	}
}

QTEST_MAIN(TestGPersonality)
