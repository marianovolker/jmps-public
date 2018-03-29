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
  find_package(libopenssl_3rdparty 1.0.1 NO_MODULE QUIET NO_DEFAULT_PATH PATHS ${GVR_REMOTE_IMPORT_LOCATIONS} PATH_SUFFIXES ${GVR_IMPORT_PATH_SUFFIXES})
endif (NOT GVR_DISABLE_IMPORT)
find_package(libopenssl_3rdparty 1.0.1 NO_MODULE QUIET NO_DEFAULT_PATH PATHS ${GVR_LOCAL_IMPORT_LOCATIONS})
find_package(libopenssl_3rdparty 1.0.1 NO_MODULE REQUIRED)

if (NOT GVR_DISABLE_IMPORT)
  find_package(libpoco_3rdparty 1.4.7 NO_MODULE QUIET NO_DEFAULT_PATH PATHS ${GVR_REMOTE_IMPORT_LOCATIONS} PATH_SUFFIXES ${GVR_IMPORT_PATH_SUFFIXES})
endif (NOT GVR_DISABLE_IMPORT)
find_package(libpoco_3rdparty 1.4.7 NO_MODULE QUIET NO_DEFAULT_PATH PATHS ${GVR_LOCAL_IMPORT_LOCATIONS})
find_package(libpoco_3rdparty 1.4.7 NO_MODULE REQUIRED)


if (NOT libpoco_3rdparty_IMPORTED_EXTERNAL)
  message(FATAL_ERROR "Package libpoco_3rdparty should be imported as an external library.")
endif (NOT libpoco_3rdparty_IMPORTED_EXTERNAL)


############### SubProject Check Version ###############
#gvr_check_version(PACKAGE ${${PROJECT_NAME}_TOOL_NAME} VERSION ${${${PROJECT_NAME}_TOOL_NAME}_VERSION}
#                  DEPENDS libpoco_3rdparty 17.1gvr ${libpoco_3rdparty_VERSION})

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

file(REMOVE_RECURSE ${CMAKE_CURRENT_BINARY_DIR}/payloads)
file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/misc/payloads
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

if (libopenssl_3rdparty_IMPORTED_EXTERNAL)
  add_definitions(${libopenssl_3rdparty_IMPORTED_DEFINITIONS})
  include_directories(${libopenssl_3rdparty_IMPORTED_INCLUDES})
  link_directories(${libopenssl_3rdparty_IMPORTED_LINK_LOCATIONS})
endif (libopenssl_3rdparty_IMPORTED_EXTERNAL)

if (libpoco_3rdparty_IMPORTED_EXTERNAL)
  add_definitions(${libpoco_3rdparty_IMPORTED_DEFINITIONS})
  include_directories(${libpoco_3rdparty_IMPORTED_INCLUDES})
  link_directories(${libpoco_3rdparty_IMPORTED_LINK_LOCATIONS})
endif (libpoco_3rdparty_IMPORTED_EXTERNAL)

if (NOT TARGET gvrcloud_client_registration_tool)
  set(${${PROJECT_NAME}_TOOL_NAME}_TARGET gvrcloud_client_registration_tool)

  add_executable( ${${${PROJECT_NAME}_TOOL_NAME}_TARGET}
                                                        "${CMAKE_CURRENT_SOURCE_DIR}/src/WrongCertificateChecker.cpp"
                                                        "${CMAKE_CURRENT_SOURCE_DIR}/src/cloud_client_registration_tool.cpp"
                )

  set_target_properties(${${${PROJECT_NAME}_TOOL_NAME}_TARGET}
                                                               PROPERTIES
                                                                          VERSION ${${${PROJECT_NAME}_TOOL_NAME}_VERSION})

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

  target_link_libraries(${${${PROJECT_NAME}_TOOL_NAME}_TARGET}
                                                               gvrcloudcore gvrucc gvrspot gvrcommunication gvrlog
                                                               ${${PROJECT_NAME}_POCO_LIBS}
                                                               cjson
                                                               ssl crypto
                                                               ${libpoco_3rdparty_OPTIONAL_DYNAMIC_LINK_FLAGS}
                                                               ${CMAKE_THREAD_LIBS_INIT})

  add_dependencies(${${${PROJECT_NAME}_TOOL_NAME}_TARGET}
                                                          ${GVR_CHECK_REVISION}
                                                          gvrcloudcore gvrucc gvrspot gvrcommunication gvrlog
                                                          ${${PROJECT_NAME}_POCO_LIBS}
                                                          cjson
                                                          ssl crypto
                                                          ${libpoco_3rdparty_OPTIONAL_DYNAMIC_LINK_FLAGS}
                                                          ${CMAKE_THREAD_LIBS_INIT})

  set(${${PROJECT_NAME}_TOOL_NAME}_EXPORTABLE_TARGETS ${${${PROJECT_NAME}_TOOL_NAME}_EXPORTABLE_TARGETS} ${${${PROJECT_NAME}_TOOL_NAME}_TARGET})

  set(${${PROJECT_NAME}_TOOL_NAME}_INSTALLABLE_TARGETS ${${${PROJECT_NAME}_TOOL_NAME}_INSTALLABLE_TARGETS} ${${${PROJECT_NAME}_TOOL_NAME}_TARGET})
endif (NOT TARGET gvrcloud_client_registration_tool)

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

install(DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/misc/" DESTINATION "${${PROJECT_NAME}_INSTALL_RELATIVE_LOCATION}/bin"
                  USE_SOURCE_PERMISSIONS
                  DIRECTORY_PERMISSIONS
                    OWNER_READ OWNER_EXECUTE OWNER_WRITE
                    GROUP_READ GROUP_EXECUTE
                    WORLD_READ WORLD_EXECUTE
                  COMPONENT Development
                  REGEX "share/info/dir|share/info/dir\\.gz$" EXCLUDE
                  REGEX "/\\.svn$" EXCLUDE)
