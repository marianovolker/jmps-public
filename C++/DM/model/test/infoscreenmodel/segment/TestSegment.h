#include <QtTest/QtTest>
#include <QString>
#include "model/src/infoscreenmodel/GISModel.h"
#include "model/src/infoscreenmodel/GDisplay.h"
#include "model/src/infoscreenmodel/GScreen.h"
#include "model/src/infoscreenmodel/GSegment.h"

class TestSegment: public QObject
{
	Q_OBJECT
	
private slots:		
	void initTestCase();
	void cleanupTestCase();
	
	// Screen Testing
	void getSegments();
	void getAllSegments();
	void removeSegments();
	void segmentAttributes();
	
private:
	GISModel *pModel;
};


