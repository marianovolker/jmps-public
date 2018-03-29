//! Test GFlyTextMsgDataXML
/*!
  Test case for class GFlyTextMsgDataXML
*/

#include <QtTest/QtTest>
#include <QByteArray>

class TestGFlyTextMsgDataXML : public QObject
{
	Q_OBJECT
	
private slots:

	void initTestCase();
	void cleanupTestCase();

	void gFlyTextMsgDataUnMarshall();
};
