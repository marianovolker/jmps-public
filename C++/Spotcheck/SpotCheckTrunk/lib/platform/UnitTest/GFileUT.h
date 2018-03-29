// GFileUT.h

#ifndef GFileUT_h
#define GFileUT_h

#include <gtest/gtest.h>


class GFileUT : public testing::Test
{
public:
	GFileUT();
	virtual ~GFileUT();

	void SetUp();
	void TearDown();
};

#endif
