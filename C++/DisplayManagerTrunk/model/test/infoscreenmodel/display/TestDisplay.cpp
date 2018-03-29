#include "TestDisplay.h"

#include "utils/logger/src/GLogger.h"
#include "utils/logger/src/GLogConfig.h"
#include "utils/logger/src/GLogManager.h"

void TestModel::initTestCase()
{
  	GLogConfig* poConfig;
  	GLogger* poLogger;

  	// Logger StartUp
  	poConfig = new GLogConfig(GLogConfig::eBoth, GLogConfig::eLevel1, "./DisplayManager.log");
	poLogger = new GLogger(poConfig);
	GLogManager::AddLogger("DisplayManager.log", poLogger, false);

	// Create new model
	pModel = new GISModel();
	
	// Add Displays
	pModel->AddDisplay(new GDisplay(1));
	
	// Add Screens
	GDisplay* dis = pModel->GetDisplay(1);
	dis->AddScreen(new GScreen(1, dis));
	dis->AddScreen(new GScreen(2, dis));
	dis->AddScreen(new GScreen(3, dis));		
}

void TestModel::cleanupTestCase()
{		
	delete pModel;
	pModel = NULL;
}

void TestModel::getScreens()
{
	GDisplay* dis = pModel->GetDisplay(1);
	QVERIFY(dis->GetScreen(0) != NULL);
	QVERIFY(dis->GetScreen(1) != NULL);
	QVERIFY(dis->GetScreen(2) != NULL);
	QVERIFY(dis->GetScreen(3) != NULL);
	QVERIFY(dis->GetScreen(4) == NULL);	
}

void TestModel::getAllScreens()
{
	GDisplay* dis = pModel->GetDisplay(1);	
	QList<GScreen *> screenList(dis->GetAllScreens());	
	QList<GScreen *>::iterator i = screenList.begin();
	
	GScreen* aux;
	while (i != screenList.end())
	{ 
		aux = *i;
		QVERIFY( aux != NULL && aux->GetNumber() >= 0 && aux->GetNumber() <= 3 );
		++i;
	}	
}

void TestModel::removeScreens()
{
	GDisplay* dis = pModel->GetDisplay(1);		
	QVERIFY(dis->GetScreen(2) != NULL);	
	dis->RemoveScreen(2);
	QVERIFY(dis->GetScreen(2) == NULL);		
}

void TestModel::currentScreen()
{
	GDisplay* dis = pModel->GetDisplay(1);
	dis->SetCurrentScreen(dis->GetScreen(1));
	
	QVERIFY(dis->GetCurrentScreen() == dis->GetScreen(1));
}

QTEST_MAIN(TestModel)
