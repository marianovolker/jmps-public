#include "TestGKioskIdMsgDataXML.h"

#include "control/src/displaymanagermsgs/xml/GKioskIdMsgDataXML.h"

#include "utils/logger/src/GLogger.h"
#include "utils/logger/src/GLogConfig.h"
#include "utils/logger/src/GLogManager.h"

void TestGKioskIdMsgDataXML::initTestCase()
{
  	GLogConfig* poConfig;
  	GLogger* poLogger;

  	// Logger StartUp
  	poConfig = new GLogConfig(GLogConfig::eBoth, GLogConfig::eLevel1, "./DisplayManager.log");
	poLogger = new GLogger(poConfig);
	GLogManager::AddLogger("DisplayManager.log", poLogger, false);

	LOG_LEVEL3("Starting Unit Test...");
}

void TestGKioskIdMsgDataXML::cleanupTestCase()
{
	LOG_LEVEL4("cleanupTestCase()");

}

void TestGKioskIdMsgDataXML::gKioskIdMsgDataUnMarshall()
{
	LOG_LEVEL4("gKioskIdMsgDataUnMarshall()");
	
	QByteArray oXML("<kiosk_id_request xmlns=\"http://www.gilbarco.com/crind/displayManager/api\">" \
						"<interface_version>01.0.01</interface_version>" \
					"</kiosk_id_request>");

	GKioskIdMsgData *oRequest = GKioskIdMsgDataXML::UnMarshall(oXML);

	QVERIFY(oRequest != NULL);	
	QCOMPARE(oRequest->GetInterfaceVersion() , QString("01.0.01"));
}

QTEST_MAIN(TestGKioskIdMsgDataXML)
