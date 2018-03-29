##-#######################################################################-##
# Copyright © 2014 Gilbarco Inc.
# Confidential and Proprietary.
##
## @file CMakeLists.txt
## @author gparis
## @date Friday, September 12, 2014
## @copyright Copyright © 2014 Gilbarco Inc. Confidential and Proprietary.
##-#######################################################################-##

cmake_minimum_required(VERSION 2.8.1 FATAL_ERROR)

if (CMAKE_CROSSCOMPILING)
  message(STATUS "Skipped '${PROJECT_NAME}' for this cross-compilation.")
  return()
endif (CMAKE_CROSSCOMPILING)

############### SubProject General Information ###############
set(${PROJECT_NAME}_TOOL_NAME gvrcloud_client_registration_tool)

set(${${PROJECT_NAME}_TOOL_NAME}_MAJOR_VERSION 0)
set(${${PROJECT_NAME}_TOOL_NAME}_MINOR_VERSION 0)
set(${${PROJECT_NAME}_TOOL_NAME}_PATCH_VERSION 0)
# set(${${PROJECT_NAME}_TOOL_NAME}_TWEAK_VERSION 0) ##TODO: Modify as needed.
set(${${PROJECT_NAME}_TOOL_NAME}_VERSION_COUNT 3) ## 1 Means "Major" - 2 Means "Major.Minor" - 3 Means "Major.Minor.Patch" - 4 Means "Major.Minor.Patch.Tweak"

set(${${PROJECT_NAME}_TOOL_NAME}_VERSION "${${${PROJECT_NAME}_TOOL_NAME}_MAJOR_VERSION}.${${${PROJECT_NAME}_TOOL_NAME}_MINOR_VERSION}.${${${PROJECT_NAME}_TOOL_NAME}_PATCH_VERSION}")
if (GVR_REVISION)
  set(${${PROJECT_NAME}_TOOL_NAME}_VERSION "${${${PROJECT_NAME}_TOOL_NAME}_VERSION}-r${GVR_REVISION}")
endif (GVR_REVISION)

############### SubProject Temporary Information ###############
set(${${PROJECT_NAME}_TOOL_NAME}_TEMPORARY_LOCATION "${CMAKE_CURRENT_BINARY_DIR}/temporary")

############### SubProject Export Information ###############
set(${${PROJECT_NAME}_TOOL_NAME}_EXPORTABLE_TARGETS)

############### SubProject Install Information ###############
set(${${PROJECT_NAME}_TOOL_NAME}_INSTALLABLE_TARGETS)

############### Project Build Requirements ###############
#gvr_import_package({...} 0.0.0 REQUIRED NO_MODULE)

#SET(CMAKE_THREAD_PREFER_PTHREAD ON)
find_package(Threads REQUIRED)
find_package(OpenSSL REQUIRED)

if (NOT GVR_DISABLE_IMPORT)
  find_package(libboost_3rdparty 1.55.0 NO_MODULE QUIET NO_DEFAULT_PATH PATHS ${GVR_REMOTE_IMPORT_LOCATIONS} PATH_SUFFIXES ${GVR_IMPORT_PATH_SUFFIXES})
endif (NOT GVR_DISABLE_IMPORT)
find_package(libboost_3rdparty 1.55.0 NO_MODULE QUIET NO_DEFAULT_PATH PATHS ${GVR_LOCAL_IMPORT_LOCATIONS})
find_package(libboost_3rdparty 1.55.0 NO_MODULE REQUIRED)

if (NOT OPENSSL_FOUND)
  message(FATAL_ERROR "Package openssl should be imported as an external library.")
endif (NOT OPENSSL_FOUND)

if (NOT libboost_3rdparty_IMPORTED_EXTERNAL)
  message(FATAL_ERROR "Package libboost_3rdparty should be imported as an external library.")
endif (NOT libboost_3rdparty_IMPORTED_EXTERNAL)

############### SubProject Check Version ###############
gvr_check_version(PACKAGE ${${PROJECT_NAME}_TOOL_NAME} VERSION ${${${PROJECT_NAME}_TOOL_NAME}_VERSION}
                  DEPENDS libboost_3rdparty 1.55.0.0gvr ${libboost_3rdparty_VERSION})

