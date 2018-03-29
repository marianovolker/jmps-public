/**
 * Copyright © 2015 Gilbarco Inc.
 * Confidential and Proprietary.
 *
 * @file main.cpp
 * @author pcantarini
 * @date Monday, Mars 02, 2015
 * @copyright Copyright © 2015 Gilbarco Inc. Confidential and Proprietary.
 */
#include <gtest/gtest.h>

/**
 * @brief      Ucc Unittest Main entry point.
 * @param [in] argc Amount of arguments passed to the cloud tool command line.
 * @param [in] argv List of arguments passed to the cloud tool command line.
 * @return     Command line expected result code. Cloud Unittest always return what the Google Test framework resolves.
 *
 * @since 1.0.0
 */
int main(int argc, char **argv)
{
	// Initialize the testing framework
	testing::InitGoogleTest(&argc, argv);

	// Run the tests
	return RUN_ALL_TESTS();
}
