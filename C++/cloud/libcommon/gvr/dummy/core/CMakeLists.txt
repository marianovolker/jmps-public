##-#######################################################################-##
# Copyright © 2014, 2015 Gilbarco Inc.
# Confidential and Proprietary.
##
## @file CMakeLists.txt
## @author pcantarini
## @date Tuesday, April 01, 2014
## @copyright Copyright © 2014, 2015 Gilbarco Inc. Confidential and Proprietary.
##-#######################################################################-##

cmake_minimum_required(VERSION 2.8.1 FATAL_ERROR)

if (NOT DEFINED GVR_MODULE_PATH)
  message(FATAL_ERROR "GVR_MODULE_PATH not defined.")
endif (NOT DEFINED GVR_MODULE_PATH)

############### Project Definition ###############
project(libgvrdummy)

if (GVR_MODULE_PATH)
  include("${GVR_MODULE_PATH}/gvr/GvrFoundation.cmake")
endif (GVR_MODULE_PATH)
if (NOT DEFINED GVR_FOUNDATION_INCLUDED)
  message(FATAL_ERROR "GvrFoundation not found.")
endif (NOT DEFINED GVR_FOUNDATION_INCLUDED)

include(GvrExport)
if (NOT DEFINED GVR_EXPORT_INCLUDED)
  message(FATAL_ERROR "GvrExport not found.")
endif (NOT DEFINED GVR_EXPORT_INCLUDED)

include(GvrDoxygen)
if (NOT DEFINED GVR_DOXYGEN_INCLUDED)
  message(FATAL_ERROR "GvrDoxygen not found.")
endif (NOT DEFINED GVR_DOXYGEN_INCLUDED)

include(GvrCheckVersion)
if (NOT DEFINED GVR_CHECK_VERSION_INCLUDED)
  message(FATAL_ERROR "GvrCheckVersion not found.")
endif (NOT DEFINED GVR_CHECK_VERSION_INCLUDED)

include(GvrExportBuild)
if (NOT DEFINED GVR_EXPORT_BUILD_INCLUDED)
  message(FATAL_ERROR "GvrExportBuild not found.")
endif (NOT DEFINED GVR_EXPORT_BUILD_INCLUDED)

if (NOT GVR_DISABLE_IMPORT)
  gvr_import_check(libgvrdummy PATHS ${GVR_REMOTE_IMPORT_LOCATIONS} PATH_SUFFIXES ${GVR_IMPORT_PATH_SUFFIXES})
endif (NOT GVR_DISABLE_IMPORT)
if (libgvrdummy_IMPORTABLE)
  return()
endif (libgvrdummy_IMPORTABLE)

if (GVR_RUN_TEST)
  enable_testing()
endif (GVR_RUN_TEST)

############### Project General Information ###############
set(${PROJECT_NAME}_BUNDLE_NAME "gvrdummy")

set(${PROJECT_NAME}_MAJOR_VERSION 0)
set(${PROJECT_NAME}_MINOR_VERSION 0)
set(${PROJECT_NAME}_PATCH_VERSION 0)
# set(${PROJECT_NAME}_TWEAK_VERSION 0) ##TODO: Modify as needed.
set(${PROJECT_NAME}_VERSION_COUNT 3) ## 1 Means "Major" - 2 Means "Major.Minor" - 3 Means "Major.Minor.Patch" - 4 Means "Major.Minor.Patch.Tweak"

set(${PROJECT_NAME}_VERSION "${${PROJECT_NAME}_MAJOR_VERSION}.${${PROJECT_NAME}_MINOR_VERSION}.${${PROJECT_NAME}_PATCH_VERSION}")
if (GVR_REVISION)
  set(${PROJECT_NAME}_VERSION "${${PROJECT_NAME}_VERSION}-r${GVR_REVISION}")
endif (GVR_REVISION)

############### Project Temporary Information ###############
set(${PROJECT_NAME}_TEMPORARY_LOCATION "${PROJECT_BINARY_DIR}/temporary")
set(${PROJECT_NAME}_TEMPORARY_DOCUMENTATION_LOCATION "${${PROJECT_NAME}_TEMPORARY_LOCATION}/documentation")

############### Project Export Information ###############
set(${PROJECT_NAME}_EXPORTABLE_TARGETS)

############### Project Install Information ###############
set(${PROJECT_NAME}_INSTALL_RELATIVE_LOCATION "${GVR_INSTALL_PROVIDER_RELATIVE_LOCATION}")

set(${PROJECT_NAME}_INSTALLABLE_TARGETS)

