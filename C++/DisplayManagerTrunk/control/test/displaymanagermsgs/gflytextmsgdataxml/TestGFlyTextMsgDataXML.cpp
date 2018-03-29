#include "TestGFlyTextMsgDataXML.h"

#include "control/src/displaymanagermsgs/xml/GFlyTextMsgDataXML.h"

#include "utils/logger/src/GLogger.h"
#include "utils/logger/src/GLogConfig.h"
#include "utils/logger/src/GLogManager.h"

void TestGFlyTextMsgDataXML::initTestCase()
{
  	GLogConfig* poConfig;
  	GLogger* poLogger;

  	// Logger StartUp
  	poConfig = new GLogConfig(GLogConfig::eBoth, GLogConfig::eLevel1, "./DisplayManager.log");
	poLogger = new GLogger(poConfig);
	GLogManager::AddLogger("DisplayManager.log", poLogger, false);

	LOG_LEVEL3("Starting Unit Test...");
}

void TestGFlyTextMsgDataXML::cleanupTestCase()
{
	LOG_LEVEL4("cleanupTestCase()");

}

void TestGFlyTextMsgDataXML::gFlyTextMsgDataUnMarshall()
{
	LOG_LEVEL4("gFlyTextMsgDataUnMarshall()");
	
	QByteArray oXML("<?xml version=\"1.0\" encoding=\"utf-8\"?>" \
					"<flytext_request xmlns=\"http://www.gilbarco.com/crind/displayManager/api\">" \
						"<interface_version>01.0.01</interface_version>" \
						"<text>Insert Card</text>" \
						"<x_loc>51</x_loc>" \
						"<y_loc>50</y_loc>" \
						"<attribute>\?\?</attribute>" \
						"<font></font>" \
						"<row><cell x=\"10\" justify=\"right\">first text</cell><cell>second text</cell></row>" \
					"</flytext_request>");	

	GFlyTextMsgData *oRequest = GFlyTextMsgDataXML::UnMarshall(oXML);
	
	QVERIFY(oRequest != NULL);	
	QCOMPARE(oRequest->GetInterfaceVersion() , QString("01.0.01"));
	QCOMPARE(oRequest->GetFont() , QString(""));
	QCOMPARE(oRequest->GetText() , QString("Insert Card"));
	QVERIFY(oRequest->GetXLoc() == 51);
	QVERIFY(oRequest->GetYLoc() == 50);
}

QTEST_MAIN(TestGFlyTextMsgDataXML)
