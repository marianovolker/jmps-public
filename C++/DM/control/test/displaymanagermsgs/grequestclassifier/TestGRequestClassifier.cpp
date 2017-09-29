#include "TestGRequestClassifier.h"

#include "control/src/displaymanagermsgs/xml/GRequestClassifier.h"

#include "utils/logger/src/GLogger.h"
#include "utils/logger/src/GLogConfig.h"
#include "utils/logger/src/GLogManager.h"

void TestGRequestClassifier::initTestCase()
{
  	GLogConfig* poConfig;
  	GLogger* poLogger;

  	// Logger StartUp
  	poConfig = new GLogConfig(GLogConfig::eBoth, GLogConfig::eLevel1, "./DisplayManager.log");
	poLogger = new GLogger(poConfig);
	GLogManager::AddLogger("DisplayManager.log", poLogger, false);

	LOG_LEVEL3("Starting Unit Test...");
}

void TestGRequestClassifier::cleanupTestCase()
{
	LOG_LEVEL4("cleanupTestCase()");

}

void TestGRequestClassifier::testNotFoundClassifier()
{
	LOG_LEVEL4(".");

	QByteArray oKioskId("<?xml version=\"1.0\" encoding=\"utf-8\"?>" \
						"<will_not_be_found xmlns=\"http://www.gilbarco.com/crind/displayManager/api\">" \
							"<interface_version>01.0.01</interface_version>" \
						"</will_not_be_found>");
	GRequestClassifier* poClassifier = GRequestClassifier::GetInstance();
	
	QVERIFY( poClassifier->GetRequestType(oKioskId) == GRequestClassifier::eNotFound);
}

void TestGRequestClassifier::testClearScreenClasifier()
{
	LOG_LEVEL4(".");

	QByteArray oXML("<?xml version=\"1.0\" encoding=\"utf-8\"?>" \
					"<clear_screen_request xmlns=\"http://www.gilbarco.com/crind/displayManager/api\">" \
						"<interface_version>01.0.01</interface_version>" \
						"<color>1234</color>" \
					"</clear_screen_request>");
	
	GRequestClassifier* poClassifier = GRequestClassifier::GetInstance();
	
	QVERIFY( poClassifier->GetRequestType(oXML) == GRequestClassifier::eClearScreenRequest);	
}

void TestGRequestClassifier::testGdiClearScreen()
{
	LOG_LEVEL4(".");

	QByteArray oXML("<?xml version=\"1.0\" encoding=\"utf-8\"?>" \
					"<gdi_clearscreen_request xmlns=\"http://www.gilbarco.com/crind/displayManager/api\">" \
						"<interface_version>01.0.01</interface_version>" \
					"</gdi_clearscreen_request>");
	
	GRequestClassifier* poClassifier = GRequestClassifier::GetInstance();
	
	QVERIFY( poClassifier->GetRequestType(oXML) == GRequestClassifier::eGdiClearScreenRequest);	
}

void TestGRequestClassifier::testGdiDrawLineClassifier()
{
	LOG_LEVEL4(".");

	QByteArray oXML("<?xml version=\"1.0\" encoding=\"utf-8\"?>" \
					"<gdi_drawline_request xmlns=\"http://www.gilbarco.com/crind/displayManager/api\">" \
						"<interface_version>01.0.01</interface_version>" \
						"<x_loc>123</x_loc>" \
						"<y_loc>456</y_loc>" \
						"<length>78.9</length>" \
						"<thickness>0.2</thickness>" \
						"<color>4312</color>" \
					"</gdi_drawline_request>");
	
	GRequestClassifier* poClassifier = GRequestClassifier::GetInstance();
	
	QVERIFY( poClassifier->GetRequestType(oXML) == GRequestClassifier::eGdiDrawLineRequest);	
}

void TestGRequestClassifier::testGdiDrawTextClassifier()
{
	LOG_LEVEL4(".");

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
	
	GRequestClassifier* poClassifier = GRequestClassifier::GetInstance();
	
	QVERIFY( poClassifier->GetRequestType(oXML) == GRequestClassifier::eGdiDrawTextRequest);	
}

void TestGRequestClassifier::testGdiFillRectClassifier()
{
	LOG_LEVEL4(".");

	QByteArray oXML("<?xml version=\"1.0\" encoding=\"utf-8\"?>" \
					"<gdi_fillrect_request xmlns=\"http://www.gilbarco.com/crind/displayManager/api\">" \
						"<interface_version>01.0.01</interface_version>" \
						"<x_loc>123</x_loc>" \
						"<y_loc>456</y_loc>" \
						"<width>4445</width>" \
						"<height>667</height>" \
						"<color>123</color>" \
					"</gdi_fillrect_request>");
	
	GRequestClassifier* poClassifier = GRequestClassifier::GetInstance();
	
	QVERIFY( poClassifier->GetRequestType(oXML) == GRequestClassifier::eGdiFillRectRequest);	
}

void TestGRequestClassifier::testGdiGetScreenSizeClassifier()
{
	LOG_LEVEL4(".");

	QByteArray oXML("<?xml version=\"1.0\" encoding=\"utf-8\"?>" \
					"<gdi_getscreensize_request xmlns=\"http://www.gilbarco.com/crind/displayManager/api\">" \
						"<interface_version>01.0.01</interface_version>" \
					"</gdi_getscreensize_request>");
	
	GRequestClassifier* poClassifier = GRequestClassifier::GetInstance();
	
	QVERIFY( poClassifier->GetRequestType(oXML) == GRequestClassifier::eGdiGetScreenSizeRequest);	
}

