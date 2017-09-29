//! Test GdiRefreshScreenMsgDataXML
/*!
  Test case for class GdiRefreshScreenMsgDataXML
*/

#include <QtTest/QtTest>
#include <QByteArray>

class TestGdiRefreshScreenMsgDataXML : public QObject
{
	Q_OBJECT
	
private slots:

	void initTestCase();
	void cleanupTestCase();

	void gdiRefreshScreenMsgDataUnMarshall();
};
