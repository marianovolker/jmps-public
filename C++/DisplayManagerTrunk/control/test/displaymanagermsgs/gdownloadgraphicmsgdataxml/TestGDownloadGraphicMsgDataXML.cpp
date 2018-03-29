#include "TestGDownloadGraphicMsgDataXML.h"

#include "control/src/displaymanagermsgs/xml/GDownloadGraphicMsgDataXML.h"

#include "utils/logger/src/GLogger.h"
#include "utils/logger/src/GLogConfig.h"
#include "utils/logger/src/GLogManager.h"

void TestGDownloadGraphicMsgDataXML::initTestCase()
{
  	GLogConfig* poConfig;
  	GLogger* poLogger;

  	// Logger StartUp
  	poConfig = new GLogConfig(GLogConfig::eBoth, GLogConfig::eLevel1, "./DisplayManager.log");
	poLogger = new GLogger(poConfig);
	GLogManager::AddLogger("DisplayManager.log", poLogger, false);

	LOG_LEVEL3("Starting Unit Test...");
}

void TestGDownloadGraphicMsgDataXML::cleanupTestCase()
{
	LOG_LEVEL4("cleanupTestCase()");

}

void TestGDownloadGraphicMsgDataXML::gDownloadGraphicMsgDataUnMarshall()
{
	LOG_LEVEL4("gDownloadGraphicMsgDataUnMarshall()");

	QByteArray oXML("<?xml version=\"1.0\" encoding=\"UTF-8\" standalone=\"yes\"?>"
					"<download_graphic_request xmlns=\"http://www.gilbarco.com/crind/display_manager/api\">"
						"<interface_version>01.0.01</interface_version>"
						"<type>image</type>"
						"<crc>12121212</crc>"
						"<file_name>/var/opt/crindbios/images/12121212.bmp</file_name>"
						"<segment>2</segment>"
						"<screen>1</screen>"
						"<display>1</display>"
					"</download_graphic_request>");
	
	GDownloadGraphicMsgData *oRequest = GDownloadGraphicMsgDataXML::UnMarshall(oXML);
	
	QVERIFY(oRequest != NULL);	
	
	QCOMPARE(oRequest->GetInterfaceVersion() , QString("01.0.01"));
	QCOMPARE(oRequest->GetType() , QString("image"));
	QCOMPARE(oRequest->GetCRC() , QString("12121212"));
	QCOMPARE(oRequest->GetFilename() , QString("/var/opt/crindbios/images/12121212.bmp"));
	QCOMPARE(oRequest->GetSegment() , QString("2"));	
	QCOMPARE(oRequest->GetScreen() , QString("1"));
	QCOMPARE(oRequest->GetDisplay() , QString("1"));
}

QTEST_MAIN(TestGDownloadGraphicMsgDataXML)
