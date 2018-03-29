/*
 * © 2013 Gilbarco Inc.
 * Confidential and Proprietary
 *
 * @file GConfig.h
 * @author Mariano Volker
 * @date Oct 21 2013
 * @copyright © 2013 Gilbarco Inc. Confidential and Propietary
 */

#ifndef GConfigUT_h
#define GConfigUT_h

#include <gtest/gtest.h>


class GConfigUT : public testing::Test
{
public:
	GConfigUT();
	virtual ~GConfigUT();

	void SetUp();
	void TearDown();

};

#endif
