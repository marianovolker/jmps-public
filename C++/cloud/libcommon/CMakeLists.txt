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

if (NOT DEFINED GVR_MODULE_PATH)
  set(GVR_MODULE_PATH "${CMAKE_SOURCE_DIR}/build/cmake/module")
  file(TO_CMAKE_PATH ${GVR_MODULE_PATH} GVR_MODULE_PATH)
endif (NOT DEFINED GVR_MODULE_PATH)

if (GVR_MODULE_PATH)
  include("${GVR_MODULE_PATH}/gvr/GvrFoundation.cmake")
endif (GVR_MODULE_PATH)
if (NOT DEFINED GVR_FOUNDATION_INCLUDED)
  message(FATAL_ERROR "GvrFoundation not found.")
endif (NOT DEFINED GVR_FOUNDATION_INCLUDED)

if (GVR_RUN_TEST)
  enable_testing()
endif (GVR_RUN_TEST)

if (NOT TARGET export)
  add_custom_target(export)
endif (NOT TARGET export)

add_subdirectory(opensource)
add_subdirectory(gvr)
