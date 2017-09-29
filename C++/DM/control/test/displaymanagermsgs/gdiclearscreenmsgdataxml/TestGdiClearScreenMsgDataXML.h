//! Test GdiClearScreenMsgDataXML
/*!
  Test case for class GClerScreenMsgDataXML
*/

#include <QtTest/QtTest>
#include <QByteArray>

class TestGdiClearScreenMsgDataXML : public QObject
{
	Q_OBJECT
	
private slots:

	void initTestCase();
	void cleanupTestCase();

	void gdiClearScreenMsgDataUnMarshall();
};
