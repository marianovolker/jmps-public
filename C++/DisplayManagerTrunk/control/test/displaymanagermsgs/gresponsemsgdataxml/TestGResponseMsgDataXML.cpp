#include "TestGResponseMsgDataXML.h"

#include "control/src/displaymanagermsgs/xml/GResponseMsgDataXML.h"

#include "utils/logger/src/GLogger.h"
#include "utils/logger/src/GLogConfig.h"
#include "utils/logger/src/GLogManager.h"

void TestGResponseMsgDataXML::initTestCase()
{
  	GLogConfig* poConfig;
  	GLogger* poLogger;

  	// Logger StartUp
  	poConfig = new GLogConfig(GLogConfig::eBoth, GLogConfig::eLevel1, "./DisplayManager.log");
	poLogger = new GLogger(poConfig);
	GLogManager::AddLogger("DisplayManager.log", poLogger, false);

	LOG_LEVEL3("Starting Unit Test...");
}

void TestGResponseMsgDataXML::cleanupTestCase()
{
	LOG_LEVEL4("cleanupTestCase()");

}

void TestGResponseMsgDataXML::gResponseMsgDataUnMarshall()
{
	LOG_LEVEL4("gResponseMsgDataUnMarshall()");
	
	QByteArray oXML("<?xml version=\"1.0\" encoding=\"utf-8\"?>" \
				   "<response xmlns=\"http://www.gilbarco.com/crind/displayManager/api\">" \
						"<interface_version>01.0.01</interface_version>" \
						"<command>kiosk_id_request</command>" \
						"<response_code>ok</response_code>" \
				   "</response>");
	
	GResponseMsgData *oRequest = GResponseMsgDataXML::UnMarshall(oXML);

	QVERIFY(oRequest != NULL);	
	QCOMPARE(oRequest->GetInterfaceVersion() , QString("01.0.01"));
	QCOMPARE(oRequest->GetCommand() , QString("kiosk_id_request"));
	QVERIFY(oRequest->GetResponseCode() == GResponseMsgData::eOk);
}

void TestGResponseMsgDataXML::gResponseMsgDataMarshall()
{
	LOG_LEVEL4("gResponseMsgDataMarshall()");
	
	GResponseMsgData* oRequest = new GResponseMsgData();
	oRequest->SetInterfaceVersion(QString("01.0.01"));
	oRequest->SetCommand(QString("kiosk_id_request"));
	oRequest->SetResponseCode(GResponseMsgData::eOk);
	
	// Serialize the object
	QByteArray oXML = GResponseMsgDataXML::Marshall(oRequest);
	
	// Check whether it failed or not
	QVERIFY(!oXML.isEmpty());	

	// Delete the manually loaded object
	delete oRequest;
	oRequest = NULL;
	
	// UnMarshall the previously marshalled object
	oRequest = GResponseMsgDataXML::UnMarshall(oXML);
	
	QCOMPARE(oRequest->GetInterfaceVersion() , QString("01.0.01"));
	QCOMPARE(oRequest->GetCommand() , QString("kiosk_id_request"));
	QVERIFY(oRequest->GetResponseCode() == GResponseMsgData::eOk);
}

QTEST_MAIN(TestGResponseMsgDataXML)
