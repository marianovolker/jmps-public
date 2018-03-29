#include "TestControlChannel.h"

#include <QFile>

#include "../../utils/logger/src/GLogger.h"
#include "../../utils/logger/src/GLogConfig.h"
#include "../../utils/logger/src/GLogManager.h"

#include <stdlib.h>

TestControlChannel::TestControlChannel():
	m_poControlChMgr(NULL),
	m_CstrOkResponse("<response_code>ok</response_code>"),
	m_CstrResponseCmdTag("<command>%1</command>"),
	m_CiTestPort(3601),
	m_CiResponseWaitTimeOut(5000)
{
	unsetenv("DM_CONFIG_PATH");
}

TestControlChannel::~TestControlChannel()
{
	delete m_poControlChMgr;
}

void TestControlChannel::CheckIfResponseCommandOk(const QByteArray &CoRequestMsg, const QString &CstrCmdName)
{
	QByteArray oResponse;

	oSock.write(CoRequestMsg);

	QVERIFY(oSock.waitForReadyRead(m_CiResponseWaitTimeOut));

	oResponse = oSock.readAll();

	if( oResponse.indexOf(m_CstrOkResponse) == -1 )
	{
		qDebug() << "Response = [" << oResponse << "]";
	}

	QVERIFY(oResponse.indexOf(m_CstrOkResponse)!=-1);

	QVERIFY(oResponse.indexOf(m_CstrResponseCmdTag.arg(CstrCmdName))!=-1);
}

void TestControlChannel::CheckIfResponseCommandNotOk(const QByteArray &CoRequestMsg, const QString &CstrCmdName)
{
	QByteArray oResponse;

	oSock.write(CoRequestMsg);

	QVERIFY(oSock.waitForReadyRead(m_CiResponseWaitTimeOut));

	oResponse = oSock.readAll();

	QVERIFY(oResponse.indexOf(m_CstrOkResponse)==-1);

	QVERIFY(oResponse.indexOf(m_CstrResponseCmdTag.arg(CstrCmdName))!=-1);
}

void TestControlChannel::initTestCase()
{
  	// Logger StartUp
	GLogManager::AddLogger("TestControlChannel.log", 
							new GLogger(new GLogConfig(GLogConfig::eFile, GLogConfig::eLevel4, "./TestControlChannel.log") ),
							false);
	
	m_poControlChMgr = new GControlChannelMgr(m_CiTestPort);
}

void TestControlChannel::cleanupTestCase()
{			
	m_poControlChMgr->Stop();
	m_poControlChMgr->wait();

	delete m_poControlChMgr;
	m_poControlChMgr = NULL;

	if( QFile::exists("TestControlChannel.log") )
	{
		QFile::remove("TestControlChannel.log");
	}		
}

void TestControlChannel::TestStartControlChannelManager()
{	
	m_poControlChMgr->start();
	QVERIFY(m_poControlChMgr->isRunning());
	m_poControlChMgr->WaitUntilIsReady();
	QVERIFY(m_poControlChMgr->GetNumberOfConnections() == 0);
}

void TestControlChannel::WaitForNConnections(int iNumberOfConnections, int iTimeToWaitInSeconds)
{
	QTime oTime(QTime::currentTime());

	while( (m_poControlChMgr->GetNumberOfConnections() != iNumberOfConnections) &&
		   ((QTime::currentTime().second()-oTime.second()) <= iTimeToWaitInSeconds) )
	{
		QTest::qWait(800);
	}
}

void TestControlChannel::TestConnection()
{
	const int ikBoundaryTimeToWaitInSeconds(20);
	const int ikOneConnection(1);
	const int ikTwoConnections(2);
	const QString sLocalAddrress("127.0.0.1");

	oSock.connectToHost(sLocalAddrress, m_CiTestPort);
	QVERIFY(oSock.waitForConnected(-1));

	WaitForNConnections(ikOneConnection, ikBoundaryTimeToWaitInSeconds);
	QVERIFY(m_poControlChMgr->GetNumberOfConnections() == ikOneConnection);

	oSock2.connectToHost(sLocalAddrress, m_CiTestPort);
	QVERIFY(oSock2.waitForConnected(-1));
	
	WaitForNConnections(ikTwoConnections, ikBoundaryTimeToWaitInSeconds);
	LOG_LEVEL1(QString("Connections: ") + QString::number(m_poControlChMgr->GetNumberOfConnections()));
	QVERIFY(m_poControlChMgr->GetNumberOfConnections() == ikTwoConnections);
		
	oSock2.disconnectFromHost();
	QVERIFY(oSock2.state() == QTcpSocket::UnconnectedState);

	WaitForNConnections(ikOneConnection, ikBoundaryTimeToWaitInSeconds);
	QVERIFY(m_poControlChMgr->GetNumberOfConnections() == ikOneConnection);
	
	// One socket will remain connected	
}

