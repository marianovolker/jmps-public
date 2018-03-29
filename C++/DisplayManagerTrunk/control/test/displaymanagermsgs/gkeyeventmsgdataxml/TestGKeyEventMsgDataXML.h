//! Test GKeyEventMsgDataXML
/*!
  Test case for class GKeyEventMsgDataXML
*/

#include <QtTest/QtTest>
#include <QByteArray>

class TestGKeyEventMsgDataXML : public QObject
{
	Q_OBJECT
	
private slots:

	void initTestCase();
	void cleanupTestCase();

	void gKeyEventMsgDataUnMarshall();
};
