#include "TestModel.h"

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
	pModel->AddDisplay(new GDisplay(2));
	pModel->AddDisplay(new GDisplay(3));
}

void TestModel::cleanupTestCase()
{		
	delete pModel;	
	pModel = NULL;
}

void TestModel::getDisplays()
{	
	QVERIFY(pModel->GetDisplay(1) != NULL);
	QVERIFY(pModel->GetDisplay(2) != NULL);	
	QVERIFY(pModel->GetDisplay(3) != NULL);
	QVERIFY(pModel->GetDisplay(0) == NULL);
	QVERIFY(pModel->GetDisplay(4) == NULL);
}

void TestModel::getAllDisplays()
{		
	QList<GDisplay *> displayList(pModel->GetAllDisplays());	
	QList<GDisplay *>::iterator i = displayList.begin();
	
	GDisplay* aux;
	while (i != displayList.end())
	{ 
		aux = *i;
		QVERIFY( aux != NULL && aux->GetNumber() >= 1 && aux->GetNumber() <= 3 );
		++i;
	}
}

void TestModel::removeDisplay()
{	
	QVERIFY(pModel->GetDisplay(2) != NULL);	
	pModel->RemoveDisplay(2);
	QVERIFY(pModel->GetDisplay(2) == NULL);	
}



QTEST_MAIN(TestModel)
