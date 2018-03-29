/**
 * Copyright © 2014 Gilbarco Inc.
 * Confidential and Proprietary.
 *
 * @file dummy_unittest.cpp
 * @author pcantarini
 * @date Monday, April 28, 2014
 * @copyright Copyright © 2014 Gilbarco Inc. Confidential and Proprietary.
 */
#include <gtest/gtest.h>
#include "../include/gvrdummy_unittest/dummy_unittest.h"

/**
 * @brief      Dummy Unittest GDummyTestUT::TestGilbarcoDummy entry point.
 *
 * @since 1.0.0
 */
TEST(GDummyTestUT, TestGilbarcoDummy)
{
  bool dummy(true);

  std::cout << std::endl;
  std::cout << "             "; // left margin to gracefully insert this into gtest output.
  gvr::dummy::do_dummy();
  std::cout << std::endl;

  EXPECT_EQ(dummy,true);
}
