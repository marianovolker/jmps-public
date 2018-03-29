#include <QtTest/QtTest>
#include <QString>
#include "../../src/GControlChannelMgr.h"



class TestControlChannel : public QObject
{
	Q_OBJECT
public:
	TestControlChannel();
	~TestControlChannel();

private:
	GControlChannelMgr* m_poControlChMgr;
	QTcpSocket oSock, oSock2;
	const QString m_CstrOkResponse;
	const QString m_CstrResponseCmdTag;
	const int m_CiTestPort;
	const int m_CiResponseWaitTimeOut;
	
	void CheckIfResponseCommandOk(const QByteArray &CoRequestMsg, const QString &CstrCmdName);
	void WaitForNConnections(int iNumberOfConnections, int iTimeToWaitInSeconds);
	void CheckIfResponseCommandNotOk(const QByteArray &CoRequestMsg, const QString &CstrCmdName);

private slots:

	void initTestCase();
	void cleanupTestCase();

	// ControlChannel Testing
	void TestStartControlChannelManager();
	void TestConnection();
	
	void TestTwoPartPacket();
	void TestTwoMessages();
	void TestTwoMessagesTwoPackets();
	
	void TestSend_ClearScreen_Request();
	void TestSend_GdiClearScreen_Request();
	void TestSend_GdiDrawline_Request();
	void TestSend_GdiDrawtext_Request();
	void TestSend_GdiFillrect_Request();
	void TestSend_GdiGetScreensize_Request();
	void TestSend_GdiRefreshScreen_Request();
	void TestSend_DownloadGraphic_Request();
	void TestSend_Flytext_Request();
	void TestSend_Infoscreen_Request();
	void TestSend_KeyEvent_Request();
	void TestSend_KioskId_Request();
	void TestSend_GetPersonalities_Request();
	void TestSend_SetPersonality_Request();
	void TestSend_SetDefaultPersonality_Request();
	void TestSend_UrlRequest_Request();
	void TestSend_UrlRequestNonSecure_Request();
	void TestSend_UrlJavascriptSecureRequest_Request();
	void TestSend_UrlJavascriptSecureIIRequest_Request();
	void TestSend_UrlJavascriptNonSecureRequest_Request();

	void TestStopControlChannelManager();

};
