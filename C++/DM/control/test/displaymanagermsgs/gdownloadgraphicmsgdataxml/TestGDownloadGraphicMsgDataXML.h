//! Test GDownloadGraphicMsgDataXML
/*!
  Test case for class GDownloadGraphicMsgDataXML
*/

#include <QtTest/QtTest>
#include <QByteArray>

class TestGDownloadGraphicMsgDataXML : public QObject
{
	Q_OBJECT
	
private slots:

	void initTestCase();
	void cleanupTestCase();

	void gDownloadGraphicMsgDataUnMarshall();
};
