//! Test GResponseMsgDataXML
/*!
  Test case for class GResponseMsgDataXML
*/

#include <QtTest/QtTest>
#include <QByteArray>

class TestGResponseMsgDataXML : public QObject
{
	Q_OBJECT
	
private slots:

	void initTestCase();
	void cleanupTestCase();

	void gResponseMsgDataMarshall();
	void gResponseMsgDataUnMarshall();
};
