#include <QtTest/QtTest>
#include <QString>
#include "model/src/infoscreenmodel/GISModel.h"
#include "model/src/infoscreenmodel/GDisplay.h"
#include "model/src/infoscreenmodel/GScreen.h"

class TestModel : public QObject
{
	Q_OBJECT
	
private:
	GISModel* pModel;
	
private slots:	
	void initTestCase();
	void cleanupTestCase();

	// Display Testing
	void getScreens();
	void getAllScreens();
	void removeScreens();
	void currentScreen();
};


