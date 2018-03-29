#include <gtest/gtest.h>

class GMutex;

class GMutexUT : public testing::Test
{
public:
	GMutexUT();
	virtual ~GMutexUT();

	void SetUp();

	void TearDown();

	unsigned long m_ulCounter;

	GMutex* m_pMutex;
};
