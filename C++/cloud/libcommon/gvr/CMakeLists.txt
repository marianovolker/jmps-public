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

if (NOT TARGET export-gvr)
  add_custom_target(export-gvr)
endif (NOT TARGET export-gvr)

#add_subdirectory(dummy/core)
add_subdirectory(log/core)
add_subdirectory(communication/core)
add_subdirectory(spot/core)
add_subdirectory(ucc/core)

