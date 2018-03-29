#include <gtest/gtest.h>
#include <GMutex.h>
#include <GCondition.h>

class GSystemUT : public testing::Test
{
public:
	GSystemUT();
	virtual ~GSystemUT();

	void SetUp();

	void TearDown();

	bool m_bInit;
	bool m_bDone;
	bool m_bFinish;

	GMutex m_gMutex;
	GCondition m_gCondition;
};
