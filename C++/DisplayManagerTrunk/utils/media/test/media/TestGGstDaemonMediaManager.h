//! Test GconfigReader
/*!
  Test case for class GconfigReader
 */

#include <QtTest/QtTest>
#include <QString>
#include "../../src/GGstDaemonMediaManager.h"

class TestGGstDaemonMediaManager : public QObject
{
	Q_OBJECT

public:
	void CheckFile(const char* pszFileName, int iLoopsExecuted, int iEntriesPerLoop);

private slots:

	void initTestCase();
	void cleanupTestCase();

	void getInstanceTest();

private:
	QSharedPointer<GGstDaemonMediaManager> m_poGstDaemonMediaManager;

};
