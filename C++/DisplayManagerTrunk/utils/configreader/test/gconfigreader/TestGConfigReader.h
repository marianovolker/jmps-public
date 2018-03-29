//! Test GconfigReader
/*!
  Test case for class GconfigReader
 */

#include <QtTest/QtTest>
#include <QString>
#include "../../src/GConfigReader.h"

class TestGConfigReader : public QObject
{
	Q_OBJECT

public:
	void CheckFile(const char* pszFileName, int iLoopsExecuted, int iEntriesPerLoop);

private slots:

	void initTestCase();
	void cleanupTestCase();

	void int0Test();
	void int1Test();
	void string0Test();
	void string1Test();
	void bool0Test();
	void bool1Test();
	void setTest();
	void saveTest();

private:
	GConfigReader *m_poConfigReader;

};
