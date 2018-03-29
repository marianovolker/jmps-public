#include <gtest/gtest.h>

class GMutex;
class GCondition;

class GConditionUT : public testing::Test
{
public:
	GConditionUT();
	virtual ~GConditionUT();

	void SetUp();

	void TearDown();

	unsigned long m_ulCounter;
	bool m_bDone;

	GMutex * m_pMutex;
	GCondition * m_pCondition;
};
