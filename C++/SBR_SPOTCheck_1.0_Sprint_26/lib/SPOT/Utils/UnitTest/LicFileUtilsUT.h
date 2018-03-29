/*
 * © 2013 Gilbarco Inc.
 * Confidential and Proprietary
 *
 * GGlobalSystemInformationUT.h
 * Created on: July 30 2013
 * Author: pcrespo
 */

#ifndef GLicFileUtilsUT_h
#define GLicFileUtilsUT_h

#include <gtest/gtest.h>
#include "SPOT/Utils/LicFileUtils.h"


class GLicFileUtilsUT : public testing::Test
{
public:
	GLicFileUtilsUT();
	virtual ~GLicFileUtilsUT();

	static void SetUpTestCase();
	static void TearDownTestCase();

	void SetUp();
	void TearDown();
};

#endif /* GLicFileUtilsUT_h */
