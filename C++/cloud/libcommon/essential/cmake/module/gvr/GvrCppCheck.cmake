##-#######################################################################-##
# Copyright © 2014 Gilbarco Inc.
# Confidential and Proprietary.
#
## @file GvrCppCheck.cmake
## @author pcantarini
## @date Friday, May 16, 2014
## @copyright Copyright © 2014 Gilbarco Inc. Confidential and Proprietary.
##-#######################################################################-##

cmake_minimum_required(VERSION 2.8.1 FATAL_ERROR)

if (NOT DEFINED GVR_CPPCHECK_INCLUDED)
  set(GVR_CPPCHECK_INCLUDED ON)

  ############### GVR Support Definitions ###############
  function (GVR_CPPCHECK)
    include(CMakeParseArguments)
    set(GVR_FLAGS OPTIONAL ALL)
    set(GVR_SINGLE_VALUE CUSTOM_TARGET)
    set(GVR_MULTI_VALUE INCLUDES SOURCES DEPENDS)
    cmake_parse_arguments(GVR_CPPCHECK "${GVR_FLAGS}" "${GVR_SINGLE_VALUE}" "${GVR_MULTI_VALUE}" ${ARGN})
    
    if (GVR_CPPCHECK_SOURCES)
      if (GVR_CPPCHECK_CUSTOM_TARGET)
          set(GVR_CPPCHECK_IN_ALL "")
          if (GVR_CPPCHECK_ALL)
            set(GVR_CPPCHECK_IN_ALL ALL)
          endif (GVR_CPPCHECK_ALL)

          if (NOT DEFINED GVR_CPPCHECK_COMMAND)
            find_program(GVR_CPPCHECK_COMMAND NAMES cppcheck)
          endif (NOT DEFINED GVR_CPPCHECK_COMMAND)
          if (GVR_CPPCHECK_COMMAND)
            set(GVR_CPPCHECK_INCLUDES_LIST "")
            foreach(GVR_CPPCHECK_INCLUDE ${GVR_CPPCHECK_INCLUDES})
              set(GVR_CPPCHECK_INCLUDES_LIST ${GVR_CPPCHECK_INCLUDES_LIST} -I ${GVR_CPPCHECK_INCLUDE})
            endforeach(GVR_CPPCHECK_INCLUDE ${GVR_CPPCHECK_INCLUDES})

            add_custom_target(${GVR_CPPCHECK_CUSTOM_TARGET} ${GVR_CPPCHECK_IN_ALL}
              ${GVR_CPPCHECK_COMMAND} --verbose --force --enable=all --error-exitcode=66 ${GVR_CPPCHECK_INCLUDES_LIST} ${GVR_CPPCHECK_SOURCES}
                DEPENDS ${GVR_CPPCHECK_DEPENDS}
                COMMENT "Performing static source code check with cppcheck" VERBATIM)

            if (NOT TARGET static-check)
              add_custom_target(static-check)
            endif (NOT TARGET static-check)
            add_dependencies(static-check ${GVR_CPPCHECK_CUSTOM_TARGET})
          else (GVR_CPPCHECK_COMMAND)
            if (GVR_CPPCHECK_OPTIONAL)
              message(STATUS "Skipped cppcheck tests. Command cppcheck not found.")
            else (GVR_CPPCHECK_OPTIONAL)
              message(FATAL_ERROR "GVR_CPPCHECK called without OPTIONAL flag set and command cppcheck not found.")
            endif (GVR_CPPCHECK_OPTIONAL)
          endif (GVR_CPPCHECK_COMMAND)
      else (GVR_CPPCHECK_CUSTOM_TARGET)
        if (NOT GVR_CPPCHECK_OPTIONAL)
          message(FATAL_ERROR "GVR_CPPCHECK called with invalid CUSTOM_TARGET and no OPTIONAL flag was provided.")
        endif (NOT GVR_CPPCHECK_OPTIONAL)
      endif (GVR_CPPCHECK_CUSTOM_TARGET)
    endif (GVR_CPPCHECK_SOURCES)
  endfunction (GVR_CPPCHECK)

endif (NOT DEFINED GVR_CPPCHECK_INCLUDED)
