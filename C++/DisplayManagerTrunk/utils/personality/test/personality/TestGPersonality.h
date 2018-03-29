//! Test GPersonality
/*!
  Test case for class GPersonality
*/

#include <QtTest/QtTest>

class TestGPersonality : public QObject
{
	Q_OBJECT
	
private slots:

	void initTestCase();
	void cleanupTestCase();

	void GPersonalityDisplayManagerPropertiesFileInit();
	void GPersonality320x240Init();
	void GPersonality640x480Init();
	void GPersonalityFallbackInit();
};
