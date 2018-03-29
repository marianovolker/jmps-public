#include "TestGGstDaemonMediaManager.h"
#include "../../../logger/src/GLogger.h"
#include "../../../media/src/GGstDaemonMediaManager.h"

#define CLoopsToRun 1000

void TestGGstDaemonMediaManager::initTestCase()
{
	GLogConfig* poConfig;
	GLogger* poLogger;

	// Logger StartUp
	poConfig = new GLogConfig(GLogConfig::eFile, GLogConfig::eLevel4, "./DisplayManager.log");
	poLogger = new GLogger(poConfig);
	GLogManager::AddLogger("DisplayManager.log", poLogger, false);
	LOG_LEVEL3("Starting Up Test...");

	//m_poGstDaemonMediaManager=NULL;

}

void TestGGstDaemonMediaManager::cleanupTestCase()
{
/*	try
	{
		if( m_poGstDaemonMediaManager )
		{
			m_poGstDaemonMediaManager = NULL;
		}
	}
	catch(GException& e)
	{
		qDebug() << "Exception : [" << " GConfigReader=" << e.GetMessage() << "]";
		QVERIFY(false);
	}
 * */
}

void TestGGstDaemonMediaManager::getInstanceTest()
{
	try
	{
                m_poGstDaemonMediaManager = GGstDaemonMediaManager::GetInstance();
                QVERIFY(m_poGstDaemonMediaManager != NULL);
	}
	catch(GException e)
	{
		qDebug() << "Exception : [" << " GConfigReader=" << e.GetMessage() << "]";
		QVERIFY(false);
	}
}



QTEST_MAIN(TestGGstDaemonMediaManager)
