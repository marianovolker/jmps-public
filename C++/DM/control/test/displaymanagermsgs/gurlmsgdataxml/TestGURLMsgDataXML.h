//! Test GURLMsgDataXML
/*!
  Test case for class GURLMsgDataXML
*/

#include <QtTest/QtTest>
#include <QByteArray>

class TestGURLMsgDataXML : public QObject
{
	Q_OBJECT
	
private slots:

	void initTestCase();
	void cleanupTestCase();

	void GURLMsgDataUnMarshall();
};
