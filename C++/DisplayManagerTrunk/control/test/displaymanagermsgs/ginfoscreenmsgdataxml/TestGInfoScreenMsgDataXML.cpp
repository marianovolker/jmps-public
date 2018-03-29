#include "TestGInfoScreenMsgDataXML.h"

#include "control/src/displaymanagermsgs/xml/GInfoScreenMsgDataXML.h"

#include "utils/logger/src/GLogger.h"
#include "utils/logger/src/GLogConfig.h"
#include "utils/logger/src/GLogManager.h"

void TestGInfoScreenMsgDataXML::initTestCase()
{
  	GLogConfig* poConfig;
  	GLogger* poLogger;

  	// Logger StartUp
  	poConfig = new GLogConfig(GLogConfig::eBoth, GLogConfig::eLevel1, "./DisplayManager.log");
	poLogger = new GLogger(poConfig);
	GLogManager::AddLogger("DisplayManager.log", poLogger, false);

	LOG_LEVEL3("Starting Unit Test...");
}

void TestGInfoScreenMsgDataXML::cleanupTestCase()
{
	LOG_LEVEL4("cleanupTestCase()");

}

void TestGInfoScreenMsgDataXML::gInfoScreenMsgDataUnMarshall()
{
	LOG_LEVEL4("gInfoScreenMsgDataUnMarshall()");
	
	QByteArray oXML("<?xml version=\"1.0\" encoding=\"utf-8\"?>" \
				   "<infoscreen_request xmlns=\"http://www.gilbarco.com/crind/display_manager/api\">" \
				   		"<interface_version>01.0.01</interface_version>" \
						"<data>303239003100303030003030300030303000303030003030300030300030300030303000</data>" \
				   "</infoscreen_request>");
	QByteArray oData("303239003100303030003030300030303000303030003030300030300030300030303000");
	
	GInfoScreenMsgData *oRequest = GInfoScreenMsgDataXML::UnMarshall(oXML);

	QVERIFY(oRequest != NULL);	
	QCOMPARE(oRequest->GetInterfaceVersion() , QString("01.0.01"));
	QVERIFY(oRequest->GetData() == oData);
}

QTEST_MAIN(TestGInfoScreenMsgDataXML)
