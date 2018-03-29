#include "TestGLogger.h"

#define CLoopsToRun 1000

void TestGLogger::initTestCase()
{
	GLogConfig* poConfig;
	oRegExp = new QRegExp("\\[(\\d){2}.(\\d){2}.(\\d){4} (\\d){2}:(\\d){2}:(\\d){2}.(\\d){3}\\] \\[(L)\\d\\] \\[(\\S+)\\] \\[(\\s+)(TestGLogger.cpp):(\\S+)\\] \\[(\\s+)(\\S+)\\] \\[(Error number) (1|2|3|4) (loop)(\\s+)(\\d+)\\]\\n");

	poConfig = new GLogConfig(GLogConfig::eFile, GLogConfig::eLevel1, "./DisplayManagerLvl1.log");
	GLogManager::AddLogger("DisplayManagerLvl1.log", new GLogger(poConfig), true);

	poConfig = new GLogConfig(GLogConfig::eFile, GLogConfig::eLevel2, "./DisplayManagerLvl2.log");
	GLogManager::AddLogger("DisplayManagerLvl2.log", new GLogger(poConfig), false);

	poConfig = new GLogConfig(GLogConfig::eFile, GLogConfig::eLevel3, "./DisplayManagerLvl3.log");
	GLogManager::AddLogger("DisplayManagerLvl3.log", new GLogger(poConfig), false);

	poConfig = new GLogConfig(GLogConfig::eFile, GLogConfig::eLevel4, "./DisplayManagerLvl4.log");
	GLogManager::AddLogger("DisplayManagerLvl4.log", new GLogger(poConfig), false);	
}

void TestGLogger::cleanupTestCase()
{
	QFile oFile;

	delete oRegExp;
	oRegExp = NULL;

	// Please note that... if in real use we shoundn't delete the file where the logger is working
	if( oFile.exists("./DisplayManagerLvl1.log") )
	{
		oFile.remove("./DisplayManagerLvl1.log");
	}

	if( oFile.exists("./DisplayManagerLvl2.log") )
	{
		oFile.remove("./DisplayManagerLvl2.log");
	}

	if( oFile.exists("./DisplayManagerLvl3.log") )
	{
		oFile.remove("./DisplayManagerLvl3.log");
	}

	if( oFile.exists("./DisplayManagerLvl4.log") )
	{
		oFile.remove("./DisplayManagerLvl4.log");
	}
}

void TestGLogger::WriteLevel1()
{	
	int i = -1;
	GLogger* poLogger = GLogManager::GetLogger("DisplayManagerLvl1.log");

	while( (++i) <= CLoopsToRun )
	{
		_LOG_LEVEL1(poLogger, QString("Error number 1 loop ") + QString::number(i) );
		_LOG_LEVEL2(poLogger, QString("Error number 2 loop ") + QString::number(i) );
		_LOG_LEVEL3(poLogger, QString("Error number 3 loop ") + QString::number(i) );
		_LOG_LEVEL4(poLogger, QString("Error number 4 loop ") + QString::number(i) );
	}
	
	delete poLogger;

	CheckFile("DisplayManagerLvl1.log", CLoopsToRun, 1);
}

void TestGLogger::WriteLevel2()
{	
	int i = -1;
	GLogger* poLogger = GLogManager::GetLogger("DisplayManagerLvl2.log");

	while( ++i <= CLoopsToRun )
	{
		_LOG_LEVEL1(poLogger, QString("Error number 1 loop ") + QString::number(i) );
		_LOG_LEVEL2(poLogger, QString("Error number 2 loop ") + QString::number(i) );
		_LOG_LEVEL3(poLogger, QString("Error number 3 loop ") + QString::number(i) );
		_LOG_LEVEL4(poLogger, QString("Error number 4 loop ") + QString::number(i) );
	}

	delete poLogger;
	
	CheckFile("DisplayManagerLvl2.log", CLoopsToRun, 2);	
}

void TestGLogger::WriteLevel3()
{	
	int i = -1;
	GLogger* poLogger = GLogManager::GetLogger("DisplayManagerLvl3.log");	

	while( ++i <= CLoopsToRun )
	{
		_LOG_LEVEL1(poLogger, QString("Error number 1 loop ") + QString::number(i) );
		_LOG_LEVEL2(poLogger, QString("Error number 2 loop ") + QString::number(i) );
		_LOG_LEVEL3(poLogger, QString("Error number 3 loop ") + QString::number(i) );
		_LOG_LEVEL4(poLogger, QString("Error number 4 loop ") + QString::number(i) );
	}

	delete poLogger;

	CheckFile("DisplayManagerLvl3.log", CLoopsToRun, 3);	
}

void TestGLogger::WriteLevel4()
{	
	int i = -1;
	GLogger* poLogger = GLogManager::GetLogger("DisplayManagerLvl4.log");	

	while( ++i <= CLoopsToRun )
	{
		_LOG_LEVEL1(poLogger, QString("Error number 1 loop ") + QString::number(i) );
		_LOG_LEVEL2(poLogger, QString("Error number 2 loop ") + QString::number(i) );
		_LOG_LEVEL3(poLogger, QString("Error number 3 loop ") + QString::number(i) );
		_LOG_LEVEL4(poLogger, QString("Error number 4 loop ") + QString::number(i) );
	}

	delete poLogger;

	CheckFile("DisplayManagerLvl4.log", CLoopsToRun, 4);
}

void TestGLogger::CheckFile(const char* pszFileName, int iLoopsExecuted, int iEntriesPerLoop)
{
	QFile oFile(pszFileName);	
	QByteArray oLine;
	int i = 0, j = 1;
	
	QVERIFY(oFile.open(QIODevice::ReadOnly | QIODevice::Text));		 
	QVERIFY(oFile.size() > 1024);  // We should have, by far, more than 1k logged
	
	oLine = oFile.readLine();
		
	while(oLine.length() != 0)
	{
		if(oLine[0] != '#' && oLine[0] != '\n')	// Skip the header and empty lines
		{
			QString strStringLine(oLine);
			QVERIFY(oRegExp->exactMatch(strStringLine));
			QVERIFY(oRegExp->cap(oRegExp->numCaptures()).toInt() == i);
			if( ++j > iEntriesPerLoop )
			{
				++i;
				j = 1;
			}			
		}
		oLine = oFile.readLine();			
	}

	QVERIFY((i - 1) == iLoopsExecuted);
}

QTEST_MAIN(TestGLogger)
