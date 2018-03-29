//! Test GKioskIdMsgDataXML
/*!
  Test case for class GKioskIdMsgDataXML
*/

#include <QtTest/QtTest>
#include <QByteArray>

class TestGKioskIdMsgDataXML : public QObject
{
	Q_OBJECT
	
private slots:

	void initTestCase();
	void cleanupTestCase();

	void gKioskIdMsgDataUnMarshall();
};
