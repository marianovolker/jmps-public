//! Test GConfigEnvironment
/*!
  Test case for class GConfigEnvironment
 */

#include <QtTest/QtTest>
#include <QString>

class TestGConfigEnvironment : public QObject
{
	Q_OBJECT

private slots:

	void initTestCase();
	void cleanupTestCase();

	void checkEnvVarTest();
};
