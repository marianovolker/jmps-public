#include "TestSegment.h"

#include "utils/logger/src/GLogger.h"
#include "utils/logger/src/GLogConfig.h"
#include "utils/logger/src/GLogManager.h"

void TestSegment::initTestCase()
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
	GDisplay* dis = new GDisplay(1);
	pModel->AddDisplay(dis);
	
	// Add Screens
	GScreen* scr = new GScreen(1, dis);
	dis->AddScreen(scr);
	
	// Add Segments
	GSegment* seg = new GSegment(1, scr);
	scr->AddSegment(seg);

	// Add TextFields
	seg->AddField(new GTextField(1, seg));
	seg->AddField(new GTextField(2, seg));
	seg->AddField(new GTextField(3, seg));
	seg->AddField(new GTextField(4, seg));
	seg->AddField(new GTextField(5, seg));
}

void TestSegment::cleanupTestCase()
{		
	delete pModel;
	pModel = NULL;
}


void TestSegment::getSegments()
{
	GSegment* seg = pModel->GetDisplay(1)->GetScreen(1)->GetSegment(1);
	
	QVERIFY(seg->GetField(0) == NULL);
	QVERIFY(seg->GetField(1) != NULL);
	QVERIFY(seg->GetField(2) != NULL);
	QVERIFY(seg->GetField(3) != NULL);
	QVERIFY(seg->GetField(4) != NULL);
	QVERIFY(seg->GetField(5) != NULL);
	QVERIFY(seg->GetField(6) == NULL);	
}

void TestSegment::getAllSegments()
{
	GSegment* seg = pModel->GetDisplay(1)->GetScreen(1)->GetSegment(1);	
	QList<GTextField *> fieldList(seg->GetAllFields());	
	QList<GTextField *>::iterator i = fieldList.begin();
	
	GTextField* aux;
	while (i != fieldList.end())
	{ 
		aux = *i;
		QVERIFY( aux != NULL && aux->GetNumber() >= 1 && aux->GetNumber() <= 5 );
		++i;
	}	
}

void TestSegment::removeSegments()
{
	GSegment* seg = pModel->GetDisplay(1)->GetScreen(1)->GetSegment(1);
	QVERIFY(seg->GetField(2) != NULL);	
	seg->RemoveField(2);
	QVERIFY(seg->GetField(2) == NULL);		
}

void TestSegment::segmentAttributes()
{
	GSegment* seg = pModel->GetDisplay(1)->GetScreen(1)->GetSegment(1);
	QVERIFY(true == true);
	
	
	seg->SetBGColor(1234);
	seg->SetVisible(false);
	seg->SetWidth(340);
	seg->SetHeight(480);
	seg->SetXPos(5);
	seg->SetYPos(6);
	
	QVERIFY(seg->GetBGColor() == 1234);	
	QVERIFY(seg->IsVisible() == false);	
	QVERIFY(seg->GetWidth() == 340);
	QVERIFY(seg->GetHeight() == 480);
	QVERIFY(seg->GetXPos() == 5);
	QVERIFY(seg->GetYPos() == 6);	
	QVERIFY(seg->GetNumber() == 1);
	QVERIFY(seg->GetScreen() == pModel->GetDisplay(1)->GetScreen(1));	
}

QTEST_MAIN(TestSegment)
