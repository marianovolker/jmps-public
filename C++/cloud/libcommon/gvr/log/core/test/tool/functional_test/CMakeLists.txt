##-#######################################################################-##
# Copyright © 2014 Gilbarco Inc.
# Confidential and Proprietary.
##
## @file CMakeLists.txt
## @author gparis
## @date Tuesday, September 30, 2014
## @copyright Copyright © 2014 Gilbarco Inc. Confidential and Proprietary.
##-#######################################################################-##

cmake_minimum_required(VERSION 2.8.1 FATAL_ERROR)

############### SubProject General Information ###############
set(${PROJECT_NAME}_TOOL_NAME gvrlog_tool)

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

############### SubProject Check Version ###############
gvr_check_version(PACKAGE ${${PROJECT_NAME}_TOOL_NAME} VERSION ${${${PROJECT_NAME}_TOOL_NAME}_VERSION})

############### SubProject Copy Resources Process ###############
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

# log directory is freshly created into the binaries one
file(REMOVE_RECURSE ${CMAKE_CURRENT_BINARY_DIR}/log)
file(MAKE_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/log)

############### SubProject Build Process ###############
add_definitions(${GVR_BUILD_FLAGS})

include_directories("${CMAKE_CURRENT_SOURCE_DIR}/include")

#link_directories()

if (NOT TARGET gvrlog_tool)
  set(${${PROJECT_NAME}_TOOL_NAME}_TARGET gvrlog_tool)

  add_executable(${${${PROJECT_NAME}_TOOL_NAME}_TARGET}
                                                        "${CMAKE_CURRENT_SOURCE_DIR}/src/log_tool.cpp")

  set_target_properties(${${${PROJECT_NAME}_TOOL_NAME}_TARGET}
                                                               PROPERTIES
                                                                          VERSION ${${${PROJECT_NAME}_TOOL_NAME}_VERSION})

  # Need to add 'd' at the end of the library name if build for debug.
  set(${PROJECT_NAME}_POCO_LIBS PocoUtil PocoFoundation)
  if (GVR_DEBUG)
    set(${PROJECT_NAME}_POCO_LIBS_AUX ${${PROJECT_NAME}_POCO_LIBS})
    set(${PROJECT_NAME}_POCO_LIBS "")
    foreach (${PROJECT_NAME}_POCO_LIB ${${PROJECT_NAME}_POCO_LIBS_AUX})
      set(${PROJECT_NAME}_POCO_LIBS ${${PROJECT_NAME}_POCO_LIBS} "${${PROJECT_NAME}_POCO_LIB}d")
    endforeach (${PROJECT_NAME}_POCO_LIB)
    set(${PROJECT_NAME}_POCO_LIBS_AUX "")
  endif (GVR_DEBUG)

  target_link_libraries(${${${PROJECT_NAME}_TOOL_NAME}_TARGET}
                                                               ${${PROJECT_NAME}_POCO_LIBS}
                                                               gvrlog
                                                               ${libpoco_3rdparty_OPTIONAL_DYNAMIC_LINK_FLAGS}
                                                               ${CMAKE_THREAD_LIBS_INIT})

if (GVR_CHECK_REVISION)
  add_dependencies(${${${PROJECT_NAME}_TOOL_NAME}_TARGET}
                                                          ${GVR_CHECK_REVISION}
                                                          ${${PROJECT_NAME}_POCO_LIBS}
                                                          gvrlog
                                                          ${libpoco_3rdparty_OPTIONAL_DYNAMIC_LINK_FLAGS}
                                                          ${CMAKE_THREAD_LIBS_INIT})
endif (GVR_CHECK_REVISION)


  set(${${PROJECT_NAME}_TOOL_NAME}_EXPORTABLE_TARGETS ${${${PROJECT_NAME}_TOOL_NAME}_EXPORTABLE_TARGETS} ${${${PROJECT_NAME}_TOOL_NAME}_TARGET})

  set(${${PROJECT_NAME}_TOOL_NAME}_INSTALLABLE_TARGETS ${${${PROJECT_NAME}_TOOL_NAME}_INSTALLABLE_TARGETS} ${${${PROJECT_NAME}_TOOL_NAME}_TARGET})
endif (NOT TARGET gvrlog_tool)

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
