#include "TestScreen.h"

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
	
	// Add Segments
	GScreen* scr = pModel->GetDisplay(1)->GetScreen(1);
	scr->AddSegment(new GSegment(1, scr));
	scr->AddSegment(new GSegment(2, scr));
	scr->AddSegment(new GSegment(3, scr));	
}

void TestModel::cleanupTestCase()
{		
	delete pModel;
	pModel = NULL;
}


void TestModel::getSegments()
{
	GScreen* scr = pModel->GetDisplay(1)->GetScreen(1);
	
	QVERIFY(scr->GetSegment(0) == NULL);
	QVERIFY(scr->GetSegment(1) != NULL);
	QVERIFY(scr->GetSegment(2) != NULL);
	QVERIFY(scr->GetSegment(3) != NULL);
	QVERIFY(scr->GetSegment(4) == NULL);	
}

void TestModel::getAllSegments()
{
	GScreen* scr = pModel->GetDisplay(1)->GetScreen(1);	
	QList<GSegment *> segmentList(scr->GetAllSegments());	
	QList<GSegment *>::iterator i = segmentList.begin();
	
	GSegment* aux;
	while (i != segmentList.end())
	{ 
		aux = *i;
		QVERIFY( aux != NULL && aux->GetNumber() >= 1 && aux->GetNumber() <= 3 );
		++i;
	}	
}

void TestModel::removeSegments()
{
	GScreen* scr = pModel->GetDisplay(1)->GetScreen(1);
	QVERIFY(scr->GetSegment(2) != NULL);	
	scr->RemoveSegment(2);
	QVERIFY(scr->GetSegment(2) == NULL);		
}

void TestModel::screenAttributes()
{
	GScreen* scr = pModel->GetDisplay(1)->GetScreen(1);
	QVERIFY(scr->GetNumber() == 1);
	
	scr->SetBGColor(1234);
	QVERIFY(scr->GetBGColor() == 1234);
}

QTEST_MAIN(TestModel)
