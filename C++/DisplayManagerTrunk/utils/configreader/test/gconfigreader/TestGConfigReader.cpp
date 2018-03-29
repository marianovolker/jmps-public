#include "TestGConfigReader.h"
#include "../../../logger/src/GLogger.h"

#define CLoopsToRun 1000

void TestGConfigReader::initTestCase()
{
	GLogConfig* poConfig;
	GLogger* poLogger;

	// Logger StartUp
	poConfig = new GLogConfig(GLogConfig::eFile, GLogConfig::eLevel4, "./DisplayManager.log");
	poLogger = new GLogger(poConfig);
	GLogManager::AddLogger("DisplayManager.log", poLogger, false);
	LOG_LEVEL3("Starting Up Test...");

	m_poConfigReader=NULL;
	try
	{
		m_poConfigReader = new GConfigReader("displaymanager.properties");

		m_poConfigReader->Show();
	}
	catch(GException e)
	{
		qDebug() << "Exception : [" << " GConfigReader=" << e.GetMessage() << "]";
		QVERIFY(false);
	}
}

void TestGConfigReader::cleanupTestCase()
{
	try
	{
		if( m_poConfigReader )
		{
			m_poConfigReader->Reset();
		}
	}
	catch(GException& e)
	{
		qDebug() << "Exception : [" << " GConfigReader=" << e.GetMessage() << "]";
		QVERIFY(false);
	}
}

void TestGConfigReader::int0Test()
{
	try
	{
		QVERIFY(m_poConfigReader->GetInt("int0") == 4);
	}
	catch(GException& e)
	{
		qDebug() << "Exception : [" << " GConfigReader=" << e.GetMessage() << "]";
		QVERIFY(false);
	}
}

void TestGConfigReader::int1Test()
{
	try
	{
		QVERIFY(m_poConfigReader->GetInt("int1") == 10);
	}
	catch(GException& e)
	{
		qDebug() << "Exception : [" << " GConfigReader=" << e.GetMessage() << "]";
		QVERIFY(false);
	}
}

void TestGConfigReader::string0Test()
{
	try
	{
		QVERIFY(m_poConfigReader->GetString("string0") == "xyz");
	}
	catch(GException& e)
	{
		qDebug() << "Exception : [" << " GConfigReader=" << e.GetMessage() << "]";

		QVERIFY(false);
	}
}

void TestGConfigReader::string1Test()
{
	try
	{
		QVERIFY(m_poConfigReader->GetString("string1") == "hello");
	}
	catch(GException& e)
	{
		qDebug() << "Exception : [" << " GConfigReader=" << e.GetMessage() << "]";

		QVERIFY(false);
	}
}

void TestGConfigReader::bool0Test()
{
	try
	{
		QVERIFY(m_poConfigReader->GetBool("bool0") == true);
	}
	catch(GException& e)
	{
		qDebug() << "Exception : [" << " GConfigReader=" << e.GetMessage() << "]";

		QVERIFY(false);
	}
}

void TestGConfigReader::bool1Test()
{
	try
	{
		QVERIFY(m_poConfigReader->GetBool("bool1") == false);
	}
	catch(GException& e)
	{
		qDebug() << "Exception : [" << " GConfigReader=" << e.GetMessage() << "]";

		QVERIFY(false);
	}
}

void TestGConfigReader::setTest()
{
	try
	{
		QVERIFY(m_poConfigReader->Set("DisplayManager.PersonalityName", "red") == true);
	}
	catch(GException& e)
	{
		qDebug() << "Exception : [" << " GConfigReader=" << e.GetMessage() << "]";

		QVERIFY(false);
	}
}

void TestGConfigReader::saveTest()
{
	try
	{
		m_poConfigReader->Save();
	}
	catch(GException& e)
	{
		qDebug() << "Exception : [" << " GConfigReader=" << e.GetMessage() << "]";

		QVERIFY(false);
	}
}

QTEST_MAIN(TestGConfigReader)
