//! Test GcharMapping
/*!
  Test case for class GcharMapping
 */

#include <QtTest/QtTest>
#include <QString>
#include "../../src/GCharMapping.h"

class TestGCharMapping : public QObject
{
	Q_OBJECT

public:
	void CheckFile(const char* pszFileName, int iLoopsExecuted, int iEntriesPerLoop);

private slots:

	void initTestCase();
	void cleanupTestCase();

	void char000Test();
	void char001Test();
	void char002Test();
	void char003Test();
	void char004Test();

private:
	GCharMapping *m_poGCharMapping;

};
