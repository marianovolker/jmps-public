#include "TestModelImporterExporter.h"

#include "utils/logger/src/GLogger.h"
#include "utils/logger/src/GLogConfig.h"
#include "utils/logger/src/GLogManager.h"

void TestModelImporterExporter::initTestCase()
{
  	GLogConfig* poConfig;
  	GLogger* poLogger;

  	// Logger StartUp
  	poConfig = new GLogConfig(GLogConfig::eBoth, GLogConfig::eLevel1, "./DisplayManager.log");
	poLogger = new GLogger(poConfig);
	GLogManager::AddLogger("DisplayManager.log", poLogger, false);

	pModel = new GISModel();	
	AddDisplays(2, 2, 3, 4);
}

void TestModelImporterExporter::AddDisplays(int iDisplaysNumber, 
								 	int iScreensNumber, 
								 	int iSegmentsNumber, 
								 	int iFeldsNumber)
{
	GDisplay* oDis;	
	GColorPalette* oPalette;
	int i(0), j;	
	
	while(++i <= iDisplaysNumber)
	{
		oDis = new GDisplay(i);		
		oPalette = oDis->GetColorPalette();
		oDis->SetColor(i <= 2);	

		// Set a new color palette
		for(j = 0; ++j <= GColorPalette::CLastRegister; )
		{
			oPalette->SetColor(j, j, j + 1, j + 2);
		}
		
		pModel->AddDisplay(oDis);
		
		AddScreens(iScreensNumber, iSegmentsNumber, iFeldsNumber, oDis);			
	}	
	
}

void TestModelImporterExporter::AddScreens(int iScreensNumber, int iSegmentsNumber, int iFeldsNumber, GDisplay *oDis)
{
	GScreen* oScr;
	int i(0);

	while(++i <= iScreensNumber)
	{
		oScr = new GScreen(i, oDis);
		
		AddSegments(iSegmentsNumber, iFeldsNumber, oScr);
		oScr->SetBGColor(9 * i);
		
		oDis->AddScreen(oScr);
	}		
}

void TestModelImporterExporter::AddSegments(int iSegmentsNumber, int iFeldsNumber, GScreen *oScr)
{
	GSegment* oSeg;
	int i(0);

	while(++i <= iSegmentsNumber)
	{
		oSeg = new GSegment(i, oScr);
		
		AddFields(iFeldsNumber, oSeg);
		oSeg->SetBGColor(2 * i);
		oSeg->SetVisible(i > 2);
		oSeg->SetWidth(20 * i);
		oSeg->SetHeight(50 * i);
		oSeg->SetXPos(50 * i);
		oSeg->SetYPos(90 * i);
		
		oScr->AddSegment(oSeg);
	}	
}

void TestModelImporterExporter::AddFields(int iFeldsNumber, GSegment *oSegment)
{
	GTextField* oTextField;
	int i(0);

	while(++i <= iFeldsNumber)
	{
		oTextField = new GTextField(i, oSegment);

		oTextField->SetFlashing(i > 2);
		oTextField->SetJustification(i > 2);
		oTextField->SetScrollSpeed(20 * i);
		oTextField->SetUnderscore(i < 2);
		oTextField->SetVisible(i > 2);
		oTextField->SetXpos(50 * i);
		oTextField->SetYpos(50 * i);

		oTextField->SetScroll(50 * i);
		oTextField->SetFont(i);
		
		oSegment->AddField(oTextField);
	}	
}

void TestModelImporterExporter::cleanupTestCase()
{		
	QFile oFile;
	
	delete pModel;	
	pModel = NULL;
	
	if(oFile.exists("model.xml"))
	{
		oFile.remove("model.xml");
	}
	
	if(oFile.exists("modelImportedExported.xml"))
	{
		oFile.remove("modelImportedExported.xml");
	}		
}

void TestModelImporterExporter::ExportModel()
{	
	GISModelExporter::Export(pModel, "model.xml");
}

void TestModelImporterExporter::ImportAndReExportModel()
{		
	GISModel* pModel = GISModelImporter::Import("model.xml");
	QVERIFY(pModel != NULL);	

	// ReExport imported model
	GISModelExporter::Export(pModel, "modelImportedExported.xml");
}

void TestModelImporterExporter::CompareFiles()
{
    QFile oFileExported("model.xml");
    QFile oFileImportedReExported("modelImportedExported.xml");
    char cExported, cReExported;
    
    QVERIFY( oFileExported.open(QIODevice::ReadOnly | QIODevice::Text) == true );
    QVERIFY( oFileImportedReExported.open(QIODevice::ReadOnly | QIODevice::Text) == true );  
    
    while (!oFileExported.atEnd() && !oFileImportedReExported.atEnd()) 
    {
    	oFileExported.getChar(&cExported);
    	oFileImportedReExported.getChar(&cReExported);
        
        QVERIFY( cExported == cReExported );
    }
    
    QVERIFY( oFileExported.atEnd() == oFileImportedReExported.atEnd() );    
    
    oFileExported.close();
    oFileImportedReExported.close();
}


QTEST_MAIN(TestModelImporterExporter)
