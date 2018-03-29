##-#######################################################################-##
# Copyright © 2014, 2015 Gilbarco Inc.
# Confidential and Proprietary.
##
## @file CMakeLists.txt
## @author mgrieco
## @date Friday, December 12, 2014
## @copyright Copyright © 2014, 2015 Gilbarco Inc. Confidential and Proprietary.
##-#######################################################################-##

cmake_minimum_required(VERSION 2.8.1 FATAL_ERROR)

if (NOT DEFINED GVR_MODULE_PATH)
  message(FATAL_ERROR "GVR_MODULE_PATH not defined.")
endif (NOT DEFINED GVR_MODULE_PATH)

############### Project Definition ###############
project(libgvrspot)

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
  gvr_import_check(libgvrspot PATHS ${GVR_REMOTE_IMPORT_LOCATIONS} PATH_SUFFIXES ${GVR_IMPORT_PATH_SUFFIXES})
endif (NOT GVR_DISABLE_IMPORT)
if (libgvrspot_IMPORTABLE)
  return()
endif (libgvrspot_IMPORTABLE)

if (GVR_RUN_TEST)
  enable_testing()
endif (GVR_RUN_TEST)

############### Project General Information ###############
set(${PROJECT_NAME}_BUNDLE_NAME "gvrspot")

set(${PROJECT_NAME}_MAJOR_VERSION 0)
set(${PROJECT_NAME}_MINOR_VERSION 8)
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

if (NOT libopenssl_3rdparty_IMPORTED_EXTERNAL)
  message(FATAL_ERROR "Package libopenssl_3rdparty should be imported as an external library.")
endif (NOT libopenssl_3rdparty_IMPORTED_EXTERNAL)

if (NOT GVR_DISABLE_IMPORT)
  find_package(libpoco_3rdparty 1.4.7 NO_MODULE QUIET NO_DEFAULT_PATH PATHS ${GVR_REMOTE_IMPORT_LOCATIONS} PATH_SUFFIXES ${GVR_IMPORT_PATH_SUFFIXES})
endif (NOT GVR_DISABLE_IMPORT)
find_package(libpoco_3rdparty 1.4.7 NO_MODULE QUIET NO_DEFAULT_PATH PATHS ${GVR_LOCAL_IMPORT_LOCATIONS})
find_package(libpoco_3rdparty 1.4.7 NO_MODULE REQUIRED)

if (NOT libpoco_3rdparty_IMPORTED_EXTERNAL)
  message(FATAL_ERROR "Package libpoco_3rdparty should be imported as an external library.")
endif (NOT libpoco_3rdparty_IMPORTED_EXTERNAL)

if (NOT GVR_DISABLE_IMPORT)
  find_package(libgvrcommunication 0.0.0 NO_MODULE QUIET NO_DEFAULT_PATH PATHS ${GVR_REMOTE_IMPORT_LOCATIONS} PATH_SUFFIXES ${GVR_IMPORT_PATH_SUFFIXES})
endif (NOT GVR_DISABLE_IMPORT)
find_package(libgvrcommunication 0.0.0 NO_MODULE QUIET NO_DEFAULT_PATH PATHS ${GVR_LOCAL_IMPORT_LOCATIONS})
find_package(libgvrcommunication 0.0.0 NO_MODULE REQUIRED)

if (NOT libgvrcommunication_IMPORTED)
  message(FATAL_ERROR "Package libgvrcommunication should be imported as an external library.")
endif (NOT libgvrcommunication_IMPORTED)

############### Project Check Version ###############
gvr_check_version(PACKAGE ${PROJECT_NAME} VERSION ${${PROJECT_NAME}_VERSION} CHANGELOG "${PROJECT_SOURCE_DIR}/ChangeLog")

############### Project Build Process ###############
add_definitions(${GVR_BUILD_FLAGS})

include_directories("${PROJECT_SOURCE_DIR}/include"
                    "${libopenssl_3rdparty_IMPORTED_INCLUDES}"
                    "${libpoco_3rdparty_IMPORTED_INCLUDES}"
                    "${libgvrcommunication_IMPORTED_INCLUDES}")

link_directories("${libopenssl_3rdparty_IMPORTED_LINK_LOCATIONS}"
                 "${libpoco_3rdparty_IMPORTED_LINK_LOCATIONS}"
                 "${libgvrcommunication_IMPORTED_LINK_LOCATIONS}")

