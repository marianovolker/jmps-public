##-#######################################################################-##
# Copyright © 2014 Gilbarco Inc.
# Confidential and Proprietary.
##
## @file CMakeLists.txt
## @author pcantarini
## @date Wednesday, September 29, 2014
## @copyright Copyright © 2014 Gilbarco Inc. Confidential and Proprietary.
##-#######################################################################-##

cmake_minimum_required(VERSION 2.8.1 FATAL_ERROR)

############### Project Unittest General Information ###############
set(${PROJECT_NAME}_UNITTEST_NAME gvrcloud_unittest)

if (CMAKE_CROSSCOMPILING)
  message(STATUS "Skipped '${${PROJECT_NAME}_UNITTEST_NAME}' for this cross-compilation.")
  return()
endif (CMAKE_CROSSCOMPILING)

set(${${PROJECT_NAME}_UNITTEST_NAME}_MAJOR_VERSION 0)
set(${${PROJECT_NAME}_UNITTEST_NAME}_MINOR_VERSION 0)
set(${${PROJECT_NAME}_UNITTEST_NAME}_PATCH_VERSION 0)
# set(${${PROJECT_NAME}_UNITTEST_NAME}_TWEAK_VERSION 0) ##TODO: Modify as needed.
set(${${PROJECT_NAME}_UNITTEST_NAME}_VERSION_COUNT 3) ## 1 Means "Major" - 2 Means "Major.Minor" - 3 Means "Major.Minor.Patch" - 4 Means "Major.Minor.Patch.Tweak"

set(${${PROJECT_NAME}_UNITTEST_NAME}_VERSION "${${${PROJECT_NAME}_UNITTEST_NAME}_MAJOR_VERSION}.${${${PROJECT_NAME}_UNITTEST_NAME}_MINOR_VERSION}.${${${PROJECT_NAME}_UNITTEST_NAME}_PATCH_VERSION}")
if (GVR_REVISION)
  set(${${PROJECT_NAME}_UNITTEST_NAME}_VERSION "${${${PROJECT_NAME}_UNITTEST_NAME}_VERSION}-r${GVR_REVISION}")
endif (GVR_REVISION)

############### Project Unittest Temporary Information ###############
set(${${PROJECT_NAME}_UNITTEST_NAME}_TEMPORARY_LOCATION "${CMAKE_CURRENT_BINARY_DIR}/temporary")

############### Project UnitTest Virtual Machine Information ###############
set(${${PROJECT_NAME}_UNITTEST_NAME}_TEST_VIRTUAL_MACHINE "")
if (GVR_RUN_VALGRIND_TEST AND GVR_VALGRIND_COMMAND)
  set(${${PROJECT_NAME}_UNITTEST_NAME}_TEST_VIRTUAL_MACHINE ${GVR_VALGRIND_COMMAND} ${GVR_VALGRIND_DEFAULT_ARGUMENTS})
endif (GVR_RUN_VALGRIND_TEST AND GVR_VALGRIND_COMMAND)

############### Project UnitTest Build Requirements ###############
####################################################
## Prioritized import locations for a new package ##
#if (NOT GVR_DISABLE_IMPORT)
#  find_package({...} 0.0.0 NO_MODULE QUIET NO_DEFAULT_PATH PATHS ${GVR_REMOTE_IMPORT_LOCATIONS} PATH_SUFFIXES import)
#endif (NOT GVR_DISABLE_IMPORT)
#find_package({...} 0.0.0 NO_MODULE QUIET NO_DEFAULT_PATH PATHS ${GVR_LOCAL_IMPORT_LOCATIONS})
#find_package({...} 0.0.0 NO_MODULE REQUIRED)
####################################################

#SET(CMAKE_THREAD_PREFER_PTHREAD ON)
find_package(Threads REQUIRED)

if (NOT GVR_DISABLE_IMPORT)
  find_package(libgtest_3rdparty 1.7.0 NO_MODULE QUIET NO_DEFAULT_PATH PATHS ${GVR_REMOTE_IMPORT_LOCATIONS} PATH_SUFFIXES ${GVR_IMPORT_PATH_SUFFIXES})
endif (NOT GVR_DISABLE_IMPORT)
find_package(libgtest_3rdparty 1.7.0 NO_MODULE QUIET NO_DEFAULT_PATH PATHS ${GVR_LOCAL_IMPORT_LOCATIONS})
find_package(libgtest_3rdparty 1.7.0 NO_MODULE REQUIRED)

if (NOT libgtest_3rdparty_IMPORTED_EXTERNAL)
  message(FATAL_ERROR "Package libgtest_3rdparty should be imported as an external library.")
endif (NOT libgtest_3rdparty_IMPORTED_EXTERNAL)

############### Project UnitTest Check Version ###############
gvr_check_version(PACKAGE ${${PROJECT_NAME}_UNITTEST_NAME} VERSION ${${${PROJECT_NAME}_UNITTEST_NAME}_VERSION}
                  DEPENDS libgtest_3rdparty 1.7.0 ${libgtest_3rdparty_VERSION})

