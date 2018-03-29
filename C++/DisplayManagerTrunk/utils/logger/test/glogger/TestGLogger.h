//! Test GLogger
/*!
  Test case for class GLogger
*/

#include <QtTest/QtTest>
#include <QString>
#include <QRegExp>
#include "../../src/GLogger.h"
#include "../../src/GLogConfig.h"
#include "../../src/GLogManager.h"


class TestGLogger : public QObject
{
	Q_OBJECT
	
public:
	void CheckFile(const char* pszFileName, int iLoopsExecuted, int iEntriesPerLoop);
	
private slots:	

	void initTestCase();
	void cleanupTestCase();
	
	void WriteLevel1();
	void WriteLevel2();
	void WriteLevel3();
	void WriteLevel4();
	
private:
	QRegExp* oRegExp;
};