if (NOT TARGET gvrspot)
  set(${PROJECT_NAME}_TARGET gvrspot)

  add_library(${${PROJECT_NAME}_TARGET}
                                        ${GVR_BUILD_LIBRARY_TYPE}
                                        "${PROJECT_SOURCE_DIR}/src/SpotMessageSpec.cpp"
                                        "${PROJECT_SOURCE_DIR}/src/ISpotMessage.cpp"
                                        "${PROJECT_SOURCE_DIR}/src/ISpotSessionListener.cpp"
                                        "${PROJECT_SOURCE_DIR}/src/ISpotMessageWithAppCatCmdID.cpp"
                                        "${PROJECT_SOURCE_DIR}/src/AbsSpotMessageWithAppCatCmdID.cpp"
                                        "${PROJECT_SOURCE_DIR}/src/AbsSpotTransferFlow.cpp"
                                        "${PROJECT_SOURCE_DIR}/src/AbsColumnSeparatedValuesConfigurationFile.cpp"
                                        "${PROJECT_SOURCE_DIR}/src/SpotDeviceEventFilter.cpp"
                                        "${PROJECT_SOURCE_DIR}/src/UploadableFileList.cpp"
                                        "${PROJECT_SOURCE_DIR}/src/SPOTFileDownloadFlow.cpp"
                                        "${PROJECT_SOURCE_DIR}/src/SPOTFileUploadFlow.cpp"
                                        "${PROJECT_SOURCE_DIR}/src/AbsSpotPackage.cpp"
                                        "${PROJECT_SOURCE_DIR}/src/M3M5SpotPackage.cpp"
                                        "${PROJECT_SOURCE_DIR}/src/UnsignedBinaryFile.cpp"
                                        "${PROJECT_SOURCE_DIR}/src/SpotResourceFile.cpp"
                                        "${PROJECT_SOURCE_DIR}/src/ISpotClassifyMessage.cpp"
                                        "${PROJECT_SOURCE_DIR}/src/AbsSpotClassifyMessageHeader.cpp"
                                        "${PROJECT_SOURCE_DIR}/src/SpotClassifyMessagePing.cpp"
                                        "${PROJECT_SOURCE_DIR}/src/SpotClassifyMessageLogin.cpp"
                                        "${PROJECT_SOURCE_DIR}/src/SpotClassifyMessagePingResponse.cpp"
                                        "${PROJECT_SOURCE_DIR}/src/SpotClassifyMessageLoginResponse.cpp"
                                        "${PROJECT_SOURCE_DIR}/src/SpotClassifyMessageHardwareInfo.cpp"
                                        "${PROJECT_SOURCE_DIR}/src/SpotClassifyMessageHardwareInfoResponse.cpp"
                                        "${PROJECT_SOURCE_DIR}/src/SpotClassifyMessageSoftwareInfo.cpp"
                                        "${PROJECT_SOURCE_DIR}/src/SpotClassifyMessageSoftwareInfoResponse.cpp"
                                        "${PROJECT_SOURCE_DIR}/src/SpotClassifyMessageOPTHwrInfoResponse.cpp"
                                        "${PROJECT_SOURCE_DIR}/src/SpotClassifyMessageOptSwInfo.cpp"
                                        "${PROJECT_SOURCE_DIR}/src/SpotClassifyMessageOptSwInfoResponse.cpp"
                                        "${PROJECT_SOURCE_DIR}/src/SpotClassifyMessagePackageInfo.cpp"
                                        "${PROJECT_SOURCE_DIR}/src/SpotClassifyMessagePackageInfoResponse.cpp"
                                        "${PROJECT_SOURCE_DIR}/src/SpotClassifyMessagePackageDownload.cpp"
                                        "${PROJECT_SOURCE_DIR}/src/SpotClassifyMessagePackageDownloadResponse.cpp"
                                        "${PROJECT_SOURCE_DIR}/src/SpotClassifyMessagePackageBlockDownload.cpp"
                                        "${PROJECT_SOURCE_DIR}/src/SpotClassifyMessageVgdProcessPackageResponse.cpp"
                                        "${PROJECT_SOURCE_DIR}/src/SpotClassifyMessageVGDFileTransferResponse.cpp"
                                        "${PROJECT_SOURCE_DIR}/src/SpotClassifyMessagePackageBlockDownloadResponse.cpp"
                                        "${PROJECT_SOURCE_DIR}/src/SpotClassifyMessageVGDFileTransferResponse.cpp"
                                        "${PROJECT_SOURCE_DIR}/src/SpotSession.cpp"
                                        "${PROJECT_SOURCE_DIR}/src/SpotMessageLogin.cpp"
                                        "${PROJECT_SOURCE_DIR}/src/SpotMessageLoginResponse.cpp"
                                        "${PROJECT_SOURCE_DIR}/src/SpotMessagePing.cpp"
                                        "${PROJECT_SOURCE_DIR}/src/SpotMessageReset.cpp"
                                        "${PROJECT_SOURCE_DIR}/src/SpotMessagePingResponse.cpp"
                                        "${PROJECT_SOURCE_DIR}/src/SpotMessageHardwareInfo.cpp"
                                        "${PROJECT_SOURCE_DIR}/src/SpotMessageHardwareInfoResponse.cpp"
                                        "${PROJECT_SOURCE_DIR}/src/SpotMessageSoftwareInfo.cpp"
                                        "${PROJECT_SOURCE_DIR}/src/SpotMessageSoftwareInfoResponse.cpp"
                                        "${PROJECT_SOURCE_DIR}/src/SpotMessageOPTHwrInfo.cpp"
                                        "${PROJECT_SOURCE_DIR}/src/SpotMessageOPTHwrInfoResponse.cpp"
                                        "${PROJECT_SOURCE_DIR}/src/SpotMessageOptSwInfo.cpp"
                                        "${PROJECT_SOURCE_DIR}/src/SpotMessageOptSwInfoResponse.cpp"
                                        "${PROJECT_SOURCE_DIR}/src/SpotMessagePackageInfo.cpp"
                                        "${PROJECT_SOURCE_DIR}/src/SpotMessagePackageInfoResponse.cpp"
                                        "${PROJECT_SOURCE_DIR}/src/SpotMessagePackageDownload.cpp"
                                        "${PROJECT_SOURCE_DIR}/src/SpotMessagePackageDownloadResponse.cpp"
                                        "${PROJECT_SOURCE_DIR}/src/SpotMessagePackageBlockDownload.cpp"
                                        "${PROJECT_SOURCE_DIR}/src/SpotMessagePackageBlockDownloadResponse.cpp"
                                        "${PROJECT_SOURCE_DIR}/src/SpotMessageVGDFileTransfer.cpp"
                                        "${PROJECT_SOURCE_DIR}/src/SpotMessageVGDFileTransferResponse.cpp"
                                        "${PROJECT_SOURCE_DIR}/src/SpotMessageVgdProcessPackage.cpp"
                                        "${PROJECT_SOURCE_DIR}/src/SpotMessageVgdProcessPackageResponse.cpp"
                                        "${PROJECT_SOURCE_DIR}/src/SpotChannel.cpp"
                                        "${PROJECT_SOURCE_DIR}/src/SpotMessageSystemStatusResponse.cpp"
                                        "${PROJECT_SOURCE_DIR}/src/SpotMessageSystemStatus.cpp"
                                        "${PROJECT_SOURCE_DIR}/src/SpotClassifyMessageSystemStatusResponse.cpp"
                                        "${PROJECT_SOURCE_DIR}/src/SpotMessageDebianInfo.cpp"
                                        "${PROJECT_SOURCE_DIR}/src/SpotMessageDebianInfoResponse.cpp"
                                        "${PROJECT_SOURCE_DIR}/src/SpotClassifyMessageDebianInfo.cpp"
                                        "${PROJECT_SOURCE_DIR}/src/SpotClassifyMessageDebianInfoResponse.cpp"
                                        "${PROJECT_SOURCE_DIR}/src/SpotClassifyMessagePackageActivation.cpp"
                                        "${PROJECT_SOURCE_DIR}/src/SpotClassifyMessagePackageActivationResponse"
                                        "${PROJECT_SOURCE_DIR}/src/SpotClassifyMessageVgdPackageActivation.cpp"
                                        "${PROJECT_SOURCE_DIR}/src/SpotClassifyMessageVgdPackageActivationResponse"
                                        "${PROJECT_SOURCE_DIR}/src/SpotMessagePackageActivationResponse.cpp"
                                        "${PROJECT_SOURCE_DIR}/src/SpotMessagePackageActivation.cpp"
                                        "${PROJECT_SOURCE_DIR}/src/SpotMessageFileDownload.cpp"
                                        "${PROJECT_SOURCE_DIR}/src/SpotMessageFileDownloadResponse.cpp"
                                        "${PROJECT_SOURCE_DIR}/src/SpotClassifyMessageFileDownload.cpp"
                                        "${PROJECT_SOURCE_DIR}/src/SpotClassifyMessageFileDownloadResponse.cpp"
                                        "${PROJECT_SOURCE_DIR}/src/SpotMessageFileDelete.cpp"
                                        "${PROJECT_SOURCE_DIR}/src/SpotMessageFileDeleteResponse.cpp"
                                        "${PROJECT_SOURCE_DIR}/src/SpotClassifyMessageFileDelete.cpp"
                                        "${PROJECT_SOURCE_DIR}/src/SpotClassifyMessageFileDeleteResponse.cpp"
                                        "${PROJECT_SOURCE_DIR}/src/TagLengthValueVector.cpp"
                                        "${PROJECT_SOURCE_DIR}/src/Conversion.cpp"
                                        "${PROJECT_SOURCE_DIR}/src/Utils.cpp"
                                        "${PROJECT_SOURCE_DIR}/src/SpotMessageFileBlockDownload.cpp"
                                        "${PROJECT_SOURCE_DIR}/src/SpotMessageFileBlockDownloadResponse.cpp"
                                        "${PROJECT_SOURCE_DIR}/src/SpotClassifyMessageFileBlockDownload.cpp"
                                        "${PROJECT_SOURCE_DIR}/src/SpotClassifyMessageFileBlockDownloadResponse.cpp"
                                        "${PROJECT_SOURCE_DIR}/src/SpotClassifyMessageNewEphemeralKey.cpp"
                                        "${PROJECT_SOURCE_DIR}/src/SpotClassifyMessageNewEphemeralKeyResponse.cpp"
                                        "${PROJECT_SOURCE_DIR}/src/SpotMessageNewEphemeralKey.cpp"
                                        "${PROJECT_SOURCE_DIR}/src/SpotMessageNewEphemeralKeyResponse.cpp"
                                        "${PROJECT_SOURCE_DIR}/src/SpotClassifyMessageEphemeralEncrypt.cpp"
                                        "${PROJECT_SOURCE_DIR}/src/SpotClassifyMessageEphemeralEncryptResponse.cpp"
                                        "${PROJECT_SOURCE_DIR}/src/SpotMessageEphemeralEncrypt.cpp"
                                        "${PROJECT_SOURCE_DIR}/src/SpotMessageEphemeralEncryptResponse.cpp"
                                        "${PROJECT_SOURCE_DIR}/src/SpotClassifyMessageFileUpload.cpp"
                                        "${PROJECT_SOURCE_DIR}/src/SpotClassifyMessageFileUploadResponse.cpp"
                                        "${PROJECT_SOURCE_DIR}/src/SpotClassifyMessageFileBlockUpload.cpp"
                                        "${PROJECT_SOURCE_DIR}/src/SpotClassifyMessageFileBlockUploadResponse.cpp"
                                        "${PROJECT_SOURCE_DIR}/src/SpotMessageFileUpload.cpp"
                                        "${PROJECT_SOURCE_DIR}/src/SpotMessageFileUploadResponse.cpp"
                                        "${PROJECT_SOURCE_DIR}/src/SpotMessageFileBlockUpload.cpp"
                                        "${PROJECT_SOURCE_DIR}/src/SpotMessageFileBlockUploadResponse.cpp"
                                        )

  set_target_properties(${${PROJECT_NAME}_TARGET}
                                                  PROPERTIES
                                                             VERSION ${${PROJECT_NAME}_VERSION}
                                                             SOVERSION 0)

  # Need to add 'd' at the end of the library name if build for debug.
  set(${PROJECT_NAME}_POCO_LIBS PocoNet PocoFoundation)
  if (GVR_DEBUG)
    set(${PROJECT_NAME}_POCO_LIBS_AUX ${${PROJECT_NAME}_POCO_LIBS})
    set(${PROJECT_NAME}_POCO_LIBS "")
    foreach (${PROJECT_NAME}_POCO_LIB ${${PROJECT_NAME}_POCO_LIBS_AUX})
      set(${PROJECT_NAME}_POCO_LIBS ${${PROJECT_NAME}_POCO_LIBS} "${${PROJECT_NAME}_POCO_LIB}d")
    endforeach (${PROJECT_NAME}_POCO_LIB)
    set(${PROJECT_NAME}_POCO_LIBS_AUX "")
  endif (GVR_DEBUG)

  target_link_libraries(${${PROJECT_NAME}_TARGET}
                                                  gvrcommunication
                                                  ${${PROJECT_NAME}_POCO_LIBS}
                                                  ssl crypto
                                                  ${libpoco_3rdparty_OPTIONAL_DYNAMIC_LINK_FLAGS}
                                                  ${CMAKE_THREAD_LIBS_INIT})

  if (GVR_CHECK_REVISION)
    add_dependencies(${${PROJECT_NAME}_TARGET}
                                               ${GVR_CHECK_REVISION}
                                               gvrcommunication
                                               ${${PROJECT_NAME}_POCO_LIBS}
                                               ssl crypto
                                               ${libpoco_3rdparty_OPTIONAL_DYNAMIC_LINK_FLAGS}
                                               ${CMAKE_THREAD_LIBS_INIT})
  endif (GVR_CHECK_REVISION)

  set(${PROJECT_NAME}_EXPORTABLE_TARGETS ${${PROJECT_NAME}_EXPORTABLE_TARGETS} ${${PROJECT_NAME}_TARGET})

  set(${PROJECT_NAME}_INSTALLABLE_TARGETS ${${PROJECT_NAME}_INSTALLABLE_TARGETS} ${${PROJECT_NAME}_TARGET})
endif (NOT TARGET gvrspot)

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

gvr_export_build_bundle(PROJECT ${PROJECT_NAME} BUNDLES gvr spot)

############### Project Test Process ###############
add_subdirectory(test)
