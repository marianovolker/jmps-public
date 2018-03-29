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

if (NOT DEFINED GVR_MODULE_PATH)
  message(FATAL_ERROR "GVR_MODULE_PATH not defined.")
endif (NOT DEFINED GVR_MODULE_PATH)

############### Project Definition ###############
project(gvrcloud)

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
  gvr_import_check(gvrcloud PATHS ${GVR_REMOTE_IMPORT_LOCATIONS} PATH_SUFFIXES ${GVR_IMPORT_PATH_SUFFIXES})
endif (NOT GVR_DISABLE_IMPORT)
if (gvrcloud_IMPORTABLE)
  return()
endif (gvrcloud_IMPORTABLE)

if (GVR_RUN_TEST)
  enable_testing()
endif (GVR_RUN_TEST)

############### Project General Information ###############
set(${PROJECT_NAME}_BUNDLE_NAME "gvrcloud")

set(${PROJECT_NAME}_MAJOR_VERSION 0)
set(${PROJECT_NAME}_MINOR_VERSION 7)
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

if (NOT GVR_DISABLE_IMPORT)
  find_package(libcjson_3rdparty 0.0.1 NO_MODULE QUIET NO_DEFAULT_PATH PATHS ${GVR_REMOTE_IMPORT_LOCATIONS} PATH_SUFFIXES ${GVR_IMPORT_PATH_SUFFIXES})
endif (NOT GVR_DISABLE_IMPORT)
find_package(libcjson_3rdparty 0.0.1 NO_MODULE QUIET NO_DEFAULT_PATH PATHS ${GVR_LOCAL_IMPORT_LOCATIONS})
find_package(libcjson_3rdparty 0.0.1 NO_MODULE REQUIRED)

if (NOT GVR_DISABLE_IMPORT)
  find_package(libgvrlog 0.0.0 NO_MODULE QUIET NO_DEFAULT_PATH PATHS ${GVR_REMOTE_IMPORT_LOCATIONS} PATH_SUFFIXES ${GVR_IMPORT_PATH_SUFFIXES})
endif (NOT GVR_DISABLE_IMPORT)
find_package(libgvrlog 0.0.0 NO_MODULE QUIET NO_DEFAULT_PATH PATHS ${GVR_LOCAL_IMPORT_LOCATIONS})
find_package(libgvrlog 0.0.0 NO_MODULE REQUIRED)

if (NOT GVR_DISABLE_IMPORT)
  find_package(libgvrcommunication 0.0.0 NO_MODULE QUIET NO_DEFAULT_PATH PATHS ${GVR_REMOTE_IMPORT_LOCATIONS} PATH_SUFFIXES ${GVR_IMPORT_PATH_SUFFIXES})
endif (NOT GVR_DISABLE_IMPORT)
find_package(libgvrcommunication 0.0.0 NO_MODULE QUIET NO_DEFAULT_PATH PATHS ${GVR_LOCAL_IMPORT_LOCATIONS})
find_package(libgvrcommunication 0.0.0 NO_MODULE REQUIRED)

if (NOT GVR_DISABLE_IMPORT)
  find_package(libgvrucc 0.0.0 NO_MODULE QUIET NO_DEFAULT_PATH PATHS ${GVR_REMOTE_IMPORT_LOCATIONS} PATH_SUFFIXES ${GVR_IMPORT_PATH_SUFFIXES})
endif (NOT GVR_DISABLE_IMPORT)
find_package(libgvrucc 0.0.0 NO_MODULE QUIET NO_DEFAULT_PATH PATHS ${GVR_LOCAL_IMPORT_LOCATIONS})
find_package(libgvrucc 0.0.0 NO_MODULE REQUIRED)

if (NOT GVR_DISABLE_IMPORT)
  find_package(libgvrspot 0.0.0 NO_MODULE QUIET NO_DEFAULT_PATH PATHS ${GVR_REMOTE_IMPORT_LOCATIONS} PATH_SUFFIXES ${GVR_IMPORT_PATH_SUFFIXES})
endif (NOT GVR_DISABLE_IMPORT)
find_package(libgvrspot 0.0.0 NO_MODULE QUIET NO_DEFAULT_PATH PATHS ${GVR_LOCAL_IMPORT_LOCATIONS})
find_package(libgvrspot 0.0.0 NO_MODULE REQUIRED)

