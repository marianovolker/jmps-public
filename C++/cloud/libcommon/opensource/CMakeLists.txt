##-#######################################################################-##
# Copyright © 2014 Gilbarco Inc.
# Confidential and Proprietary.
##
## @file CMakeLists.txt
## @author pcantarini
## @date Tuesday, April 01, 2014
## @copyright Copyright © 2014 Gilbarco Inc. Confidential and Proprietary.
##-#######################################################################-##

cmake_minimum_required(VERSION 2.8.1 FATAL_ERROR)

if (NOT TARGET export-opensource)
  add_custom_target(export-opensource)
endif (NOT TARGET export-opensource)

add_subdirectory(gtest/core)
add_subdirectory(openssl/core)
add_subdirectory(poco/core)
add_subdirectory(cjson/core)
#add_subdirectory(boost/core)

