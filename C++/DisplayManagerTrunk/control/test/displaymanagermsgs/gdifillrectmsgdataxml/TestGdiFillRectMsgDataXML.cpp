#include "TestGdiFillRectMsgDataXML.h"

#include "control/src/displaymanagermsgs/xml/GGdiFillRectMsgDataXML.h"

#include "utils/logger/src/GLogger.h"
#include "utils/logger/src/GLogConfig.h"
#include "utils/logger/src/GLogManager.h"

void TestGdiFillRectMsgDataXML::initTestCase()
{
  	GLogConfig* poConfig;
  	GLogger* poLogger;

  	// Logger StartUp
  	poConfig = new GLogConfig(GLogConfig::eBoth, GLogConfig::eLevel1, "./DisplayManager.log");
	poLogger = new GLogger(poConfig);
	GLogManager::AddLogger("DisplayManager.log", poLogger, false);

	LOG_LEVEL3("Starting Unit Test...");
}

void TestGdiFillRectMsgDataXML::cleanupTestCase()
{
	LOG_LEVEL4("cleanupTestCase()");

}

void TestGdiFillRectMsgDataXML::gdiFillRectMsgDataUnMarshall()
{
	LOG_LEVEL4("gdiFillRectMsgDataUnMarshall()");

	QByteArray oXML("<?xml version=\"1.0\" encoding=\"utf-8\"?>" \
					"<gdi_fillrect_request xmlns=\"http://www.gilbarco.com/crind/displayManager/api\">" \
						"<interface_version>01.0.01</interface_version>" \
						"<x_loc>123</x_loc>" \
						"<y_loc>456</y_loc>" \
						"<width>4445</width>" \
						"<height>667</height>" \
						"<color>123</color>" \
					"</gdi_fillrect_request>");

	GGdiFillRectMsgData *oRequest = GGdiFillRectMsgDataXML::UnMarshall(oXML);
	
	QVERIFY(oRequest != NULL);	
	QCOMPARE(oRequest->GetInterfaceVersion() , QString("01.0.01"));
	QVERIFY(oRequest->GetXLoc() == 123);
	QVERIFY(oRequest->GetYLoc() == 456);
	QVERIFY(oRequest->GetHeight() == 667);
	QVERIFY(oRequest->GetWidth() == 4445);	
	QCOMPARE(oRequest->GetColor() , QString("123"));
}

QTEST_MAIN(TestGdiFillRectMsgDataXML)