void TestGRequestClassifier::testGdiRefreshScreenClassifier()
{
	LOG_LEVEL4(".");

	QByteArray oXML("<?xml version=\"1.0\" encoding=\"utf-8\"?>" \
					"<gdi_refreshscreen_request xmlns=\"http://www.gilbarco.com/crind/displayManager/api\">" \
						"<interface_version>01.0.01</interface_version>" \
					"</gdi_refreshscreen_request>");
	
	GRequestClassifier* poClassifier = GRequestClassifier::GetInstance();
	
	QVERIFY( poClassifier->GetRequestType(oXML) == GRequestClassifier::eGdiRefreshScreenRequest);	
}

void TestGRequestClassifier::testDownloadGraphicRequestClassifier()
{
	LOG_LEVEL4(".");

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
	
	GRequestClassifier* poClassifier = GRequestClassifier::GetInstance();
	
	QVERIFY( poClassifier->GetRequestType(oXML) == GRequestClassifier::eDownloadGraphicRequest);	
}

void TestGRequestClassifier::testFlyTextClassifier()
{
	LOG_LEVEL4(".");

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
	
	GRequestClassifier* poClassifier = GRequestClassifier::GetInstance();
	
	QVERIFY( poClassifier->GetRequestType(oXML) == GRequestClassifier::eFlyTextRequest);	
}

void TestGRequestClassifier::testInfoScreenRequestClassifier()
{
	LOG_LEVEL4(".");

	QByteArray oXML("<?xml version=\"1.0\" encoding=\"utf-8\"?>" \
				   "<infoscreen_request xmlns=\"http://www.gilbarco.com/crind/display_manager/api\">" \
				   		"<interface_version>01.0.01</interface_version>" \
						"<data>303239003100303030003030300030303000303030003030300030300030300030303000</data>" \
				   "</infoscreen_request>");
	
	GRequestClassifier* poClassifier = GRequestClassifier::GetInstance();
	
	QVERIFY( poClassifier->GetRequestType(oXML) == GRequestClassifier::eInfoScreenRequest);	
}

void TestGRequestClassifier::testKeyEventClassifier()
{
	LOG_LEVEL4(".");

	QByteArray oXML("<?xml version=\"1.0\" encoding=\"utf-8\"?>" \
				   "<key_event xmlns=\"http://www.gilbarco.com/crind/displayManager/api\">" \
					   "<interface_version>01.0.01</interface_version>" \
				   	   "<key_code>3</key_code>" \
				   "</key_event>");
	
	GRequestClassifier* poClassifier = GRequestClassifier::GetInstance();
	
	QVERIFY( poClassifier->GetRequestType(oXML) == GRequestClassifier::eKeyEventRequest);	
}

void TestGRequestClassifier::testKioskIdClassifier()
{
	LOG_LEVEL4(".");

	QByteArray oXML("<kiosk_id_request xmlns=\"http://www.gilbarco.com/crind/displayManager/api\">" \
						"<interface_version>01.0.01</interface_version>" \
					"</kiosk_id_request>");
	
	GRequestClassifier* poClassifier = GRequestClassifier::GetInstance();
	
	QVERIFY( poClassifier->GetRequestType(oXML) == GRequestClassifier::eKioskIdRequest);	
}

void TestGRequestClassifier::testResponseClassifier()
{
	LOG_LEVEL4(".");

	QByteArray oXML("<?xml version=\"1.0\" encoding=\"utf-8\"?>" \
				   "<response xmlns=\"http://www.gilbarco.com/crind/displayManager/api\">" \
						"<interface_version>01.0.01</interface_version>" \
						"<command>kiosk_id_request</command>" \
						"<response_code>ok</response_code>" \
				   "</response>");
	
	GRequestClassifier* poClassifier = GRequestClassifier::GetInstance();
	
	QVERIFY( poClassifier->GetRequestType(oXML) == GRequestClassifier::eResponse);	
}

void TestGRequestClassifier::testSetPersonalityRequestClassifier()
{
	LOG_LEVEL4(".");

	QByteArray oXML("<?xml version=\"1.0\" encoding=\"utf-8\"?>" \
				   "<set_personality_request xmlns=\"http://www.gilbarco.com/crind/displayManager/api\">" \
						"<interface_version>01.0.01</interface_version>" \
						"<personality>shell</personality>" \
				   "</set_personality_request>");
	
	GRequestClassifier* poClassifier = GRequestClassifier::GetInstance();
	
	QVERIFY( poClassifier->GetRequestType(oXML) == GRequestClassifier::eSetPersonalityRequest);	
}

void TestGRequestClassifier::testURLRequestClassifier()
{
	LOG_LEVEL4(".");

	QByteArray oXML("<?xml version=\"1.0\" encoding=\"utf-8\"?>" \
				   "<url_request xmlns=\"http://www.gilbarco.com/crind/display_manager/api\">" \
				   	   	"<interface_version>01.0.01</interface_version>" \
						"<url>http://127.0.0.1</url>" \
				   "</url_request>");
	
	GRequestClassifier* poClassifier = GRequestClassifier::GetInstance();
	
	QVERIFY( poClassifier->GetRequestType(oXML) == GRequestClassifier::eURLRequest);	
}

QTEST_MAIN(TestGRequestClassifier)



