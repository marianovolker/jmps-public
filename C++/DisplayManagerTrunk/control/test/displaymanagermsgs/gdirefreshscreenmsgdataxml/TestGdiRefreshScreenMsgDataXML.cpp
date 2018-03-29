#include "TestGdiRefreshScreenMsgDataXML.h"

#include "control/src/displaymanagermsgs/xml/GGdiRefreshScreenMsgDataXML.h"

#include "utils/logger/src/GLogger.h"
#include "utils/logger/src/GLogConfig.h"
#include "utils/logger/src/GLogManager.h"

void TestGdiRefreshScreenMsgDataXML::initTestCase()
{
  	GLogConfig* poConfig;
  	GLogger* poLogger;

  	// Logger StartUp
  	poConfig = new GLogConfig(GLogConfig::eBoth, GLogConfig::eLevel1, "./DisplayManager.log");
	poLogger = new GLogger(poConfig);
	GLogManager::AddLogger("DisplayManager.log", poLogger, false);

	LOG_LEVEL3("Starting Unit Test...");
}

void TestGdiRefreshScreenMsgDataXML::cleanupTestCase()
{
	LOG_LEVEL4("cleanupTestCase()");

}

void TestGdiRefreshScreenMsgDataXML::gdiRefreshScreenMsgDataUnMarshall()
{
	LOG_LEVEL4("gdiRefreshScreenMsgDataUnMarshall()");

	QByteArray oXML("<?xml version=\"1.0\" encoding=\"utf-8\"?>" \
					"<gdi_refreshscreen_request xmlns=\"http://www.gilbarco.com/crind/displayManager/api\">" \
						"<interface_version>01.0.01</interface_version>" \
					"</gdi_refreshscreen_request>");

	GGdiRefreshScreenMsgData *oRequest = GGdiRefreshScreenMsgDataXML::UnMarshall(oXML);
	
	QVERIFY(oRequest != NULL);	
	QCOMPARE(oRequest->GetInterfaceVersion() , QString("01.0.01"));
}

QTEST_MAIN(TestGdiRefreshScreenMsgDataXML)
