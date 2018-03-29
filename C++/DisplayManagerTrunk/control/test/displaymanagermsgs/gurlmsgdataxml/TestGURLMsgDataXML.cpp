#include "TestGURLMsgDataXML.h"

#include "control/src/displaymanagermsgs/xml/GURLMsgDataXML.h"

#include "utils/logger/src/GLogger.h"
#include "utils/logger/src/GLogConfig.h"
#include "utils/logger/src/GLogManager.h"

void TestGURLMsgDataXML::initTestCase()
{
  	GLogConfig* poConfig;
  	GLogger* poLogger;

  	// Logger StartUp
  	poConfig = new GLogConfig(GLogConfig::eBoth, GLogConfig::eLevel1, "./DisplayManager.log");
	poLogger = new GLogger(poConfig);
	GLogManager::AddLogger("DisplayManager.log", poLogger, false);

	LOG_LEVEL3("Starting Unit Test...");
}

void TestGURLMsgDataXML::cleanupTestCase()
{
	LOG_LEVEL4("cleanupTestCase()");

}

void TestGURLMsgDataXML::GURLMsgDataUnMarshall()
{
	LOG_LEVEL4("gSetPersonalityMsgDataUnMarshall()");
	
	QByteArray oXML("<?xml version=\"1.0\" encoding=\"utf-8\"?>" \
				   "<url_request xmlns=\"http://www.gilbarco.com/crind/display_manager/api\">" \
				   	   	"<interface_version>01.0.01</interface_version>" \
						"<url>http://127.0.0.1</url>" \
				   "</url_request>");
	
	GURLMsgData *oRequest = GURLMsgDataXML::UnMarshall(oXML);
	
	QVERIFY(oRequest != NULL);	
	QCOMPARE(oRequest->GetInterfaceVersion() , QString("01.0.01"));
	QCOMPARE(oRequest->GetURL() , QString("http://127.0.0.1"));
}

QTEST_MAIN(TestGURLMsgDataXML)
