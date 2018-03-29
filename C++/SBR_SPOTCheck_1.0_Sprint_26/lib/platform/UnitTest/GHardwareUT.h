// GHardwareUT.h

#ifndef GHardwareUT_h
#define GHardwareUT_h

#include <gtest/gtest.h>


class GHardwareUT : public testing::Test
{
public:
	GHardwareUT();
	virtual ~GHardwareUT();

	void SetUp();
	void TearDown();
};

#endif