############### Project API (Internal) Documentation Information ###############
set(${PROJECT_NAME}_DOCUMENTATION_API_FILE "${PROJECT_SOURCE_DIR}/Doxyfile.in")
set(${PROJECT_NAME}_DOCUMENTATION_API_SOURCE "${PROJECT_SOURCE_DIR}")
set(${PROJECT_NAME}_DOCUMENTATION_API_DESTINATION "${GVR_DOCUMENTATION_LOCATION}/${${PROJECT_NAME}_BUNDLE_NAME}-${${PROJECT_NAME}_VERSION}")
set(${PROJECT_NAME}_DOCUMENTATION_API_TEMPORARY "${${PROJECT_NAME}_TEMPORARY_DOCUMENTATION_LOCATION}/api")

############### Project API (Internal) Documentation Process ###############
gvr_document_with_doxygen(DOXYFILE "${${PROJECT_NAME}_DOCUMENTATION_API_FILE}" DESTINATION_LOCATION "${${PROJECT_NAME}_DOCUMENTATION_API_DESTINATION}"
                                   TEMPORARY_LOCATION "${${PROJECT_NAME}_DOCUMENTATION_API_TEMPORARY}"
                                   PROPERTIES
                                              DOCUMENTATION_PROJECT ${PROJECT_NAME}
                                              DOCUMENTATION_VERSION ${${PROJECT_NAME}_VERSION}
                                              DOCUMENTATION_SOURCE  ${${PROJECT_NAME}_DOCUMENTATION_API_SOURCE}
                                              DOCUMENTATION_EXCLUDE_PATTERNS "*/.svn/* */.build/*"
                                              DOCUMENTATION_DESTINATION "."
                                   CUSTOM_TARGET ${PROJECT_NAME}-documentation
                                   DEPENDS "${${PROJECT_NAME}_DOCUMENTATION_API_FILE}"
                                   SKIP_DOT
                                   VERSION 1.6.3)

############### Project Build Requirements ###############
####################################################
## Prioritized import locations for a new package ##
#if (NOT GVR_DISABLE_IMPORT)
#  find_package({...} 0.0.0 NO_MODULE QUIET NO_DEFAULT_PATH PATHS ${GVR_REMOTE_IMPORT_LOCATIONS} PATH_SUFFIXES import)
#endif (NOT GVR_DISABLE_IMPORT)
#find_package({...} 0.0.0 NO_MODULE QUIET NO_DEFAULT_PATH PATHS ${GVR_LOCAL_IMPORT_LOCATIONS})
#find_package({...} 0.0.0 NO_MODULE REQUIRED)
####################################################

############### Project Check Version ###############
gvr_check_version(PACKAGE ${PROJECT_NAME} VERSION ${${PROJECT_NAME}_VERSION} CHANGELOG "${PROJECT_SOURCE_DIR}/ChangeLog")

############### Project Build Process ###############
add_definitions(${GVR_BUILD_FLAGS})

include_directories("${PROJECT_SOURCE_DIR}/include")

#link_directories()

if (NOT TARGET gvrdummy)
  set(${PROJECT_NAME}_TARGET gvrdummy)

  add_library(${${PROJECT_NAME}_TARGET}
                                        ${GVR_BUILD_LIBRARY_TYPE}
                                        "${PROJECT_SOURCE_DIR}/src/dummy.cpp")

  set_target_properties(${${PROJECT_NAME}_TARGET}
                                                  PROPERTIES
                                                             VERSION ${${PROJECT_NAME}_VERSION}
                                                             SOVERSION 0)

#  target_link_libraries(${${PROJECT_NAME}_TARGET}
#                                                  dependency)
  if (GVR_CHECK_REVISION)
    add_dependencies(${${PROJECT_NAME}_TARGET}
                                               ${GVR_CHECK_REVISION})
  endif (GVR_CHECK_REVISION)

  set(${PROJECT_NAME}_EXPORTABLE_TARGETS ${${PROJECT_NAME}_EXPORTABLE_TARGETS} ${${PROJECT_NAME}_TARGET})

  set(${PROJECT_NAME}_INSTALLABLE_TARGETS ${${PROJECT_NAME}_INSTALLABLE_TARGETS} ${${PROJECT_NAME}_TARGET})
endif (NOT TARGET gvrdummy)

############### Project Export Process ###############
if (${PROJECT_NAME}_EXPORTABLE_TARGETS)
  gvr_export( NAME "${PROJECT_NAME}"
              DESTINATION_LOCATION "${GVR_LOCAL_EXPORT_LOCATION}"
              COPYRIGHT "Copyright © 2014 Gilbarco Inc. Confidential and Proprietary."
              VERSION "${${PROJECT_NAME}_VERSION}"
              COMPATIBILITY AnyNewerVersion
              INCLUDES "${PROJECT_SOURCE_DIR}/include"
              TARGETS ${${PROJECT_NAME}_EXPORTABLE_TARGETS})
endif (${PROJECT_NAME}_EXPORTABLE_TARGETS)

