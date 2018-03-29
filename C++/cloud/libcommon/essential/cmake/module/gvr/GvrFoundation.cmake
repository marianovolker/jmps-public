##-#######################################################################-##
# Copyright © 2014, 2015 Gilbarco Inc.
# Confidential and Proprietary.
#
## @file GvrFoundation.cmake
## @author pcantarini
## @date Tuesday, April 01, 2014
## @copyright Copyright © 2014, 2015 Gilbarco Inc. Confidential and Proprietary.
##-#######################################################################-##

cmake_minimum_required(VERSION 2.8.1 FATAL_ERROR)

if (NOT DEFINED GVR_FOUNDATION_INCLUDED)
  set(GVR_FOUNDATION_INCLUDED ON)

  ############### GVR Build Information ###############
  if (NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE DEBUG)
  else (NOT CMAKE_BUILD_TYPE)
    string(TOUPPER ${CMAKE_BUILD_TYPE} CMAKE_BUILD_TYPE)
  endif (NOT CMAKE_BUILD_TYPE)

  ############### GVR Configuration Information ###############
  set(GVR_TMP_PATHS ${CMAKE_MODULE_PATH})
  set(CMAKE_MODULE_PATH "")
  foreach (GVR_TMP_PATH ${GVR_TMP_PATHS})
    file(TO_CMAKE_PATH ${GVR_TMP_PATH} GVR_TMP_PATH)
    set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${GVR_TMP_PATH})
  endforeach (GVR_TMP_PATH)
  set(GVR_TMP_PATHS "")
  ###############
  if (GVR_MODULE_PATH)
    file(TO_CMAKE_PATH ${GVR_MODULE_PATH} GVR_MODULE_PATH)
    set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${GVR_MODULE_PATH}
                                               "${GVR_MODULE_PATH}/gvr"
                                               "${GVR_MODULE_PATH}/opensource"
                                               "${GVR_MODULE_PATH}/opensource/cmake/Modules")
  else (GVR_MODULE_PATH)
    set(GVR_MODULE_PATH "")
  endif (GVR_MODULE_PATH)

  ############### GVR Repository Information ###############
  if (NOT DEFINED GVR_REPOSITORY_LOCAL)
    SET(GVR_REPOSITORY_LOCAL "${CMAKE_BINARY_DIR}/repository_local")
  endif (NOT DEFINED GVR_REPOSITORY_LOCAL)
  ###############
  if (NOT DEFINED GVR_REPOSITORY_REMOTE)
    SET(GVR_REPOSITORY_REMOTE "${CMAKE_SOURCE_DIR}/.repository")
  endif (NOT DEFINED GVR_REPOSITORY_REMOTE)

  ############### GVR Provider Information ###############
  if (NOT DEFINED GVR_PROVIDER_NAME)
    SET(GVR_PROVIDER_NAME "gilbarco")
  endif (NOT DEFINED GVR_PROVIDER_NAME)
  ###############
  if (NOT DEFINED GVR_REPOSITORY_LOCAL_PROVIDER)
    SET(GVR_REPOSITORY_LOCAL_PROVIDER "${GVR_REPOSITORY_LOCAL}/${GVR_PROVIDER_NAME}")
  endif (NOT DEFINED GVR_REPOSITORY_LOCAL_PROVIDER)
  ###############
  if (NOT DEFINED GVR_REPOSITORY_REMOTE_PROVIDER)
    SET(GVR_REPOSITORY_REMOTE_PROVIDER ${GVR_REPOSITORY_REMOTE})
  endif (NOT DEFINED GVR_REPOSITORY_REMOTE_PROVIDER)

  ############### GVR Import Information ###############
  set(GVR_TMP_PATHS ${CMAKE_PREFIX_PATH})
  set(CMAKE_PREFIX_PATH "")
  foreach (GVR_TMP_PATH ${GVR_TMP_PATHS})
    file(TO_CMAKE_PATH ${GVR_TMP_PATH} GVR_TMP_PATH)
    set(CMAKE_PREFIX_PATH ${CMAKE_PREFIX_PATH} ${GVR_TMP_PATH})
  endforeach (GVR_TMP_PATH)
  set(GVR_TMP_PATHS "")
  ###############
  set(GVR_TMP_PATHS ${CMAKE_INCLUDE_PATH})
  set(CMAKE_INCLUDE_PATH "")
  foreach (GVR_TMP_PATH ${GVR_TMP_PATHS})
    file(TO_CMAKE_PATH ${GVR_TMP_PATH} GVR_TMP_PATH)
    set(CMAKE_INCLUDE_PATH ${CMAKE_INCLUDE_PATH} ${GVR_TMP_PATH})
  endforeach (GVR_TMP_PATH)
  set(GVR_TMP_PATHS "")
  ###############
  set(GVR_TMP_PATHS ${CMAKE_LIBRARY_PATH})
  set(CMAKE_LIBRARY_PATH "")
  foreach (GVR_TMP_PATH ${GVR_TMP_PATHS})
    file(TO_CMAKE_PATH ${GVR_TMP_PATH} GVR_TMP_PATH)
    set(CMAKE_LIBRARY_PATH ${CMAKE_LIBRARY_PATH} ${GVR_TMP_PATH})
  endforeach (GVR_TMP_PATH)
  set(GVR_TMP_PATHS "")
  ###############
  set(GVR_TMP_PATHS ${CMAKE_PROGRAM_PATH})
  set(CMAKE_PROGRAM_PATH "")
  foreach (GVR_TMP_PATH ${GVR_TMP_PATHS})
    file(TO_CMAKE_PATH ${GVR_TMP_PATH} GVR_TMP_PATH)
    set(CMAKE_PROGRAM_PATH ${CMAKE_PROGRAM_PATH} ${GVR_TMP_PATH})
  endforeach (GVR_TMP_PATH)
  set(GVR_TMP_PATHS "")
  ###############
  if (NOT DEFINED GVR_LOCAL_IMPORT_LOCATIONS)
    set(GVR_LOCAL_IMPORT_LOCATIONS "${GVR_REPOSITORY_LOCAL_PROVIDER}/import")
  endif (NOT DEFINED GVR_LOCAL_IMPORT_LOCATIONS)
  set(GVR_TMP_PATHS ${GVR_LOCAL_IMPORT_LOCATIONS})
  set(GVR_LOCAL_IMPORT_LOCATIONS "")
  foreach (GVR_TMP_PATH ${GVR_TMP_PATHS})
    file(TO_CMAKE_PATH ${GVR_TMP_PATH} GVR_TMP_PATH)
    set(GVR_LOCAL_IMPORT_LOCATIONS ${GVR_LOCAL_IMPORT_LOCATIONS} ${GVR_TMP_PATH})
    file(REMOVE_RECURSE ${GVR_TMP_PATH})
  endforeach (GVR_TMP_PATH)
  set(GVR_TMP_PATHS "")
  ###############
  if (NOT DEFINED GVR_REMOTE_IMPORT_LOCATIONS)
    set(GVR_REMOTE_IMPORT_LOCATIONS "${CMAKE_SOURCE_DIR}/.import")
  endif (NOT DEFINED GVR_REMOTE_IMPORT_LOCATIONS)
  set(GVR_TMP_PATHS ${GVR_REMOTE_IMPORT_LOCATIONS})
  set(GVR_REMOTE_IMPORT_LOCATIONS "")
  foreach (GVR_TMP_PATH ${GVR_TMP_PATHS})
    file(TO_CMAKE_PATH ${GVR_TMP_PATH} GVR_TMP_PATH)
    set(GVR_REMOTE_IMPORT_LOCATIONS ${GVR_REMOTE_IMPORT_LOCATIONS} ${GVR_TMP_PATH})
  endforeach (GVR_TMP_PATH)
  set(GVR_TMP_PATHS "")
  ###############
  if (NOT GVR_DISABLE_IMPORT)
    set(GVR_DISABLE_IMPORT OFF)
  endif (NOT GVR_DISABLE_IMPORT)
  if (NOT GVR_AUTO_IMPORTED_EXTERNAL_PREFIX_LOCATION)
    set(GVR_AUTO_IMPORTED_EXTERNAL_PREFIX_LOCATION OFF)
  else (NOT GVR_AUTO_IMPORTED_EXTERNAL_PREFIX_LOCATION)
    if (NOT GVR_AUTO_IMPORTED_EXTERNAL_PREFIX_LOCATION_REGEX_PAIR)
      set(GVR_AUTO_IMPORTED_EXTERNAL_PREFIX_LOCATION_REGEX_PAIR "^(.*)/import(/?|/.*)$" "\\1")
    endif (NOT GVR_AUTO_IMPORTED_EXTERNAL_PREFIX_LOCATION_REGEX_PAIR)
  endif (NOT GVR_AUTO_IMPORTED_EXTERNAL_PREFIX_LOCATION)
  if (NOT GVR_AUTO_IMPORTED_EXTERNAL_PREFIX_LOCATION_REGEX_PAIR)
    set(GVR_AUTO_IMPORTED_EXTERNAL_PREFIX_LOCATION_REGEX_PAIR "")
  endif (NOT GVR_AUTO_IMPORTED_EXTERNAL_PREFIX_LOCATION_REGEX_PAIR)

  ###############
  if (NOT GVR_EXPORT_BUILD_IMPORT_PATH_SUFFIX)
    if (CMAKE_BUILD_TYPE)
      set(GVR_EXPORT_BUILD_IMPORT_PATH_SUFFIX "import/${CMAKE_BUILD_TYPE}")
    else (CMAKE_BUILD_TYPE)
      set(GVR_EXPORT_BUILD_IMPORT_PATH_SUFFIX "import")
    endif (CMAKE_BUILD_TYPE)
  endif (NOT GVR_EXPORT_BUILD_IMPORT_PATH_SUFFIX)
  if (NOT GVR_IMPORT_PATH_SUFFIXES)
    set(GVR_IMPORT_PATH_SUFFIXES ${GVR_EXPORT_BUILD_IMPORT_PATH_SUFFIX})
  endif (NOT GVR_IMPORT_PATH_SUFFIXES)
  ###############
  function(GVR_IMPORT_CHECK GVR_IMPORT_CHECK_PROJECT)
    include(CMakeParseArguments)
    set(GVR_FLAGS "")
    set(GVR_SINGLE_VALUE "")
    set(GVR_MULTI_VALUE PATHS PATH_SUFFIXES)
    cmake_parse_arguments(GVR_IMPORT_CHECK "${GVR_FLAGS}" "${GVR_SINGLE_VALUE}" "${GVR_MULTI_VALUE}" ${ARGN})

    if (GVR_IMPORT_CHECK_PATHS)
      ###############
      set(GVR_IMPORT_CHECK_TMPS ${GVR_IMPORT_CHECK_PATHS})
      set(GVR_IMPORT_CHECK_PATHS "")
      foreach (GVR_IMPORT_CHECK_TMP ${GVR_IMPORT_CHECK_TMPS})
        set(GVR_IMPORT_CHECK_PATHS ${GVR_IMPORT_CHECK_PATHS} ${GVR_IMPORT_CHECK_TMP})
      endforeach (GVR_IMPORT_CHECK_TMP)
      set(GVR_IMPORT_CHECK_TMPS "")
      ###############
      set(GVR_IMPORT_CHECK_PATHS PATHS ${GVR_IMPORT_CHECK_PATHS})
    else (GVR_IMPORT_CHECK_PATHS)
      set(GVR_IMPORT_CHECK_PATHS "")
    endif (GVR_IMPORT_CHECK_PATHS)

    if (GVR_IMPORT_CHECK_PATH_SUFFIXES)
      ###############
      set(GVR_IMPORT_CHECK_TMPS ${GVR_IMPORT_CHECK_PATH_SUFFIXES})
      set(GVR_IMPORT_CHECK_PATH_SUFFIXES "")
      foreach (GVR_IMPORT_CHECK_TMP ${GVR_IMPORT_CHECK_TMPS})
        set(GVR_IMPORT_CHECK_PATH_SUFFIXES ${GVR_IMPORT_CHECK_PATH_SUFFIXES} ${GVR_IMPORT_CHECK_TMP})
      endforeach (GVR_IMPORT_CHECK_TMP)
      set(GVR_IMPORT_CHECK_TMPS "")
      ###############
      set(GVR_IMPORT_CHECK_PATH_SUFFIXES PATH_SUFFIXES ${GVR_IMPORT_CHECK_PATH_SUFFIXES})
    else (GVR_IMPORT_CHECK_PATH_SUFFIXES)
      set(GVR_IMPORT_CHECK_PATH_SUFFIXES "")
    endif (GVR_IMPORT_CHECK_PATH_SUFFIXES)

    find_package(${GVR_IMPORT_CHECK_PROJECT} ${GVR_IMPORT_CHECK_PATH_SUFFIXES} ${GVR_IMPORT_CHECK_UNPARSED_ARGUMENTS} QUIET NO_DEFAULT_PATH ${GVR_IMPORT_CHECK_PATHS})
    if (${GVR_IMPORT_CHECK_PROJECT}_FOUND)
      set(${GVR_IMPORT_CHECK_PROJECT}_IMPORTABLE ON PARENT_SCOPE)
    else (${GVR_IMPORT_CHECK_PROJECT}_FOUND)
      set(${GVR_IMPORT_CHECK_PROJECT}_IMPORTABLE PARENT_SCOPE)
    endif (${GVR_IMPORT_CHECK_PROJECT}_FOUND)
  endfunction(GVR_IMPORT_CHECK)

  ############### GVR Export Information ###############
  if (NOT DEFINED GVR_LOCAL_EXPORT_LOCATION)
    if (GVR_LOCAL_IMPORT_LOCATIONS)
      list(GET GVR_LOCAL_IMPORT_LOCATIONS 0 GVR_LOCAL_EXPORT_LOCATION)
    else (GVR_LOCAL_IMPORT_LOCATIONS)
      set(GVR_LOCAL_EXPORT_LOCATION "${GVR_REPOSITORY_LOCAL_PROVIDER}/import")
    endif (GVR_LOCAL_IMPORT_LOCATIONS)
  endif (NOT DEFINED GVR_LOCAL_EXPORT_LOCATION)
  file(TO_CMAKE_PATH ${GVR_LOCAL_EXPORT_LOCATION} GVR_LOCAL_EXPORT_LOCATION)
  ###############
  if (NOT DEFINED GVR_REMOTE_EXPORT_LOCATION)
    set(GVR_REMOTE_EXPORT_LOCATION "${GVR_REPOSITORY_REMOTE}/export")
  endif (NOT DEFINED GVR_REMOTE_EXPORT_LOCATION)
  file(TO_CMAKE_PATH ${GVR_REMOTE_EXPORT_LOCATION} GVR_REMOTE_EXPORT_LOCATION)

  ############### GVR Temporary Information ###############
  if (NOT DEFINED GVR_TEMPORARY_LOCATION)
    set(GVR_TEMPORARY_LOCATION "${GVR_REPOSITORY_LOCAL_PROVIDER}/temporary")
  endif (NOT DEFINED GVR_TEMPORARY_LOCATION)
  file(TO_CMAKE_PATH ${GVR_TEMPORARY_LOCATION} GVR_TEMPORARY_LOCATION)
  file(REMOVE_RECURSE ${GVR_TEMPORARY_LOCATION})

  ############### GVR Documentation Information ###############
  if (NOT DEFINED GVR_DOCUMENTATION_LOCATION)
    set(GVR_DOCUMENTATION_LOCATION "${GVR_REPOSITORY_REMOTE}/documentation")
  endif (NOT DEFINED GVR_DOCUMENTATION_LOCATION)
  file(TO_CMAKE_PATH ${GVR_DOCUMENTATION_LOCATION} GVR_DOCUMENTATION_LOCATION)

  ############### GVR Install Information ###############
  if (NOT GVR_INSTALL_PREFIX)
    set(GVR_INSTALL_PREFIX "")
    set(CMAKE_INSTALL_PREFIX "${GVR_REPOSITORY_REMOTE}/install")
  else (NOT GVR_INSTALL_PREFIX)
    set(CMAKE_INSTALL_PREFIX ${GVR_INSTALL_PREFIX})
  endif (NOT GVR_INSTALL_PREFIX)
  file(TO_CMAKE_PATH ${CMAKE_INSTALL_PREFIX} CMAKE_INSTALL_PREFIX)
  ###############
  if (NOT DEFINED GVR_PACKAGE_LOCATION)
    set(GVR_PACKAGE_LOCATION "${GVR_REPOSITORY_REMOTE}/packages")
  endif (NOT DEFINED GVR_PACKAGE_LOCATION)
  file(TO_CMAKE_PATH ${GVR_PACKAGE_LOCATION} GVR_PACKAGE_LOCATION)
  ###############
  if (NOT DEFINED GVR_INSTALL_PROVIDER_RELATIVE_LOCATION)
    set(GVR_INSTALL_PROVIDER_RELATIVE_LOCATION "opt/${GVR_PROVIDER_NAME}")
  endif (NOT DEFINED GVR_INSTALL_PROVIDER_RELATIVE_LOCATION)
  file(TO_CMAKE_PATH ${GVR_INSTALL_PROVIDER_RELATIVE_LOCATION} GVR_INSTALL_PROVIDER_RELATIVE_LOCATION)

  ############### GVR Custom Install Information ###############
  if (NOT TARGET install-3rdparty)
    add_custom_target(install-3rdparty
                        COMMAND ${CMAKE_COMMAND} -DCOMPONENT=ThirdParty -DCMAKE_INSTALL_DO_STRIP=1 -P cmake_install.cmake)
  endif (NOT TARGET install-3rdparty)
  ###############
  if (NOT TARGET install-runtime)
    add_custom_target(install-runtime
                        COMMAND ${CMAKE_COMMAND} -DCOMPONENT=Runtime -DCMAKE_INSTALL_DO_STRIP=1 -P cmake_install.cmake)
  endif (NOT TARGET install-runtime)

  ############### GVR Debug Information ###############
  if (CMAKE_BUILD_TYPE STREQUAL RELEASE)
    set(GVR_DEBUG OFF)
  endif (CMAKE_BUILD_TYPE STREQUAL RELEASE)

  if (NOT GVR_DEBUG)
    set(GVR_DEBUG OFF)
  endif (NOT GVR_DEBUG)

  message(STATUS "Debug mode is '${GVR_DEBUG}'.")

  if (NOT DEFINED GVR_DEBUG_FLAGS)
    set(GVR_DEBUG_FLAGS "-ggdb3" "-O0")
  endif (NOT DEFINED GVR_DEBUG_FLAGS)

  ############### GVR Release Information ###############
  if (NOT DEFINED GVR_RELEASE_FLAGS)
    set(GVR_RELEASE_FLAGS "-O3")
  endif (NOT DEFINED GVR_RELEASE_FLAGS)

  ############### GVR Revision Information ###############
  if (NOT DEFINED GVR_USE_REVISION)
    set(GVR_USE_REVISION OFF)
  endif (NOT DEFINED GVR_USE_REVISION)

  if (GVR_USE_REVISION)
    if (NOT DEFINED GVR_VCS_COMMAND)
      find_program(GVR_VCS_COMMAND NAMES svn)
    endif (NOT DEFINED GVR_VCS_COMMAND)
    if (NOT DEFINED GVR_VCS_GET_REVISION_COMMAND)
      find_program(GVR_VCS_GET_REVISION_COMMAND NAMES svnversion)
    endif (NOT DEFINED GVR_VCS_GET_REVISION_COMMAND)
    if ((NOT GVR_VCS_COMMAND) OR (NOT GVR_VCS_GET_REVISION_COMMAND))
      message(WARNING "Unable to find 'Version Control System' commands - VCS '${GVR_VCS_COMMAND}' - VCSGetRevision '${GVR_VCS_GET_REVISION_COMMAND}'.")
      set(GVR_USE_REVISION OFF)
    endif ((NOT GVR_VCS_COMMAND) OR (NOT GVR_VCS_GET_REVISION_COMMAND))
  endif (GVR_USE_REVISION)

  set(GVR_REVISION_FILE_IN "${CMAKE_BINARY_DIR}/revision.in")
  set(GVR_REVISION_FILE_TMP "${CMAKE_BINARY_DIR}/revision.tmp")
  set(GVR_REVISION_FILE_STAMP "${CMAKE_BINARY_DIR}/revision")

  if (GVR_USE_REVISION)
    set(GVR_REVISION "")
    file(REMOVE ${GVR_REVISION_FILE_IN})

    execute_process(COMMAND ${GVR_VCS_GET_REVISION_COMMAND} ${CMAKE_SOURCE_DIR}
                    OUTPUT_FILE ${GVR_REVISION_FILE_IN}
                    ERROR_QUIET)

    if (EXISTS ${GVR_REVISION_FILE_IN})
      if (NOT IS_DIRECTORY ${GVR_REVISION_FILE_IN})
        file(STRINGS ${GVR_REVISION_FILE_IN} GVR_REVISION REGEX "^[ \t]*[0-9]+(:[0-9]+)?[MS]?[ \t]*$")
      endif (NOT IS_DIRECTORY ${GVR_REVISION_FILE_IN})
    endif (EXISTS ${GVR_REVISION_FILE_IN})

    if (GVR_REVISION)
      if (GVR_REVISION MATCHES "^[ \t]*[0-9]+:[0-9]+[MS]?[ \t]*$")
        string(REGEX REPLACE "^[ \t]*([0-9]+):([0-9]+[MS]?)[ \t]*$" "\\1u\\2" GVR_REVISION ${GVR_REVISION})
      else (GVR_REVISION MATCHES "^[ \t]*[0-9]+:[0-9]+[MS]?[ \t]*$")
        string(REGEX REPLACE "^[ \t]*([0-9]+[MS]?)[ \t]*$" "\\1" GVR_REVISION ${GVR_REVISION})
      endif (GVR_REVISION MATCHES "^[ \t]*[0-9]+:[0-9]+[MS]?[ \t]*$")
    endif (GVR_REVISION)
    if (NOT GVR_REVISION)
      message(WARNING "Unable to collect revision information.")
      set(GVR_USE_REVISION OFF)
    endif (NOT GVR_REVISION)
  endif (GVR_USE_REVISION)

  if (GVR_USE_REVISION)
    set(GVR_CHECK_REVISION revision)
    add_custom_target (${GVR_CHECK_REVISION} ALL
                       COMMAND ${GVR_VCS_GET_REVISION_COMMAND} ${CMAKE_SOURCE_DIR} > ${GVR_REVISION_FILE_TMP}
                       COMMAND ${CMAKE_COMMAND} -E copy_if_different ${GVR_REVISION_FILE_TMP} ${GVR_REVISION_FILE_IN})
    configure_file(${GVR_REVISION_FILE_IN} ${GVR_REVISION_FILE_STAMP} COPYONLY)
  endif (GVR_USE_REVISION)

  set(GVR_REVISION_FILE_IN "")
  set(GVR_REVISION_FILE_TMP "")
  set(GVR_REVISION_FILE_STAMP "")

  if (GVR_USE_REVISION)
    message(STATUS "Revision: '${GVR_REVISION}'")
  else (GVR_USE_REVISION)
    set(GVR_VCS_COMMAND "")
    set(GVR_VCS_GET_REVISION_COMMAND "")
    set(GVR_REVISION "")
    set(GVR_CHECK_REVISION "")
    set(GVR_USE_REVISION OFF)
  endif (GVR_USE_REVISION)

  ############### GVR Build Information ###############
  if (NOT GVR_BUILD_FLAGS)
    if (GVR_DEBUG)
      set(GVR_BUILD_FLAGS ${GVR_DEBUG_FLAGS})
    else (GVR_DEBUG)
      set(GVR_BUILD_FLAGS ${GVR_RELEASE_FLAGS})
    endif (GVR_DEBUG)
  else (NOT GVR_BUILD_FLAGS)
    if (GVR_DEBUG)
      set(GVR_BUILD_FLAGS ${GVR_BUILD_FLAGS} ${GVR_DEBUG_FLAGS})
    else (GVR_DEBUG)
      set(GVR_BUILD_FLAGS ${GVR_BUILD_FLAGS} ${GVR_RELEASE_FLAGS})
    endif (GVR_DEBUG)
  endif (NOT GVR_BUILD_FLAGS)

  message(STATUS "Default build flags are '${GVR_BUILD_FLAGS}'.")

  if (NOT DEFINED BUILD_SHARED_LIBS)
    set(BUILD_SHARED_LIBS ON)
    if ((DEFINED GVR_BUILD_LIBRARY_TYPE) AND (NOT (GVR_BUILD_LIBRARY_TYPE STREQUAL SHARED)))
        set(BUILD_SHARED_LIBS OFF)
    endif ((DEFINED GVR_BUILD_LIBRARY_TYPE) AND (NOT (GVR_BUILD_LIBRARY_TYPE STREQUAL SHARED)))
  endif (NOT DEFINED BUILD_SHARED_LIBS)
  if (BUILD_SHARED_LIBS)
    set(GVR_BUILD_LIBRARY_TYPE SHARED)
  else (BUILD_SHARED_LIBS)
    set(GVR_BUILD_LIBRARY_TYPE STATIC)
  endif (BUILD_SHARED_LIBS)

  message(STATUS "Default build library type is '${GVR_BUILD_LIBRARY_TYPE}'.")

  ############### GVR Test Information ###############
  if (NOT DEFINED GVR_RUN_TEST)
    set(GVR_RUN_TEST OFF)
  endif (NOT DEFINED GVR_RUN_TEST)
  if (NOT DEFINED GVR_RUN_VALGRIND_TEST)
    set(GVR_RUN_VALGRIND_TEST OFF)
  endif (NOT DEFINED GVR_RUN_VALGRIND_TEST)

  ############### GVR Test Valgrind Information ###############
  if (GVR_RUN_VALGRIND_TEST)
    if (GVR_RUN_TEST)
      if (GVR_DEBUG)
        if (CMAKE_BUILD_TYPE STREQUAL DEBUG)
          if (NOT DEFINED GVR_VALGRIND_COMMAND)
            find_program(GVR_VALGRIND_COMMAND NAMES valgrind)
          endif (NOT DEFINED GVR_VALGRIND_COMMAND)
          if (GVR_VALGRIND_COMMAND)
            if (NOT GVR_VALGRIND_DEFAULT_ARGUMENTS)
              set(GVR_VALGRIND_DEFAULT_ARGUMENTS "--tool=memcheck" "--leak-check=yes" "--show-reachable=yes" "--track-fds=yes" "--time-stamp=yes" "--error-exitcode=66")
            endif (NOT GVR_VALGRIND_DEFAULT_ARGUMENTS)
          else (GVR_VALGRIND_COMMAND)
            set(GVR_RUN_VALGRIND_TEST OFF)
            message(STATUS "Skipped valgrind tests. Command valgrind not found.")
          endif (GVR_VALGRIND_COMMAND)
        else (CMAKE_BUILD_TYPE STREQUAL DEBUG)
          message(STATUS "Skipped valgrind tests. Build type not in debug mode.")
        endif (CMAKE_BUILD_TYPE STREQUAL DEBUG)
      else (GVR_DEBUG)
        message(STATUS "Skipped valgrind tests. Not in debug mode.")
      endif (GVR_DEBUG)
    else (GVR_RUN_TEST)
      message(STATUS "Skipped valgrind tests. Running tests is disabled.")
    endif (GVR_RUN_TEST)
  endif (GVR_RUN_VALGRIND_TEST)

  ############### GVR Test Externals Information ###############
  if (NOT DEFINED GVR_RUN_EXTERNAL_TEST)
    set(GVR_RUN_EXTERNAL_TEST OFF)
  else (NOT DEFINED GVR_RUN_EXTERNAL_TEST)
    if (NOT GVR_RUN_TEST)
      set(GVR_RUN_EXTERNAL_TEST OFF)
      message(STATUS "Skipped external tests. Running tests is disabled.")
    endif (NOT GVR_RUN_TEST)
  endif (NOT DEFINED GVR_RUN_EXTERNAL_TEST)

  ############### GVR Support Definitions ###############

 endif (NOT DEFINED GVR_FOUNDATION_INCLUDED)
