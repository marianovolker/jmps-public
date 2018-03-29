/*
 * © 2013 Gilbarco Inc.
 * Confidential and Proprietary
 *
 * @file GSelfDeleteUT.h
 * @author Sayyed Mohammad
 * @date March 13 2014
 * @copyright © 2014 Gilbarco Inc. Confidential and Propietary
 */

#ifndef GSelfDeleteUT_h
#define GSelfDeleteUT_h

#include <gtest/gtest.h>


class GSelfDeleteUT : public testing::Test
{
public:
	GSelfDeleteUT();
	~GSelfDeleteUT();

	void SetUp();
	void TearDown();

};

#endif /* GSelfDeleteUT_h */
