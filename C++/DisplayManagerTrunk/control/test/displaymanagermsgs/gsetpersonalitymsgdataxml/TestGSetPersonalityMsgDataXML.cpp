#include "TestGSetPersonalityMsgDataXML.h"

#include "control/src/displaymanagermsgs/xml/GSetPersonalityMsgDataXML.h"

#include "utils/logger/src/GLogger.h"
#include "utils/logger/src/GLogConfig.h"
#include "utils/logger/src/GLogManager.h"

void TestGSetPersonalityMsgDataXML::initTestCase()
{
  	GLogConfig* poConfig;
  	GLogger* poLogger;

  	// Logger StartUp
  	poConfig = new GLogConfig(GLogConfig::eBoth, GLogConfig::eLevel1, "./DisplayManager.log");
	poLogger = new GLogger(poConfig);
	GLogManager::AddLogger("DisplayManager.log", poLogger, false);

	LOG_LEVEL3("Starting Unit Test...");
}

void TestGSetPersonalityMsgDataXML::cleanupTestCase()
{
	LOG_LEVEL4("cleanupTestCase()");

}

void TestGSetPersonalityMsgDataXML::gSetPersonalityMsgDataUnMarshall()
{
	LOG_LEVEL4("gSetPersonalityMsgDataUnMarshall()");
	
	QByteArray oXML("<?xml version=\"1.0\" encoding=\"utf-8\"?>" \
				   "<set_personality_request xmlns=\"http://www.gilbarco.com/crind/displayManager/api\">" \
						"<interface_version>01.0.01</interface_version>" \
						"<personality>shell</personality>" \
				   "</set_personality_request>");
	
	GSetPersonalityMsgData *oRequest = GSetPersonalityMsgDataXML::UnMarshall(oXML);

	QVERIFY(oRequest != NULL);	
	QCOMPARE(oRequest->GetInterfaceVersion() , QString("01.0.01"));
	QCOMPARE(oRequest->GetPersonality() , QString("shell"));
}

QTEST_MAIN(TestGSetPersonalityMsgDataXML)
