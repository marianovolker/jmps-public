//! Test GdiDrawTextRequest
/*!
  Test case for class GdiDrawTextRequest
*/

#include <QtTest/QtTest>
#include <QByteArray>

class TestGdiDrawTextMsgDataXML : public QObject
{
	Q_OBJECT
	
private slots:

	void initTestCase();
	void cleanupTestCase();

	void gdiDrawTextMsgDataUnMarshall();
};