if (NOT libpoco_3rdparty_IMPORTED_EXTERNAL)
  message(FATAL_ERROR "Package libpoco_3rdparty should be imported as an external library.")
endif (NOT libpoco_3rdparty_IMPORTED_EXTERNAL)

if (NOT libcjson_3rdparty_IMPORTED_EXTERNAL)
  message(FATAL_ERROR "Package libcjson_3rdparty should be imported as an external library.")
endif (NOT libcjson_3rdparty_IMPORTED_EXTERNAL)

if (NOT libgvrlog_IMPORTED_EXTERNAL)
  message(FATAL_ERROR "Package libgvrlog should be imported as an external library.")
endif (NOT libgvrlog_IMPORTED_EXTERNAL)

if (NOT libgvrcommunication_IMPORTED_EXTERNAL)
  message(FATAL_ERROR "Package libgvrcommunication should be imported as an external library.")
endif (NOT libgvrcommunication_IMPORTED_EXTERNAL)

if (NOT libgvrucc_IMPORTED_EXTERNAL)
  message(FATAL_ERROR "Package libgvrucc should be imported as an external library.")
endif (NOT libgvrucc_IMPORTED_EXTERNAL)

if (NOT libgvrspot_IMPORTED_EXTERNAL)
  message(FATAL_ERROR "Package libgvrspot should be imported as an external library.")
endif (NOT libgvrspot_IMPORTED_EXTERNAL)


############### SubProject Check Version ###############
#gvr_check_version(PACKAGE ${${PROJECT_NAME}_TOOL_NAME} VERSION ${${${PROJECT_NAME}_TOOL_NAME}_VERSION}
#                  DEPENDS libpoco_3rdparty 17.1gvr ${libpoco_3rdparty_VERSION})

gvr_check_version(PACKAGE ${PROJECT_NAME} VERSION ${${PROJECT_NAME}_VERSION} CHANGELOG "${PROJECT_SOURCE_DIR}/ChangeLog")


############### Project Build Process ###############
add_definitions(${GVR_BUILD_FLAGS})

include_directories("${PROJECT_SOURCE_DIR}/include")

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

if (libcjson_3rdparty_IMPORTED_EXTERNAL)
  add_definitions(${libcjson_3rdparty_IMPORTED_DEFINITIONS})
  include_directories(${libcjson_3rdparty_IMPORTED_INCLUDES})
  link_directories(${libcjson_3rdparty_IMPORTED_LINK_LOCATIONS})
endif (libcjson_3rdparty_IMPORTED_EXTERNAL)

if (libgvrlog_IMPORTED_EXTERNAL)
  add_definitions(${libgvrlog_IMPORTED_DEFINITIONS})
  include_directories(${libgvrlog_IMPORTED_INCLUDES})
  link_directories(${libgvrlog_IMPORTED_LINK_LOCATIONS})
endif (libgvrlog_IMPORTED_EXTERNAL)

if (libgvrcommunication_IMPORTED_EXTERNAL)
  add_definitions(${libgvrcommunication_IMPORTED_DEFINITIONS})
  include_directories(${libgvrcommunication_IMPORTED_INCLUDES})
  link_directories(${libgvrcommunication_IMPORTED_LINK_LOCATIONS})
endif (libgvrcommunication_IMPORTED_EXTERNAL)

if (libgvrucc_IMPORTED_EXTERNAL)
  add_definitions(${libgvrucc_IMPORTED_DEFINITIONS})
  include_directories(${libgvrucc_IMPORTED_INCLUDES})
  link_directories(${libgvrucc_IMPORTED_LINK_LOCATIONS})
endif (libgvrucc_IMPORTED_EXTERNAL)

if (libgvrspot_IMPORTED_EXTERNAL)
  add_definitions(${libgvrspot_IMPORTED_DEFINITIONS})
  include_directories(${libgvrspot_IMPORTED_INCLUDES})
  link_directories(${libgvrspot_IMPORTED_LINK_LOCATIONS})
endif (libgvrspot_IMPORTED_EXTERNAL)