############### Unittest Build Process ###############
add_definitions(${GVR_BUILD_FLAGS})

include_directories("${CMAKE_CURRENT_SOURCE_DIR}/include")

#link_directories()

if (libgtest_3rdparty_IMPORTED_EXTERNAL)
  add_definitions(${libgtest_3rdparty_IMPORTED_DEFINITIONS})
  include_directories(${libgtest_3rdparty_IMPORTED_INCLUDES})
  link_directories(${libgtest_3rdparty_IMPORTED_LINK_LOCATIONS})
endif (libgtest_3rdparty_IMPORTED_EXTERNAL)

if (NOT TARGET gvrcloud_unittest)
  set(${${PROJECT_NAME}_UNITTEST_NAME}_TARGET gvrcloud_unittest)

  add_executable(${${${PROJECT_NAME}_UNITTEST_NAME}_TARGET}
                                                            "${CMAKE_CURRENT_SOURCE_DIR}/src/main.cpp"
                                                            "${CMAKE_CURRENT_SOURCE_DIR}/src/cloud_unittest.cpp")

  set_target_properties(${${${PROJECT_NAME}_UNITTEST_NAME}_TARGET}
                                                                   PROPERTIES
                                                                              VERSION ${${${PROJECT_NAME}_UNITTEST_NAME}_VERSION})

  # Need to add 'd' at the end of the library name if build for debug.
  set(${PROJECT_NAME}_POCO_LIBS PocoNetSSL PocoCrypto PocoNet PocoUtil PocoXML PocoZip PocoFoundation)
  if (GVR_DEBUG)
    set(${PROJECT_NAME}_POCO_LIBS_AUX ${${PROJECT_NAME}_POCO_LIBS})
    set(${PROJECT_NAME}_POCO_LIBS "")
    foreach (${PROJECT_NAME}_POCO_LIB ${${PROJECT_NAME}_POCO_LIBS_AUX})
      set(${PROJECT_NAME}_POCO_LIBS ${${PROJECT_NAME}_POCO_LIBS} "${${PROJECT_NAME}_POCO_LIB}d")
    endforeach (${PROJECT_NAME}_POCO_LIB)
    set(${PROJECT_NAME}_POCO_LIBS_AUX "")
  endif (GVR_DEBUG)

  target_link_libraries(${${${PROJECT_NAME}_UNITTEST_NAME}_TARGET}
                                                                   gvrcloudcore gvrucc gvrspot gvrcommunication gvrlog
                                                                   ${${PROJECT_NAME}_POCO_LIBS}
                                                                   cjson
                                                                   ssl crypto
                                                                   ${libgtest_3rdparty_IMPORTED_STATIC_LIBRARY_TARGETS}
                                                                   ${libpoco_3rdparty_OPTIONAL_DYNAMIC_LINK_FLAGS}
                                                                   ${CMAKE_THREAD_LIBS_INIT})

  add_dependencies(${${${PROJECT_NAME}_UNITTEST_NAME}_TARGET}
                                                              ${GVR_CHECK_REVISION}
                                                              gvrcloudcore gvrucc gvrspot gvrcommunication gvrlog
                                                              ${${PROJECT_NAME}_POCO_LIBS}
                                                              cjson
                                                              ssl crypto
                                                              ${libgtest_3rdparty_IMPORTED_STATIC_LIBRARY_TARGETS}
                                                              ${libpoco_3rdparty_OPTIONAL_DYNAMIC_LINK_FLAGS}
                                                              ${CMAKE_THREAD_LIBS_INIT})

  set(${${${PROJECT_NAME}_UNITTEST_NAME}_TARGET}_TEMPORARY_LOCATION "${${${PROJECT_NAME}_UNITTEST_NAME}_TEMPORARY_LOCATION}/${${${PROJECT_NAME}_UNITTEST_NAME}_TARGET}")
  file(REMOVE_RECURSE ${${${${PROJECT_NAME}_UNITTEST_NAME}_TARGET}_TEMPORARY_LOCATION})
  file(MAKE_DIRECTORY ${${${${PROJECT_NAME}_UNITTEST_NAME}_TARGET}_TEMPORARY_LOCATION})

  add_test(NAME ${${${PROJECT_NAME}_UNITTEST_NAME}_TARGET}
# WARNING: WORKING_DIRECTORY not implemented for add_test in cmake 2.8.1
#                                                           WORKING_DIRECTORY ${${${${PROJECT_NAME}_UNITTEST_NAME}_TARGET}_TEMPORARY_LOCATION}}
                                                           COMMAND ${${${PROJECT_NAME}_UNITTEST_NAME}_TEST_VIRTUAL_MACHINE} "$<TARGET_FILE:${${${PROJECT_NAME}_UNITTEST_NAME}_TARGET}>")
endif (NOT TARGET gvrcloud_unittest)
