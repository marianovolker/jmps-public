#include <gtest/gtest.h>

class GSemaphore;
class GMutex;

class GSemaphoreUT : public testing::Test
{
public:
	static const int CSemCount;
	GSemaphoreUT();
	virtual ~GSemaphoreUT();

	void SetUp();

	void TearDown();

	GMutex * m_pMutex;
	GSemaphore * m_pSem;

	GMutex* m_pCounterLock;
	unsigned long m_ulCounter;

	long m_lTimeout;
};