if (NOT TARGET gvrcloudcore)
  set(${PROJECT_NAME}_TARGET gvrcloudcore)

  add_library(${${PROJECT_NAME}_TARGET}
                                        ${GVR_BUILD_LIBRARY_TYPE}
                                        "${PROJECT_SOURCE_DIR}/src/cloud_core.cpp"
                                        "${PROJECT_SOURCE_DIR}/src/CloudStatus.cpp"
                                        "${PROJECT_SOURCE_DIR}/src/CrlManager.cpp"
                                        "${PROJECT_SOURCE_DIR}/src/EncryptData.cpp"
                                        "${PROJECT_SOURCE_DIR}/src/CloudConfigDeviceChecker.cpp"
                                        "${PROJECT_SOURCE_DIR}/src/DeviceInformation.cpp"
                                        "${PROJECT_SOURCE_DIR}/src/workers/AbsOptCloudClientWorker.cpp"
                                        "${PROJECT_SOURCE_DIR}/src/workers/AbsClrWorker.cpp"
                                        "${PROJECT_SOURCE_DIR}/src/workers/OptCloudClientWorkerReset.cpp"
                                        "${PROJECT_SOURCE_DIR}/src/workers/DebianInfoCollectorWorker.cpp"
                                        "${PROJECT_SOURCE_DIR}/src/workers/OptCloudClientWorkerFileActivation.cpp"
                                        "${PROJECT_SOURCE_DIR}/src/workers/FileUploadWorker.cpp"
                                        "${PROJECT_SOURCE_DIR}/src/workers/SpotReleaseDownloadWorker.cpp"
                                        "${PROJECT_SOURCE_DIR}/src/workers/CloudRegistrationWorker.cpp"
                                        "${PROJECT_SOURCE_DIR}/src/workers/CloudStatusWorker.cpp"
                                        "${PROJECT_SOURCE_DIR}/src/workers/CloudRegistrationStatusWorker.cpp"
                                        "${PROJECT_SOURCE_DIR}/src/workers/SystemStatusNotificationWorker.cpp"
                                        "${PROJECT_SOURCE_DIR}/src/workers/CloudReconnectWorker.cpp"
                                        "${PROJECT_SOURCE_DIR}/src/workers/SecurityChangeWorker.cpp"
                                        "${PROJECT_SOURCE_DIR}/src/workers/CloudDnsConfigurationWorker.cpp"
                                         )

  set_target_properties(${${PROJECT_NAME}_TARGET}
                                                  PROPERTIES
                                                             VERSION ${${PROJECT_NAME}_VERSION}
                                                             SOVERSION 0)

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

#libgvrcloudcore uses these libraries
  target_link_libraries(${${PROJECT_NAME}_TARGET}
                                                   gvrucc gvrspot gvrcommunication gvrlog
                                                   ${${PROJECT_NAME}_POCO_LIBS}
                                                   cjson
                                                   ssl crypto
                                                   ${libpoco_3rdparty_OPTIONAL_DYNAMIC_LINK_FLAGS}
                                                   ${CMAKE_THREAD_LIBS_INIT})

  if (GVR_CHECK_REVISION)
    add_dependencies(${${PROJECT_NAME}_TARGET}
                                               ${GVR_CHECK_REVISION}
                                               gvrucc gvrspot gvrcommunication gvrlog
                                               ${${PROJECT_NAME}_POCO_LIBS}
                                               cjson
                                               ssl crypto
                                               ${libpoco_3rdparty_OPTIONAL_DYNAMIC_LINK_FLAGS}
                                               ${CMAKE_THREAD_LIBS_INIT})
  endif (GVR_CHECK_REVISION)

  set(${PROJECT_NAME}_EXPORTABLE_TARGETS ${${PROJECT_NAME}_EXPORTABLE_TARGETS} ${${PROJECT_NAME}_TARGET})

  set(${PROJECT_NAME}_INSTALLABLE_TARGETS ${${PROJECT_NAME}_INSTALLABLE_TARGETS} ${${PROJECT_NAME}_TARGET})
endif (NOT TARGET gvrcloudcore)

if (NOT TARGET gvrcloud)
  set(${PROJECT_NAME}_TARGET gvrcloud)

  add_executable(${${PROJECT_NAME}_TARGET}
                                            "${CMAKE_CURRENT_SOURCE_DIR}/src/cloud_main.cpp")

  set_target_properties(${${PROJECT_NAME}_TARGET}
                                                  PROPERTIES
                                                             VERSION ${${PROJECT_NAME}_VERSION})

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

