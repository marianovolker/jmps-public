#include "TestGConfigEnvironment.h"

#include "utils/configenvironment/src/GConfigEnvironment.h"
#include "utils/logger/src/GLogger.h"

#include <stdlib.h>

void TestGConfigEnvironment::initTestCase()
{
	GLogConfig* poConfig;
	GLogger* poLogger;

	// Logger StartUp
	poConfig = new GLogConfig(GLogConfig::eFile, GLogConfig::eLevel4, "./DisplayManager.log");
	poLogger = new GLogger(poConfig);
	GLogManager::AddLogger("DisplayManager.log", poLogger, false);
	LOG_LEVEL3("Starting Up Test...");
}

void TestGConfigEnvironment::cleanupTestCase()
{
}

void TestGConfigEnvironment::checkEnvVarTest()
{
	LOG_LEVEL3("test0()...");
	
	QProcess process;
	QStringList env = QProcess::systemEnvironment();
	env << "DM_CONFIG=./";
	process.setEnvironment(env);

	setenv("DM_CONFIG", "./", 1);

	GConfigEnvironment aConfigEnv;

	QVERIFY( aConfigEnv.GetString("DM_CONFIG") == "./" );
}

QTEST_MAIN(TestGConfigEnvironment)