############### SubProject Copy Resources Process ###############
file(REMOVE_RECURSE ${CMAKE_CURRENT_BINARY_DIR}/certificates)
file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/misc/certificates
     DESTINATION ${CMAKE_CURRENT_BINARY_DIR}
     DIRECTORY_PERMISSIONS
       OWNER_READ OWNER_WRITE OWNER_EXECUTE
       GROUP_READ GROUP_WRITE GROUP_EXECUTE
       WORLD_READ WORLD_WRITE WORLD_EXECUTE
     FILE_PERMISSIONS 
       OWNER_READ OWNER_WRITE
       GROUP_READ GROUP_WRITE
       WORLD_READ WORLD_WRITE
     NO_SOURCE_PERMISSIONS
     REGEX ".*/\\.svn/.*" EXCLUDE)

file(REMOVE_RECURSE ${CMAKE_CURRENT_BINARY_DIR}/configuration)
file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/misc/configuration
     DESTINATION ${CMAKE_CURRENT_BINARY_DIR}
     DIRECTORY_PERMISSIONS
       OWNER_READ OWNER_WRITE OWNER_EXECUTE
       GROUP_READ GROUP_WRITE GROUP_EXECUTE
       WORLD_READ WORLD_WRITE WORLD_EXECUTE
     FILE_PERMISSIONS 
       OWNER_READ OWNER_WRITE
       GROUP_READ GROUP_WRITE
       WORLD_READ WORLD_WRITE
     NO_SOURCE_PERMISSIONS
     REGEX ".*/\\.svn/.*" EXCLUDE)

############### SubProject Build Process ###############
add_definitions(${GVR_BUILD_FLAGS})

include_directories("${CMAKE_CURRENT_SOURCE_DIR}/include")

#link_directories()

include_directories(${OPENSSL_INCLUDE_DIR})

if (libboost_3rdparty_IMPORTED_EXTERNAL)
  add_definitions(${libboost_3rdparty_IMPORTED_DEFINITIONS})
  include_directories(${libboost_3rdparty_IMPORTED_INCLUDES})
  link_directories(${libboost_3rdparty_IMPORTED_LINK_LOCATIONS})
endif (libboost_3rdparty_IMPORTED_EXTERNAL)

if (NOT TARGET gvrcloud_client_registration_tool)
  set(${${PROJECT_NAME}_TOOL_NAME}_TARGET gvrcloud_client_registration_tool)

  add_executable(${${${PROJECT_NAME}_TOOL_NAME}_TARGET}
                                                        ${CMAKE_CURRENT_SOURCE_DIR}/src/cloud_client_registration_tool.cpp
                                                        ${CMAKE_CURRENT_SOURCE_DIR}/src/CloudClientApplication.cpp
                                                        ${CMAKE_CURRENT_SOURCE_DIR}/src/CloudClientApplicationConfiguration.cpp
                                                        ${CMAKE_CURRENT_SOURCE_DIR}/src/SSLClient.cpp
                )

  set_target_properties(${${${PROJECT_NAME}_TOOL_NAME}_TARGET}
                                                               PROPERTIES
                                                                          VERSION ${${${PROJECT_NAME}_TOOL_NAME}_VERSION})

  target_link_libraries(${${${PROJECT_NAME}_TOOL_NAME}_TARGET}
                                                               ${CMAKE_THREAD_LIBS_INIT}
                                                               ${OPENSSL_LIBRARIES}
                                                               ${libboost_3rdparty_IMPORTED_SHARED_LIBRARY_TARGETS}
                                                               gvrdummy)

  add_dependencies(${${${PROJECT_NAME}_TOOL_NAME}_TARGET}
                                                          ${GVR_CHECK_REVISION}
                                                          ${CMAKE_THREAD_LIBS_INIT}
                                                          ${OPENSSL_LIBRARIES}
                                                          ${libboost_3rdparty_IMPORTED_SHARED_LIBRARY_TARGETS}
                                                          gvrdummy)

  set(${${PROJECT_NAME}_TOOL_NAME}_EXPORTABLE_TARGETS ${${${PROJECT_NAME}_TOOL_NAME}_EXPORTABLE_TARGETS} ${${${PROJECT_NAME}_TOOL_NAME}_TARGET})

  set(${${PROJECT_NAME}_TOOL_NAME}_INSTALLABLE_TARGETS ${${${PROJECT_NAME}_TOOL_NAME}_INSTALLABLE_TARGETS} ${${${PROJECT_NAME}_TOOL_NAME}_TARGET})
