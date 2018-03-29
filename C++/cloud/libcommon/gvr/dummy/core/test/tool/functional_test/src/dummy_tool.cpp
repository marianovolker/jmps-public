/**
 * Copyright © 2014 Gilbarco Inc.
 * Confidential and Proprietary.
 *
 * @file dummy_tool.cpp
 * @author pcantarini
 * @date Tuesday, April 01, 2014
 * @copyright Copyright © 2014 Gilbarco Inc. Confidential and Proprietary.
 */

#include "../include/gvrdummy_tool/dummy_tool.h"

/**
 * @brief      Dummy Tool Main entry point.
 * @param [in] argc Amount of arguments passed to the dummy tool command line.
 * @param [in] argv List of arguments passed to the dummy tool command line.
 * @return     Command line expected result code. Dummy Tool always return 0, which means ended successfully.
 *
 * @since 1.0.0
 */
int main (int argc, char **argv)
{
  gvr::dummy::do_dummy();
  return 0;
}
