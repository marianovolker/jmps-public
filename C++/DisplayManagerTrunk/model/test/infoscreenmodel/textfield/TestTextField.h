#include <QtTest/QtTest>
#include <QString>
#include "model/src/infoscreenmodel/GISModel.h"
#include "model/src/infoscreenmodel/GDisplay.h"
#include "model/src/infoscreenmodel/GScreen.h"
#include "model/src/infoscreenmodel/GSegment.h"
#include "model/src/infoscreenmodel/GTextField.h"
#include "model/src/infoscreenmodel/GAttributeEnum.h"

class TestSegment: public QObject
{
	Q_OBJECT
	
private slots:		
	void initTestCase();
	void cleanupTestCase();
	
	// TextField Testing
	void fieldAttributes();	

private:
	GISModel* pModel;	
	GDisplay* dis;
	GScreen* scr;
	GSegment* seg;
	GTextField* field;	
};


