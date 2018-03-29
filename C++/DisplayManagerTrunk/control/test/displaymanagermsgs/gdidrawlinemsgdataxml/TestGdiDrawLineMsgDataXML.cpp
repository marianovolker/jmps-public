#include "TestGdiDrawLineMsgDataXML.h"

#include "control/src/displaymanagermsgs/xml/GGdiDrawLineMsgDataXML.h"

#include "utils/logger/src/GLogger.h"
#include "utils/logger/src/GLogConfig.h"
#include "utils/logger/src/GLogManager.h"

void TestGdiDrawLineMsgDataXML::initTestCase()
{
  	GLogConfig* poConfig;
  	GLogger* poLogger;

  	// Logger StartUp
  	poConfig = new GLogConfig(GLogConfig::eBoth, GLogConfig::eLevel1, "./DisplayManager.log");
	poLogger = new GLogger(poConfig);
	GLogManager::AddLogger("DisplayManager.log", poLogger, false);

	LOG_LEVEL3("Starting Unit Test...");
}

void TestGdiDrawLineMsgDataXML::cleanupTestCase()
{
	LOG_LEVEL4("cleanupTestCase()");

}

void TestGdiDrawLineMsgDataXML::gdiDrawLineMsgDataUnMarshall()
{
	LOG_LEVEL4("gdiDrawLineMsgDataUnMarshall()");
	
	QByteArray oXML("<?xml version=\"1.0\" encoding=\"utf-8\"?>" \
					"<gdi_drawline_request xmlns=\"http://www.gilbarco.com/crind/displayManager/api\">" \
						"<interface_version>01.0.01</interface_version>" \
						"<x_loc>123</x_loc>" \
						"<y_loc>456</y_loc>" \
						"<length>78</length>" \
						"<thickness>0.2</thickness>" \
						"<color>4312</color>" \
					"</gdi_drawline_request>");

	GGdiDrawLineMsgData *oRequest = GGdiDrawLineMsgDataXML::UnMarshall(oXML);
	
	QVERIFY(oRequest != NULL);	
	QCOMPARE(oRequest->GetInterfaceVersion() , QString("01.0.01"));
	QCOMPARE(oRequest->GetThickness() , QString("0.2"));
	QCOMPARE(oRequest->GetColor() , QString("4312"));
	QCOMPARE(oRequest->GetLength() , 78);
	QVERIFY(oRequest->GetXLoc() == 123);
	QVERIFY(oRequest->GetYLoc() == 456);
}

QTEST_MAIN(TestGdiDrawLineMsgDataXML)
