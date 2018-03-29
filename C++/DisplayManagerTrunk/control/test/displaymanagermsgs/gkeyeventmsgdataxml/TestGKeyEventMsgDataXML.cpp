#include "TestGKeyEventMsgDataXML.h"

#include "control/src/displaymanagermsgs/xml/GKeyEventMsgDataXML.h"

#include "utils/logger/src/GLogger.h"
#include "utils/logger/src/GLogConfig.h"
#include "utils/logger/src/GLogManager.h"

void TestGKeyEventMsgDataXML::initTestCase()
{
  	GLogConfig* poConfig;
  	GLogger* poLogger;

  	// Logger StartUp
  	poConfig = new GLogConfig(GLogConfig::eBoth, GLogConfig::eLevel1, "./DisplayManager.log");
	poLogger = new GLogger(poConfig);
	GLogManager::AddLogger("DisplayManager.log", poLogger, false);

	LOG_LEVEL3("Starting Unit Test...");
}

void TestGKeyEventMsgDataXML::cleanupTestCase()
{
	LOG_LEVEL4("cleanupTestCase()");

}

void TestGKeyEventMsgDataXML::gKeyEventMsgDataUnMarshall()
{
	LOG_LEVEL4("gKeyEventMsgDataUnMarshall()");
	
	QByteArray oXML("<?xml version=\"1.0\" encoding=\"utf-8\"?>" \
				   "<key_event xmlns=\"http://www.gilbarco.com/crind/displayManager/api\">" \
					   "<interface_version>01.0.01</interface_version>" \
				   	   "<key_code>3</key_code>" \
				   "</key_event>");

	GKeyEventMsgData *oRequest = GKeyEventMsgDataXML::UnMarshall(oXML);

	QVERIFY(oRequest != NULL);	
	QCOMPARE(oRequest->GetInterfaceVersion() , QString("01.0.01"));
	QCOMPARE(oRequest->GetKeyCode() , QString("3"));
}

QTEST_MAIN(TestGKeyEventMsgDataXML)
