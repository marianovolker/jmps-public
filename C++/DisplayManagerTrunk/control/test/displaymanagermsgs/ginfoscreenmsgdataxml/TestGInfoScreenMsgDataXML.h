//! Test GInfoScreenMsgDataXML
/*!
  Test case for class GInfoScreenMsgDataXML
*/

#include <QtTest/QtTest>
#include <QByteArray>

class TestGInfoScreenMsgDataXML : public QObject
{
	Q_OBJECT
	
private slots:

	void initTestCase();
	void cleanupTestCase();

	void gInfoScreenMsgDataUnMarshall();
};
