//! Test GSetPersonalityMsgDataXML
/*!
  Test case for class GSetPersonalityMsgDataXML
*/

#include <QtTest/QtTest>
#include <QByteArray>

class TestGSetPersonalityMsgDataXML : public QObject
{
	Q_OBJECT
	
private slots:

	void initTestCase();
	void cleanupTestCase();

	void gSetPersonalityMsgDataUnMarshall();
};