#main() executable uses these libraries
  target_link_libraries(${${PROJECT_NAME}_TARGET}
                                                  gvrcloudcore gvrucc gvrspot gvrcommunication gvrlog
                                                  ${${PROJECT_NAME}_POCO_LIBS}
                                                  cjson
                                                  ssl crypto
                                                  ${libpoco_3rdparty_OPTIONAL_DYNAMIC_LINK_FLAGS}
                                                  ${CMAKE_THREAD_LIBS_INIT})

  if (GVR_CHECK_REVISION)
    add_dependencies(${${PROJECT_NAME}_TARGET}
                                               ${GVR_CHECK_REVISION}
                                               gvrcloudcore gvrucc gvrspot gvrcommunication gvrlog
                                               ${${PROJECT_NAME}_POCO_LIBS}
                                               cjson
                                               ssl crypto
                                               ${libpoco_3rdparty_OPTIONAL_DYNAMIC_LINK_FLAGS}
                                               ${CMAKE_THREAD_LIBS_INIT})
  endif (GVR_CHECK_REVISION)

  set(${PROJECT_NAME}_EXPORTABLE_TARGETS ${${PROJECT_NAME}_EXPORTABLE_TARGETS} ${${PROJECT_NAME}_TARGET})

  set(${PROJECT_NAME}_INSTALLABLE_TARGETS ${${PROJECT_NAME}_INSTALLABLE_TARGETS} ${${PROJECT_NAME}_TARGET})
endif (NOT TARGET gvrcloud)

if (GVR_TARGET_PLATFORM STREQUAL m5)
  set(CLOUD_ENCRYPTED_DATA "/gilbarco/data/CLOUD/cloud_server_data.bin")
  set(CLOUD_NEW_ENCRYPTED_DATA "/gilbarco/data/CLOUD/new_cloud_server_data.bin")
  set(SPOT_RELEASE_DIR "/storage/CLOUD/releases")
  set(CLOUD_UPLOAD_DIR "/storage/CLOUD/upload")
  set(UPLOADABLE_LOG_FILES "/gilbarco/resources/cloud/cloud-PH-logs.cfg")
  set(REGISTRATION_SERVER_DATA "/usr/ROMfs6/opt/gilbarco/etc/registration_server_data.ini")
  set(DEVICE_REGISTRATION_INFO "/gilbarco/data/CLOUD/deviceRegisterInfo.ini")
  set(CLOUD_LOG_FILE "/gilbarco/logs/Cloud_PH.log")
  set(DEVICE_DNS_CONFIGURATION_INFO "/gilbarco/data/CLOUD/deviceDnsConfigurationInfo.ini")
elseif (GVR_TARGET_PLATFORM STREQUAL m7)
  set(CLOUD_ENCRYPTED_DATA "/gilbarco/appsdata/CLOUD/cloud_server_data.bin")
  set(CLOUD_NEW_ENCRYPTED_DATA "/gilbarco/appsdata/CLOUD/new_cloud_server_data.bin")
  set(SPOT_RELEASE_DIR "/gilbarco/appsdata/CLOUD/releases")
  set(CLOUD_UPLOAD_DIR "/gilbarco/appsdata/CLOUD/upload")
  set(UPLOADABLE_LOG_FILES "/gilbarco/resources/cloud/cloud-PH-logs.cfg")
  set(REGISTRATION_SERVER_DATA "/usr/packages/CLOUD/opt/gilbarco/etc/registration_server_data.ini")
  set(DEVICE_REGISTRATION_INFO "/gilbarco/appsdata/CLOUD/deviceRegisterInfo.ini")
  set(CLOUD_LOG_FILE "/gilbarco/logs/CLOUD/Cloud_PH.log")
  set(DEVICE_DNS_CONFIGURATION_INFO "/gilbarco/appsdata/CLOUD/deviceDnsConfigurationInfo.ini")
