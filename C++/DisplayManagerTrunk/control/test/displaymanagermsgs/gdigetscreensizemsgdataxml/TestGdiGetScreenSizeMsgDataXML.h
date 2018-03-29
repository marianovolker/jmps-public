//! Test GdiGetScreenSizeMsgDataXML
/*!
  Test case for class GdiGetScreenSizeMsgDataXML
*/

#include <QtTest/QtTest>
#include <QByteArray>

class TestGdiGetScreenSizeMsgDataXML: public QObject
{
	Q_OBJECT
	
private slots:

	void initTestCase();
	void cleanupTestCase();

	void gdiGetScreenSizeMsgDataUnMarshall();
};
