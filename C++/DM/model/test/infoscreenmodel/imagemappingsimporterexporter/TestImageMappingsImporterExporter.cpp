#include "TestImageMappingsImporterExporter.h"

#include "utils/logger/src/GLogger.h"
#include "utils/logger/src/GLogConfig.h"
#include "utils/logger/src/GLogManager.h"

#include <memory>

void TestImageMappingsImporterExporter::initTestCase()
{
  	GLogConfig* poConfig;
  	GLogger* poLogger;
  	m_poImageMappings = QSharedPointer<GImageMappings> (new GImageMappings());

  	// Logger StartUp
  	poConfig = new GLogConfig(GLogConfig::eBoth, GLogConfig::eLevel1, "./DisplayManager.log");
	poLogger = new GLogger(poConfig);
	GLogManager::AddLogger("DisplayManager.log", poLogger, false);

	
	std::auto_ptr<GImageMap> poTestImage1(new GImageMap("Image1", "image1.jpg", 1, "jpg"));
	std::auto_ptr<GImageMap> poTestImage2(new GImageMap("Image2", "image2.jpg", 2, "jpg"));
	std::auto_ptr<GImageMap> poTestImage3(new GImageMap("Image3", "image3.jpg", 3, "jpg"));
	std::auto_ptr<GImageMap> poTestImage4(new GImageMap("Image4", "image4.jpg", 4, "jpg"));
	std::auto_ptr<GImageMap> poTestImage5(new GImageMap("Image5", "image5.jpg", 5, "jpg"));


	m_poImageMappings->AddImageMap(poTestImage1);
	
	m_poImageMappings->AddImageMap(poTestImage2);

	m_poImageMappings->AddImageMap(poTestImage3);

	m_poImageMappings->AddImageMap(poTestImage4);

	m_poImageMappings->AddImageMap(poTestImage5);

	
}

void TestImageMappingsImporterExporter::cleanupTestCase()
{	
	QFile oFile;
		
	if(oFile.exists("imagemappings.xml"))
	{
		oFile.remove("imagemappings.xml");
	}
	
	if(oFile.exists("imagemappingsImportedExported.xml"))
	{
		oFile.remove("imagemappingsImportedExported.xml");
	}
}

void TestImageMappingsImporterExporter::ExportImageMappings()
{
	GImageMappingsExporter::Export(m_poImageMappings, "imagemappings.xml");
}


void TestImageMappingsImporterExporter::ImportAndReExportImageMappings()
{		
	QSharedPointer<GImageMappings> poImageMappings(new GImageMappings());
	
	GImageMappingsImporter::Import("imagemappings.xml", poImageMappings);
	QVERIFY(!((poImageMappings->GetAllImageMaps()).isEmpty()));
	
	// ReExport imported ImageMappings
	GImageMappingsExporter::Export(poImageMappings, "imagemappingsImportedExported.xml");
}

void TestImageMappingsImporterExporter::CompareFiles()
{
    QFile oFileExported("imagemappings.xml");
    QFile oFileImportedReExported("imagemappingsImportedExported.xml");
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

QTEST_MAIN(TestImageMappingsImporterExporter)
