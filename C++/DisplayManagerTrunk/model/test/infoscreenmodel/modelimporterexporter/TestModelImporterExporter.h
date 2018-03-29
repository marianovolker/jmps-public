#include <QtTest/QtTest>
#include <QString>
#include <QFile>
#include "model/src/infoscreenmodel/GISModel.h"
#include "model/src/infoscreenmodel/GDisplay.h"
#include "model/src/infoscreenmodel/GScreen.h"
#include "model/src/infoscreenmodel/GSegment.h"
#include "model/src/infoscreenmodel/GTextField.h"
#include "model/src/infoscreenmodel/GISModelExporter.h"
#include "model/src/infoscreenmodel/GISModelImporter.h"

class TestModelImporterExporter : public QObject
{
	Q_OBJECT
	
private:
	void AddDisplays(int iDisplaysNumber, 
				 	 int iScreensNumber, 
				 	 int iSegmentsNumber, 
				 	 int iFeldsNumber);	
	void AddScreens(int iScreensNumber, int iSegmentsNumber, int iFeldsNumber, GDisplay *oDis);
	void AddSegments(int iSegmentsNumber, int iFeldsNumber, GScreen *oScr);	
	void AddFields(int iFeldsNumber, GSegment *oSegment);	

private slots:		
	void initTestCase();	
	void cleanupTestCase();
	
	// Export Model Test
	void ExportModel();
	void ImportAndReExportModel();
	void CompareFiles();
	
private:
	GISModel* pModel;		
};


