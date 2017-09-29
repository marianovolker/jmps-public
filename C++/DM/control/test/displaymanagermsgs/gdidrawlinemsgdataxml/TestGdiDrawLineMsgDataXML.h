//! Test GdiDrawLineMsgDataXML
/*!
  Test case for class GdiDrawLineMsgDataXML
*/

#include <QtTest/QtTest>
#include <QByteArray>

class TestGdiDrawLineMsgDataXML : public QObject
{
	Q_OBJECT
	
private slots:

	void initTestCase();
	void cleanupTestCase();

	void gdiDrawLineMsgDataUnMarshall();
};
