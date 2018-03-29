// GTaskUT.h

#ifndef GTaskUT_h
#define GTaskUT_h

#include <gtest/gtest.h>


class GTaskUT : public testing::Test
{
public:
	GTaskUT();
	virtual ~GTaskUT();

	void SetUp();
	void TearDown();
};

#endif