############### Project Install Process ###############
if (${PROJECT_NAME}_INSTALLABLE_TARGETS)
  install(TARGETS ${${PROJECT_NAME}_INSTALLABLE_TARGETS}
                  RUNTIME DESTINATION "${${PROJECT_NAME}_INSTALL_RELATIVE_LOCATION}/bin"                                            COMPONENT Runtime
                  LIBRARY DESTINATION "${${PROJECT_NAME}_INSTALL_RELATIVE_LOCATION}/lib"                                            COMPONENT Runtime
                  ARCHIVE DESTINATION "${${PROJECT_NAME}_INSTALL_RELATIVE_LOCATION}/lib/${PROJECT_NAME}-${${PROJECT_NAME}_VERSION}" COMPONENT Development)
endif (${PROJECT_NAME}_INSTALLABLE_TARGETS)

install(DIRECTORY "${PROJECT_SOURCE_DIR}/include/" DESTINATION "${${PROJECT_NAME}_INSTALL_RELATIVE_LOCATION}/include"
                  USE_SOURCE_PERMISSIONS
                  DIRECTORY_PERMISSIONS
                    OWNER_READ OWNER_EXECUTE OWNER_WRITE
                    GROUP_READ GROUP_EXECUTE
                    WORLD_READ WORLD_EXECUTE
                  COMPONENT Development
                  REGEX "share/info/dir|share/info/dir\\.gz$" EXCLUDE
                  REGEX "/\\.svn$" EXCLUDE)

############### Project Export Build Process ###############
set(EXPORT_BUILD_STRIP)
set(EXPORT_BUILD_OPTIONS)
if (CMAKE_BUILD_TYPE STREQUAL RELEASE)
  set(EXPORT_BUILD_STRIP STRIP)
endif (CMAKE_BUILD_TYPE STREQUAL RELEASE)

gvr_export_build_install(PROJECT ${PROJECT_NAME}
                         DESTINATION "${GVR_REMOTE_EXPORT_LOCATION}/${PROJECT_NAME}/${CMAKE_BUILD_TYPE}"
                         ${EXPORT_BUILD_OPTIONS}
                         REMOVE_FIRST
                         ${EXPORT_BUILD_STRIP})

if (CMAKE_BUILD_TYPE STREQUAL RELEASE)
  gvr_export_build_extra(PROJECT ${PROJECT_NAME}
                         DESTINATION "${GVR_REMOTE_EXPORT_LOCATION}/${PROJECT_NAME}"
                         RESOURCES "misc/export/Makefile"
                         DIRECTORY_PERMISSIONS
                           OWNER_READ OWNER_WRITE OWNER_EXECUTE
                           GROUP_READ GROUP_WRITE
                           WORLD_READ
                         FILE_PERMISSIONS 
                           OWNER_READ OWNER_WRITE
                           GROUP_READ GROUP_WRITE
                           WORLD_READ
                         NO_SOURCE_PERMISSIONS
                         PATTERNS
                          PATTERN "*.readonly" PERMISSIONS OWNER_READ GROUP_READ WORLD_READ
                          REGEX "/\\.svn$" EXCLUDE
                         REMOVE_FIRST_RESOURCES_ONLY)
endif (CMAKE_BUILD_TYPE STREQUAL RELEASE)

gvr_export_build_information(PROJECT ${PROJECT_NAME}
                             NAME "${PROJECT_NAME}"
                             DESTINATION "${GVR_REMOTE_EXPORT_LOCATION}/${PROJECT_NAME}/${GVR_EXPORT_BUILD_IMPORT_PATH_SUFFIX}"
                             COPYRIGHT "Copyright © 2014 Gilbarco Inc. Confidential and Proprietary."
                             VERSION "${${PROJECT_NAME}_VERSION}"
                             COMPATIBILITY AnyNewerVersion
                             INCLUDES "<PROJECT_IMPORTED_EXTERNAL_PREFIX_LOCATION>/${CMAKE_BUILD_TYPE}/${${PROJECT_NAME}_INSTALL_RELATIVE_LOCATION}/include"
                             LINK_LOCATIONS "<PROJECT_IMPORTED_EXTERNAL_PREFIX_LOCATION>/${CMAKE_BUILD_TYPE}/${${PROJECT_NAME}_INSTALL_RELATIVE_LOCATION}/lib"
                                            "<PROJECT_IMPORTED_EXTERNAL_PREFIX_LOCATION>/${CMAKE_BUILD_TYPE}/${${PROJECT_NAME}_INSTALL_RELATIVE_LOCATION}/lib/${PROJECT_NAME}-${${PROJECT_NAME}_VERSION}"
                             COMMAND_LOCATIONS "<PROJECT_IMPORTED_EXTERNAL_PREFIX_LOCATION>/${CMAKE_BUILD_TYPE}/${${PROJECT_NAME}_INSTALL_RELATIVE_LOCATION}/bin"
                             TARGETS ${${PROJECT_NAME}_INSTALLABLE_TARGETS}
                             REMOVE_FIRST)

gvr_export_build_bundle(PROJECT ${PROJECT_NAME} BUNDLES gvr dummy)

############### Project Test Process ###############
add_subdirectory(test)
