#include "TestTextField.h"

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
	dis = new GDisplay(1);
	pModel->AddDisplay(dis);
	
	// Add Screens
	scr = new GScreen(1, dis);
	dis->AddScreen(scr);
	
	// Add Segments
	seg = new GSegment(1, scr);
	scr->AddSegment(seg);

	// Add TextFields
	field = new GTextField(1, seg);
	seg->AddField(field);
}

void TestSegment::cleanupTestCase()
{		
	delete pModel;
	pModel = NULL;
}


void TestSegment::fieldAttributes()
{
	
	field->SetText("Follow the Leader");
	field->SetFlashing(true);
	field->SetJustification(GTextField::CJustificationNone);
	field->SetScrollSpeed(GTextField::CSpeedLow);

	field->SetUnderscore(false);
	field->SetVisible(true);	
	field->SetXpos(67);
	field->SetYpos(68);
	field->SetScroll(GTextField::CScrollVertical);
	field->SetFont(5);
	
	QVERIFY(field->GetNumber() == 1);
	QVERIFY(field->GetText() == "Follow the Leader");
	QVERIFY(field->IsFlashing() == true);
	QVERIFY(field->GetJustification() == GTextField::CJustificationNone);
	QVERIFY(field->GetScrollSpeed() == GTextField::CSpeedLow);	
	QVERIFY(field->IsUnderscore() == false);
	QVERIFY(field->IsVisible() == true);
	QVERIFY(field->GetXpos() == 67);
	QVERIFY(field->GetYpos() == 68);
	QVERIFY(field->GetScroll() == GTextField::CScrollVertical);
	QVERIFY(field->GetFont() == 5);
		
	
	field->SetAttr("21");
	QVERIFY(field->GetAttr() == "21");
	QVERIFY(field->GetAttribute() == GAttributeEnum::ePopTextSlow);		
	QVERIFY(field->GetScroll() == GTextField::CScrollPop);
	QVERIFY(field->GetScrollSpeed() == GTextField::CSpeedLow);
}

QTEST_MAIN(TestSegment)
