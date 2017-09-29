//! Test GRequestClassifier
/*!
  Test case for class GRequestClassifier
*/

#include <QtTest/QtTest>
#include <QString>
#include <QStringListIterator>

class TestGRequestClassifier : public QObject
{
	Q_OBJECT
	
private slots:

	void initTestCase();
	void cleanupTestCase();

	void testNotFoundClassifier();
	
	void testClearScreenClasifier();
	void testGdiClearScreen();
	void testGdiDrawLineClassifier();
	void testGdiDrawTextClassifier();
	void testGdiFillRectClassifier();
	void testGdiGetScreenSizeClassifier();
	void testGdiRefreshScreenClassifier();
	void testDownloadGraphicRequestClassifier();
	void testFlyTextClassifier();
	void testInfoScreenRequestClassifier();
	void testKeyEventClassifier();
	void testKioskIdClassifier();
	void testResponseClassifier();
	void testSetPersonalityRequestClassifier();
	void testURLRequestClassifier();
};