// -----------------------------------------------------------------------------------------------------------------------

//! Test sending a single message in two parts
void TestControlChannel::TestTwoPartPacket()
{

	QByteArray oClearScreenPart1("<?xml version=\"1.0\" encoding=\"utf-8\"?>" \
								 "<clear_screen_request xmlns=\"http://www.gilbarco.com/crind/displayManager/api\">");
	QByteArray oClearScreenPart2(	"<interface_version>01.0.01</interface_version>" \
									"<color>1234</color>" \
								"</clear_screen_request>");

	QByteArray oResponse;
	
	oSock.write(oClearScreenPart1);

	oSock.waitForBytesWritten();

	oSock.write(oClearScreenPart2);

	QVERIFY(oSock.waitForReadyRead(m_CiResponseWaitTimeOut));

	oResponse = oSock.readAll();

	QVERIFY(oResponse.indexOf(m_CstrOkResponse)!=-1);
}

//! Test sending two messages at once, and receiving the splitted versions
void TestControlChannel::TestTwoMessages()
{
	QByteArray oClearScreen("<?xml version=\"1.0\" encoding=\"utf-8\"?>" \
							 "<clear_screen_request xmlns=\"http://www.gilbarco.com/crind/displayManager/api\">" \
								"<interface_version>01.0.01</interface_version>" \
								"<color>1234</color>" \
							"</clear_screen_request>");
	
	QByteArray oGdiGetScreensize("<?xml version=\"1.0\" encoding=\"utf-8\"?>" \
									"<gdi_getscreensize_request xmlns=\"http://www.gilbarco.com/crind/displayManager/api\">" \
										"<interface_version>01.0.01</interface_version>" \
									"</gdi_getscreensize_request>");

	QByteArray twoMessages;
	QByteArray oResponse;
	
	twoMessages.append(oClearScreen);
	twoMessages.append(oGdiGetScreensize);
	
	oSock.write(twoMessages);
	
	QVERIFY(oSock.waitForReadyRead(m_CiResponseWaitTimeOut));

	oResponse = oSock.readAll();

	QVERIFY(oResponse.count(m_CstrOkResponse.toAscii())==2);
}

//! Test sending two messages split in two parts, having the firts part incomplete
void TestControlChannel::TestTwoMessagesTwoPackets()
{
	QByteArray oPart1("<?xml version=\"1.0\" encoding=\"utf-8\"?>" \
					  "<clear_screen_request xmlns=\"http://www.gilbarco.com/crind/displayManager/api\">" \
							"<interface_version>01.0.01</interface_version>" \
					  "<color>1234</color>");
						
	QByteArray oPart2("</clear_screen_request>" \
					  "<?xml version=\"1.0\" encoding=\"utf-8\"?>" \
					  "<gdi_getscreensize_request xmlns=\"http://www.gilbarco.com/crind/displayManager/api\">" \
					  		"<interface_version>01.0.01</interface_version>" \
					  "</gdi_getscreensize_request>");
	
	QByteArray oClearScreen("<?xml version=\"1.0\" encoding=\"utf-8\"?>" \
							"<clear_screen_request xmlns=\"http://www.gilbarco.com/crind/displayManager/api\">" \
								"<interface_version>01.0.01</interface_version>" \
								"<color>1234</color>" \
							"</clear_screen_request>");
	
	QByteArray oGdiGetScreensize("<?xml version=\"1.0\" encoding=\"utf-8\"?>" \
								 "<gdi_getscreensize_request xmlns=\"http://www.gilbarco.com/crind/displayManager/api\">" \
									"<interface_version>01.0.01</interface_version>" \
								 "</gdi_getscreensize_request>");	

	QByteArray oResponse;
	
	oSock.write(oPart1);
	oSock.waitForBytesWritten();

	oSock.write(oPart2);
	oSock.waitForBytesWritten();
	
	QVERIFY(oSock.waitForReadyRead(m_CiResponseWaitTimeOut));

	oResponse = oSock.readAll();

	QVERIFY(oResponse.count(m_CstrOkResponse.toAscii())==2);
}