elseif (GVR_TARGET_PLATFORM STREQUAL m3)
  set(CLOUD_ENCRYPTED_DATA "/jffs2/data/CLOUD/cloud_server_data.bin")
  set(CLOUD_NEW_ENCRYPTED_DATA "/jffs2/data/CLOUD/new_cloud_server_data.bin")
  set(SPOT_RELEASE_DIR "/jffs2/data/CLOUD/releases")
  set(CLOUD_UPLOAD_DIR "/jffs2/data/CLOUD/upload")
  set(UPLOADABLE_LOG_FILES "/jffs2/resources/cloud/cloud-PH-logs.cfg")
  set(REGISTRATION_SERVER_DATA "/usr/ROMfs6/opt/gilbarco/etc/registration_server_data.ini")
  set(DEVICE_REGISTRATION_INFO "/jffs2/data/CLOUD/deviceRegisterInfo.ini")
  set(CLOUD_LOG_FILE "/gilbarco/logs/Cloud_PH.log")
  set(DEVICE_DNS_CONFIGURATION_INFO "/jffs2/data/CLOUD/deviceDnsConfigurationInfo.ini")
else (GVR_TARGET_PLATFORM STREQUAL m5)
  set(CLOUD_ENCRYPTED_DATA "./cloud_server_data.bin")
  set(CLOUD_NEW_ENCRYPTED_DATA "./new_cloud_server_data.bin")
  set(SPOT_RELEASE_DIR "./releases")
  set(CLOUD_UPLOAD_DIR "./upload")
  set(UPLOADABLE_LOG_FILES "./cloud-PH-logs.cfg")
  set(REGISTRATION_SERVER_DATA "./registration_server_data.ini")
  set(DEVICE_REGISTRATION_INFO "./deviceRegisterInfo.ini")
  set(CLOUD_LOG_FILE "./Cloud_PH.log")
  set(DEVICE_DNS_CONFIGURATION_INFO "./deviceDnsConfigurationInfo.ini")
endif (GVR_TARGET_PLATFORM STREQUAL m5)


configure_file("${CMAKE_SOURCE_DIR}/misc/gvrcloud.ini.in" "${CMAKE_BINARY_DIR}/gvrcloud.ini" @ONLY)
configure_file("${CMAKE_SOURCE_DIR}/misc/registration_server_data.ini.in" "${CMAKE_BINARY_DIR}/registration_server_data.ini" @ONLY)
configure_file("${CMAKE_SOURCE_DIR}/misc/deviceRegisterInfo.ini.in" "${CMAKE_BINARY_DIR}/deviceRegisterInfo.ini" @ONLY)
configure_file("${CMAKE_SOURCE_DIR}/misc/logger.ini.in" "${CMAKE_BINARY_DIR}/logger.ini" @ONLY)
configure_file("${CMAKE_SOURCE_DIR}/misc/resources/cloud-PH-logs.cfg" "${CMAKE_BINARY_DIR}/cloud-PH-logs.cfg" @ONLY)
configure_file("${CMAKE_SOURCE_DIR}/misc/resources/cloud-PH-events.cfg" "${CMAKE_BINARY_DIR}/cloud-PH-events.cfg" @ONLY)



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

set (${PROJECT_NAME}_CONFIG_DIR bin)
if (CMAKE_CROSSCOMPILING)
  set (${PROJECT_NAME}_CONFIG_DIR etc)
endif (CMAKE_CROSSCOMPILING)

install(FILES "${CMAKE_BINARY_DIR}/gvrcloud.ini" "${CMAKE_BINARY_DIR}/deviceRegisterInfo.ini" "${CMAKE_BINARY_DIR}/registration_server_data.ini" "${CMAKE_BINARY_DIR}/logger.ini" DESTINATION "${${PROJECT_NAME}_INSTALL_RELATIVE_LOCATION}/${${PROJECT_NAME}_CONFIG_DIR}"
           PERMISSIONS
             OWNER_READ OWNER_WRITE
             GROUP_READ
             WORLD_READ
           COMPONENT Runtime)


set (${PROJECT_NAME}_RESOURCES_DIR resources)

install(FILES "${CMAKE_BINARY_DIR}/cloud-PH-logs.cfg" "${CMAKE_BINARY_DIR}/cloud-PH-events.cfg" DESTINATION "${${PROJECT_NAME}_INSTALL_RELATIVE_LOCATION}/${${PROJECT_NAME}_RESOURCES_DIR}"
           PERMISSIONS
             OWNER_READ OWNER_WRITE
             GROUP_READ
             WORLD_READ
           COMPONENT Runtime)

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

############### Project Test Process ###############
add_subdirectory(test)
