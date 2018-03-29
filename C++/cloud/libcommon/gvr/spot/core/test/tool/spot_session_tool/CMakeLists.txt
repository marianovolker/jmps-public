##-#######################################################################-##
# Copyright © 2014 Gilbarco Inc.
# Confidential and Proprietary.
##
## @file CMakeLists.txt
## @author mgrieco
## @date Friday, December 12, 2014
## @copyright Copyright © 2014 Gilbarco Inc. Confidential and Proprietary.
##-#######################################################################-##

cmake_minimum_required(VERSION 2.8.1 FATAL_ERROR)

############### SubProject General Information ###############
set(${PROJECT_NAME}_TOOL_NAME gvrspot_session_tool)

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

############### SubProject Check Version ###############
gvr_check_version(PACKAGE ${${PROJECT_NAME}_TOOL_NAME} VERSION ${${${PROJECT_NAME}_TOOL_NAME}_VERSION})

############### SubProject Build Process ###############
add_definitions(${GVR_BUILD_FLAGS})

#include_directories("${CMAKE_CURRENT_SOURCE_DIR}/include")

#link_directories()

if (NOT TARGET gvrspot_session_tool)
  set(${${PROJECT_NAME}_TOOL_NAME}_TARGET gvrspot_session_tool)

  add_executable(${${${PROJECT_NAME}_TOOL_NAME}_TARGET}
                                                        "${CMAKE_CURRENT_SOURCE_DIR}/src/spot_session_tool.cpp"
                                                        )

  set_target_properties(${${${PROJECT_NAME}_TOOL_NAME}_TARGET}
                                                               PROPERTIES
                                                                          VERSION ${${${PROJECT_NAME}_TOOL_NAME}_VERSION})

  target_link_libraries(${${${PROJECT_NAME}_TOOL_NAME}_TARGET}
                                                               gvrcommunication
                                                               gvrspot
                                                               ${libpoco_3rdparty_OPTIONAL_DYNAMIC_LINK_FLAGS}
                                                               ${CMAKE_THREAD_LIBS_INIT})

if (GVR_CHECK_REVISION)
  add_dependencies(${${${PROJECT_NAME}_TOOL_NAME}_TARGET}
                                                          ${GVR_CHECK_REVISION}
                                                          gvrcommunication
                                                          gvrspot
                                                          ${libpoco_3rdparty_OPTIONAL_DYNAMIC_LINK_FLAGS}
                                                          ${CMAKE_THREAD_LIBS_INIT})
endif (GVR_CHECK_REVISION)

  set(${${PROJECT_NAME}_TOOL_NAME}_EXPORTABLE_TARGETS ${${${PROJECT_NAME}_TOOL_NAME}_EXPORTABLE_TARGETS} ${${${PROJECT_NAME}_TOOL_NAME}_TARGET})

  set(${${PROJECT_NAME}_TOOL_NAME}_INSTALLABLE_TARGETS ${${${PROJECT_NAME}_TOOL_NAME}_INSTALLABLE_TARGETS} ${${${PROJECT_NAME}_TOOL_NAME}_TARGET})
endif (NOT TARGET gvrspot_session_tool)


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