// -----------------------------------------------------------------------------------------------------------------------
void TestControlChannel::TestSend_ClearScreen_Request()
{
	const QByteArray CoClearScreen("<?xml version=\"1.0\" encoding=\"utf-8\"?>" \
							"<clear_screen_request xmlns=\"http://www.gilbarco.com/crind/displayManager/api\">" \
								"<interface_version>01.0.01</interface_version>" \
								"<color>1234</color>" \
							"</clear_screen_request>");


	const QString CstrCmdName = "clear_screen_request";


	CheckIfResponseCommandOk(CoClearScreen, CstrCmdName);

}

void TestControlChannel::TestSend_GdiClearScreen_Request()
{
	const QByteArray CoGdiClearScreen("<?xml version=\"1.0\" encoding=\"utf-8\"?>" \
								"<gdi_clearscreen_request xmlns=\"http://www.gilbarco.com/crind/displayManager/api\">" \
									"<interface_version>01.0.01</interface_version>" \
								"</gdi_clearscreen_request>");


	const QString CstrCmdName = "gdi_clearscreen_request";


	CheckIfResponseCommandOk(CoGdiClearScreen, CstrCmdName);
}

void TestControlChannel::TestSend_GdiDrawline_Request()
{
	const QByteArray CoGdiDrawline("<?xml version=\"1.0\" encoding=\"utf-8\"?>" \
							"<gdi_drawline_request xmlns=\"http://www.gilbarco.com/crind/displayManager/api\">" \
								"<interface_version>01.0.01</interface_version>" \
								"<x_loc>123</x_loc>" \
								"<y_loc>456</y_loc>" \
								"<length>78</length>" \
								"<thickness>0.2</thickness>" \
								"<color>4312</color>" \
							"</gdi_drawline_request>");

	const QString CstrCmdName = "gdi_drawline_request";


	CheckIfResponseCommandOk(CoGdiDrawline, CstrCmdName);
}

