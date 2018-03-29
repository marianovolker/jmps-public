/**
 * Copyright © 2014 Gilbarco Inc.
 * Confidential and Proprietary.
 *
 * @file main.cpp
 * @author pcantarini
 * @date Monday, April 28, 2014
 * @copyright Copyright © 2014 Gilbarco Inc. Confidential and Proprietary.
 */
#include <gtest/gtest.h>

/**
 * @brief      Communication Unittest Main entry point.
 * @param [in] argc Amount of arguments passed to the communication tool command line.
 * @param [in] argv List of arguments passed to the communication tool command line.
 * @return     Command line expected result code. Communication Unittest always return what the Google Test framework resolves.
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
