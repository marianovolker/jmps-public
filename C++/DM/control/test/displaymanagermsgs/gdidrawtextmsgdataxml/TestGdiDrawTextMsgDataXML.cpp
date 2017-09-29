#include "TestGdiDrawTextMsgDataXML.h"

#include "control/src/displaymanagermsgs/xml/GGdiDrawTextMsgDataXML.h"

#include "utils/logger/src/GLogger.h"
#include "utils/logger/src/GLogConfig.h"
#include "utils/logger/src/GLogManager.h"

void TestGdiDrawTextMsgDataXML::initTestCase()
{
  	GLogConfig* poConfig;
  	GLogger* poLogger;

  	// Logger StartUp
  	poConfig = new GLogConfig(GLogConfig::eBoth, GLogConfig::eLevel1, "./DisplayManager.log");
	poLogger = new GLogger(poConfig);
	GLogManager::AddLogger("DisplayManager.log", poLogger, false);

	LOG_LEVEL3("Starting Unit Test...");
}

void TestGdiDrawTextMsgDataXML::cleanupTestCase()
{
	LOG_LEVEL4("cleanupTestCase()");

}

void TestGdiDrawTextMsgDataXML::gdiDrawTextMsgDataUnMarshall()
{
	LOG_LEVEL4("gdiDrawTextMsgDataUnMarshall()");

	QByteArray oXML("<?xml version=\"1.0\" encoding=\"utf-8\"?>" \
					"<gdi_drawtext_request xmlns=\"http://www.gilbarco.com/crind/displayManager/api\">" \
						"<interface_version>01.0.01</interface_version>" \
						"<text>Some Text</text>" \
						"<x_loc>123</x_loc>" \
						"<y_loc>456</y_loc>" \
						"<font>A Font</font>" \
						"<fgcolor>123</fgcolor>" \
						"<bgcolor>921</bgcolor>" \
						"<attr>attribute</attr>" \
					"</gdi_drawtext_request>");

	GGdiDrawTextMsgData *oRequest = GGdiDrawTextMsgDataXML::UnMarshall(oXML);
	
	QVERIFY(oRequest != NULL);	
	QCOMPARE(oRequest->GetInterfaceVersion() , QString("01.0.01"));
	QCOMPARE(oRequest->GetText() , QString("Some Text"));
	QVERIFY(oRequest->GetXLoc() == 123);
	QVERIFY(oRequest->GetYLoc() == 456);	
	QCOMPARE(oRequest->GetFont() , QString("A Font"));
	QCOMPARE(oRequest->GetBGColor() , QString("921"));
	QCOMPARE(oRequest->GetFGColor() , QString("123"));
	QCOMPARE(oRequest->GetAttr() , QString("attribute"));
}

QTEST_MAIN(TestGdiDrawTextMsgDataXML)
