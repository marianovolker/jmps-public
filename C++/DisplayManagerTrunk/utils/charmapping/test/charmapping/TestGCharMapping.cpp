#include "TestGCharMapping.h"
#include "../../../logger/src/GLogger.h"

#define CLoopsToRun 1000

void TestGCharMapping::initTestCase()
{
	GLogConfig* poConfig;
	GLogger* poLogger;

	// Logger StartUp
	poConfig = new GLogConfig(GLogConfig::eFile, GLogConfig::eLevel4, "./DisplayManager.log");
	poLogger = new GLogger(poConfig);
	GLogManager::AddLogger("DisplayManager.log", poLogger, false);
	LOG_LEVEL3("Starting Up Test...");

	try
	{
		m_poGCharMapping = new GCharMapping("x.map");

	}
	catch(GException e)
	{
		qDebug() << "Exception : [" << " GCharMapping=" << e.GetMessage() << "]";
		QVERIFY(false);
	}
}

void TestGCharMapping::cleanupTestCase()
{
	try
	{
		if( m_poGCharMapping )
		{
			m_poGCharMapping->Reset();
		}
	}
	catch(GException& e)
	{
		qDebug() << "Exception : [" << " GCharMapping=" << e.GetMessage() << "]";
		QVERIFY(false);
	}
}

void TestGCharMapping::char000Test()
{
	try
	{
		QChar qc=m_poGCharMapping->GetQChar(0);
		QVERIFY( qc== 0);
	}
	catch(GException& e)
	{
		qDebug() << "Exception : [" << " GCharMapping=" << e.GetMessage() << "]";
		QVERIFY(false);
	}
}

void TestGCharMapping::char001Test()
{
	try
	{
		QChar qc=m_poGCharMapping->GetQChar(1);
		QVERIFY( qc== 1);
	}
	catch(GException& e)
	{
		qDebug() << "Exception : [" << " GCharMapping=" << e.GetMessage() << "]";
		QVERIFY(false);
	}
}

void TestGCharMapping::char002Test()
{
	try
	{
		QChar qc=m_poGCharMapping->GetQChar(2);
		QVERIFY( qc== 2);
	}
	catch(GException& e)
	{
		qDebug() << "Exception : [" << " GCharMapping=" << e.GetMessage() << "]";
		QVERIFY(false);
	}
}

void TestGCharMapping::char003Test()
{
	try
	{
		QChar qc=m_poGCharMapping->GetQChar(3);
		QVERIFY( qc== 0xe4);
	}
	catch(GException& e)
	{
		qDebug() << "Exception : [" << " GCharMapping=" << e.GetMessage() << "]";
		QVERIFY(false);
	}
}

void TestGCharMapping::char004Test()
{
	try
	{
		QChar qc=m_poGCharMapping->GetQChar(4);
		QVERIFY( qc== 33);
	}
	catch(GException& e)
	{
		qDebug() << "Exception : [" << " GCharMapping=" << e.GetMessage() << "]";
		QVERIFY(false);
	}
}

QTEST_MAIN(TestGCharMapping)
