//! Test GClearScreenMsgDataXML 
/*!
  Test case for class GClerScreenMsgDataXML
*/

#include <QtTest/QtTest>
#include <QByteArray>

class TestGClearScreenMsgDataXML : public QObject
{
	Q_OBJECT
	
private slots:

	void initTestCase();
	void cleanupTestCase();

	void gClearScreenMsgDataUnMarshall();
};
