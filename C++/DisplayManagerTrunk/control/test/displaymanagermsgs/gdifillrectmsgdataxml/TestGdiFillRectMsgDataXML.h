//! Test GdiFillRectMsgDataXML
/*!
  Test case for class GdiFillRectMsgDataXML
*/

#include <QtTest/QtTest>
#include <QByteArray>

class TestGdiFillRectMsgDataXML : public QObject
{
	Q_OBJECT
	
private slots:

	void initTestCase();
	void cleanupTestCase();

	void gdiFillRectMsgDataUnMarshall();
};