void TestControlChannel::TestSend_GdiDrawtext_Request()
{
	const QByteArray CoGdiDrawtext("<?xml version=\"1.0\" encoding=\"utf-8\"?>" \
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

	const QString CstrCmdName = "gdi_drawtext_request";


	CheckIfResponseCommandOk(CoGdiDrawtext, CstrCmdName);
}

void TestControlChannel::TestSend_GdiFillrect_Request()
{
	const QByteArray CoGdiFillrect("<?xml version=\"1.0\" encoding=\"utf-8\"?>" \
							"<gdi_fillrect_request xmlns=\"http://www.gilbarco.com/crind/displayManager/api\">" \
								"<interface_version>01.0.01</interface_version>" \
								"<x_loc>123</x_loc>" \
								"<y_loc>456</y_loc>" \
								"<width>4445</width>" \
								"<height>667</height>" \
								"<color>123</color>" \
							"</gdi_fillrect_request>");

	const QString CstrCmdName = "gdi_fillrect_request";

	CheckIfResponseCommandOk(CoGdiFillrect, CstrCmdName);
}

void TestControlChannel::TestSend_GdiGetScreensize_Request()
{
	const QByteArray CoGdiGetScreensize("<?xml version=\"1.0\" encoding=\"utf-8\"?>" \
									"<gdi_getscreensize_request xmlns=\"http://www.gilbarco.com/crind/displayManager/api\">" \
										"<interface_version>01.0.01</interface_version>" \
									"</gdi_getscreensize_request>");

	const QString CstrCmdName = "gdi_getscreensize_request";

	CheckIfResponseCommandOk(CoGdiGetScreensize, CstrCmdName);
}

void TestControlChannel::TestSend_GdiRefreshScreen_Request()
{
	const QByteArray CoGdiRefreshScreen("<?xml version=\"1.0\" encoding=\"utf-8\"?>" \
								"<gdi_refreshscreen_request xmlns=\"http://www.gilbarco.com/crind/displayManager/api\">" \
									"<interface_version>01.0.01</interface_version>" \
								"</gdi_refreshscreen_request>");


	const QString CstrCmdName = "gdi_refreshscreen_request";

	CheckIfResponseCommandOk(CoGdiRefreshScreen, CstrCmdName);
}

void TestControlChannel::TestSend_DownloadGraphic_Request()
{
	const QByteArray CoDownloadGraphic("<?xml version=\"1.0\" encoding=\"UTF-8\" standalone=\"yes\"?>"
								"<download_graphic_request xmlns=\"http://www.gilbarco.com/crind/display_manager/api\">"
									"<interface_version>01.0.01</interface_version>"
									"<type>image</type>"
									"<crc>12121212</crc>"
									"<file_name>/var/opt/crindbios/images/12121212.bmp</file_name>"
									"<segment>2</segment>"
									"<screen>1</screen>"
									"<display>1</display>"
								"</download_graphic_request>");

	const QString CstrCmdName = "download_graphic_request";

	CheckIfResponseCommandOk(CoDownloadGraphic, CstrCmdName);
}

void TestControlChannel::TestSend_Flytext_Request()
{
	const QByteArray CoFlytext("<?xml version=\"1.0\" encoding=\"utf-8\"?>" \
						"<flytext_request xmlns=\"http://www.gilbarco.com/crind/displayManager/api\">" \
							"<interface_version>01.0.01</interface_version>" \
							"<text>Insert Card</text>" \
							"<x_loc>51</x_loc>" \
							"<y_loc>10</y_loc>" \
							"<attribute>\?\?</attribute>" \
							"<font></font>" \
							"<row><cell x=\"10\" justify=\"right\">first text</cell><cell>second text</cell></row>" \
						"</flytext_request>");

	const QString CstrCmdName = "flytext_request";

	CheckIfResponseCommandOk(CoFlytext, CstrCmdName);
}


void TestControlChannel::TestSend_Infoscreen_Request()
{

	const QByteArray CoInfoscreen("<?xml version=\"1.0\" encoding=\"utf-8\"?>"\
			"<infoscreen_request xmlns=\"http://www.gilbarco.com/crind/display_manager/api\">"\
			"<interface_version>01.0.01</interface_version>"\
			"<data>303331003100303030003030300030303000303030003030300030300030300030303000</data>"\
			"</infoscreen_request>");


	const QString CstrCmdName = "infoscreen_request";

	CheckIfResponseCommandOk(CoInfoscreen, CstrCmdName);
}

void TestControlChannel::TestSend_KeyEvent_Request()
{
	const QByteArray CoKeyEvent("<?xml version=\"1.0\" encoding=\"utf-8\"?>" \
						   "<key_event xmlns=\"http://www.gilbarco.com/crind/displayManager/api\">" \
							   "<interface_version>01.0.01</interface_version>" \
						   	   "<key_code>3</key_code>" \
						   "</key_event>");

	const QString CstrCmdName = "key_event";

	CheckIfResponseCommandOk(CoKeyEvent, CstrCmdName);
}

void TestControlChannel::TestSend_KioskId_Request()
{
	const QByteArray CoKioskId("<?xml version=\"1.0\" encoding=\"utf-8\"?>" \
						"<kiosk_id_request xmlns=\"http://www.gilbarco.com/crind/displayManager/api\">" \
							"<interface_version>01.0.01</interface_version>" \
						"</kiosk_id_request>");

	const QString CstrCmdName = "kiosk_id_request";

	CheckIfResponseCommandOk(CoKioskId, CstrCmdName);

}

void TestControlChannel::TestSend_GetPersonalities_Request()
{
	const QByteArray CoSetPersonality("<?xml version=\"1.0\" encoding=\"utf-8\"?>" \
							   "<get_personalities_request xmlns=\"http://www.gilbarco.com/crind/displayManager/api\">" \
									"<interface_version>01.0.01</interface_version>" \
							   "</get_personalities_request>");

	const QString CstrCmdName = "get_personalities_request";

	CheckIfResponseCommandOk(CoSetPersonality, CstrCmdName);
}

void TestControlChannel::TestSend_SetPersonality_Request()
{
	const QByteArray CoSetPersonality("<?xml version=\"1.0\" encoding=\"utf-8\"?>" \
							   "<set_personality_request xmlns=\"http://www.gilbarco.com/crind/displayManager/api\">" \
									"<interface_version>01.0.01</interface_version>" \
									"<personality>blue</personality>" \
							   "</set_personality_request>");

	const QString CstrCmdName = "set_personality_request";

	CheckIfResponseCommandOk(CoSetPersonality, CstrCmdName);
}

void TestControlChannel::TestSend_SetDefaultPersonality_Request()
{
	const QByteArray CoSetPersonality("<?xml version=\"1.0\" encoding=\"utf-8\"?>" \
							   "<set_default_personality_request xmlns=\"http://www.gilbarco.com/crind/displayManager/api\">" \
									"<interface_version>01.0.01</interface_version>" \
									"<personality>citgo</personality>" \
							   "</set_default_personality_request>");

	const QString CstrCmdName = "set_default_personality_request";

	CheckIfResponseCommandNotOk(CoSetPersonality, CstrCmdName);
}

void TestControlChannel::TestSend_UrlRequest_Request()
{
	const QByteArray CoUrlRequest("<?xml version=\"1.0\" encoding=\"utf-8\"?>" \
						   "<url_request xmlns=\"http://www.gilbarco.com/crind/display_manager/api\">" \
						   	   	"<interface_version>01.0.01</interface_version>" \
								"<url>https://127.0.0.1</url>" \
						   "</url_request>");

	const QString CstrCmdName = "url_request";

	CheckIfResponseCommandOk(CoUrlRequest, CstrCmdName);
}

void TestControlChannel::TestSend_UrlRequestNonSecure_Request()
{
	//This proves that a non secure address is not allowed.
	const QByteArray CoUrlRequest("<?xml version=\"1.0\" encoding=\"utf-8\"?>" \
						   "<url_request xmlns=\"http://www.gilbarco.com/crind/display_manager/api\">" \
						   	   	"<interface_version>01.0.01</interface_version>" \
								"<url>http://10.28.46.234:80/gvr/merch/encore_color/page.php?fp=1&amp;page_index=1</url>" \
						   "</url_request>");

	const QString CstrCmdName = "url_request";

	CheckIfResponseCommandOk(CoUrlRequest, CstrCmdName);
}

void TestControlChannel::TestSend_UrlJavascriptSecureRequest_Request()
{
	const QByteArray CoUrlRequest("<?xml version=\"1.0\" encoding=\"utf-8\"?>" \
						   "<url_request xmlns=\"http://www.gilbarco.com/crind/display_manager/api\">" \
						   	   	"<interface_version>01.0.01</interface_version>" \
								"<url>audio://play?f0=1_1.mp2&amp;t0=8&amp;f1=1_2.mp2&amp;t1=8&amp;f2=1_3.mp2&amp;t2=8&amp;f3=1_4.mp2&amp;t3=8&amp;url=http%3A%2F%2F10.28.46.234%3A80%2Fgvr%2Fmerch%2Fencore_color%2Fpage.php%3Ffp%3D3%26page_index%3D1</url>" \
						   "</url_request>");

	const QString CstrCmdName = "url_request";

	CheckIfResponseCommandOk(CoUrlRequest, CstrCmdName);
}

void TestControlChannel::TestSend_UrlJavascriptSecureIIRequest_Request()
{
	//The same test as the previous one but using " instead of '
	const QByteArray CoUrlRequest("<?xml version=\"1.0\" encoding=\"utf-8\"?>" \
						   "<url_request xmlns=\"http://www.gilbarco.com/crind/display_manager/api\">" \
						   	   	"<interface_version>01.0.01</interface_version>" \
								"<url>audio://volume-mixer?mute=true&amp;url=javascript%3Amute_image_swap()</url>" \
						   "</url_request>");

	const QString CstrCmdName = "url_request";

	CheckIfResponseCommandOk(CoUrlRequest, CstrCmdName);
}

void TestControlChannel::TestSend_UrlJavascriptNonSecureRequest_Request()
{
	//This proves that location.href in javascript to a non secure address is not allowed
	const QByteArray CoUrlRequest("<?xml version=\"1.0\" encoding=\"utf-8\"?>" \
						   "<url_request xmlns=\"http://www.gilbarco.com/crind/display_manager/api\">" \
						   	   	"<interface_version>01.0.01</interface_version>" \
								"<url>video://vid_1_4_640_480_1248_12.avi?h=640&amp;v=480&amp;x=0&amp;y=0&amp;url=</url>" \
						   "</url_request>");

	const QString CstrCmdName = "url_request";

	CheckIfResponseCommandNotOk(CoUrlRequest, CstrCmdName);
}

// -----------------------------------------------------------------------------------------------------------------------

void TestControlChannel::TestStopControlChannelManager()
{	
	m_poControlChMgr->Stop();
	m_poControlChMgr->wait();
	QVERIFY(m_poControlChMgr->isRunning() == false);
}

QTEST_MAIN(TestControlChannel)