endif (NOT TARGET gvrcloud_client_registration_tool)

if (NOT TARGET registration_server_test)
  set(${${PROJECT_NAME}_TOOL_NAME}_TARGET registration_server_test)

  add_executable(${${${PROJECT_NAME}_TOOL_NAME}_TARGET}
                                                        ${CMAKE_CURRENT_SOURCE_DIR}/src/RegistrationServerTest.cpp
                )

  set_target_properties(${${${PROJECT_NAME}_TOOL_NAME}_TARGET}
                                                               PROPERTIES
                                                                          VERSION ${${${PROJECT_NAME}_TOOL_NAME}_VERSION})

  target_link_libraries(${${${PROJECT_NAME}_TOOL_NAME}_TARGET}
                                                               ${CMAKE_THREAD_LIBS_INIT}
                                                               ${OPENSSL_LIBRARIES}
                                                               ${libboost_3rdparty_IMPORTED_SHARED_LIBRARY_TARGETS}
                                                               gvrdummy)

  add_dependencies(${${${PROJECT_NAME}_TOOL_NAME}_TARGET}
                                                          ${GVR_CHECK_REVISION}
                                                          ${CMAKE_THREAD_LIBS_INIT}
                                                          ${OPENSSL_LIBRARIES}
                                                          ${libboost_3rdparty_IMPORTED_SHARED_LIBRARY_TARGETS}
                                                          gvrdummy)

  set(${${PROJECT_NAME}_TOOL_NAME}_EXPORTABLE_TARGETS ${${${PROJECT_NAME}_TOOL_NAME}_EXPORTABLE_TARGETS} ${${${PROJECT_NAME}_TOOL_NAME}_TARGET})

  set(${${PROJECT_NAME}_TOOL_NAME}_INSTALLABLE_TARGETS ${${${PROJECT_NAME}_TOOL_NAME}_INSTALLABLE_TARGETS} ${${${PROJECT_NAME}_TOOL_NAME}_TARGET})
endif (NOT TARGET registration_server_test)

############### SubProject Export Process ###############
if (${${PROJECT_NAME}_TOOL_NAME}_EXPORTABLE_TARGETS)
  gvr_export( NAME "${PROJECT_NAME}"
              DESTINATION_LOCATION "${GVR_LOCAL_EXPORT_LOCATION}"
              TARGETS ${${${PROJECT_NAME}_TOOL_NAME}_EXPORTABLE_TARGETS}
              APPEND)
endif (${${PROJECT_NAME}_TOOL_NAME}_EXPORTABLE_TARGETS)

############### SubProject Install Process ###############
if (${${PROJECT_NAME}_TOOL_NAME}_INSTALLABLE_TARGETS)
  install(TARGETS ${${${PROJECT_NAME}_TOOL_NAME}_INSTALLABLE_TARGETS}
                  RUNTIME DESTINATION "${${PROJECT_NAME}_INSTALL_RELATIVE_LOCATION}/bin"                                            COMPONENT Runtime
                  LIBRARY DESTINATION "${${PROJECT_NAME}_INSTALL_RELATIVE_LOCATION}/lib"                                            COMPONENT Runtime
                  ARCHIVE DESTINATION "${${PROJECT_NAME}_INSTALL_RELATIVE_LOCATION}/lib/${PROJECT_NAME}-${${PROJECT_NAME}_VERSION}" COMPONENT Development)
endif (${${PROJECT_NAME}_TOOL_NAME}_INSTALLABLE_TARGETS)
