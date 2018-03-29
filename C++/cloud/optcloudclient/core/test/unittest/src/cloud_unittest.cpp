/**
 * Copyright © 2014 Gilbarco Inc.
 * Confidential and Proprietary.
 *
 * @file cloud_unittest.cpp
 * @author pcantarini
 * @date Wednesday, September 29, 2014
 * @copyright Copyright © 2014 Gilbarco Inc. Confidential and Proprietary.
 */

#include <gtest/gtest.h>
#include <cloud_unittest.h>

/**
 * @brief Cloud Unittest GCloudTestUT::TestGilbarcoCloud entry point.
 *
 * @since 1.0.0
 */
TEST(GCloudTestUT, TestGilbarcoCloud)
{
  bool cloud(true);

  EXPECT_EQ(cloud,true);
}
