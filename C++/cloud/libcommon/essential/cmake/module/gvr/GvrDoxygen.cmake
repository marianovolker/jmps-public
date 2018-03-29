##-#######################################################################-##
# Copyright © 2014 Gilbarco Inc.
# Confidential and Proprietary.
#
## @file GvrDoxygen.cmake
## @author pcantarini
## @date Tuesday, April 22, 2014
## @copyright Copyright © 2014 Gilbarco Inc. Confidential and Proprietary.
##-#######################################################################-##

cmake_minimum_required(VERSION 2.8.1 FATAL_ERROR)

if (NOT DEFINED GVR_DOXYGEN_INCLUDED)
  set(GVR_DOXYGEN_INCLUDED ON)

  ############### GVR Support Definitions ###############
  function (GVR_DOCUMENT_WITH_DOXYGEN)
    include(CMakeParseArguments)
    set(GVR_FLAGS SKIP_DOT OPTIONAL ALL REMOVE_FIRST)
    set(GVR_SINGLE_VALUE DESTINATION_LOCATION VERSION DOXYFILE CUSTOM_TARGET TEMPORARY_LOCATION)
    set(GVR_MULTI_VALUE PROPERTIES DEPENDS)
    cmake_parse_arguments(GVR_DOXYGEN "${GVR_FLAGS}" "${GVR_SINGLE_VALUE}" "${GVR_MULTI_VALUE}" ${ARGN})

    if (GVR_DOXYGEN_CUSTOM_TARGET)
      if (GVR_DOXYGEN_DOXYFILE)
        file(TO_CMAKE_PATH "${GVR_DOXYGEN_DOXYFILE}" GVR_DOXYGEN_DOXYFILE)
        if (NOT IS_ABSOLUTE ${GVR_DOXYGEN_DOXYFILE})
          set(GVR_DOXYGEN_DOXYFILE "${CMAKE_CURRENT_SOURCE_DIR}/${GVR_DOXYGEN_DOXYFILE}")
        endif (NOT IS_ABSOLUTE ${GVR_DOXYGEN_DOXYFILE})
        get_filename_component(GVR_DOXYGEN_DOXYFILE ${GVR_DOXYGEN_DOXYFILE} REALPATH)
        set(GVR_DOXYGEN_DOXYFILE_IS_IN OFF)
        string(REGEX MATCH ".*\\.in$"
                   GVR_DOXYGEN_DOXYFILE_IS_IN
                   ${GVR_DOXYGEN_DOXYFILE})
        if (EXISTS ${GVR_DOXYGEN_DOXYFILE} AND NOT IS_DIRECTORY ${GVR_DOXYGEN_DOXYFILE} AND GVR_DOXYGEN_DOXYFILE_IS_IN)
          if (GVR_DOXYGEN_DESTINATION_LOCATION)
            file(TO_CMAKE_PATH "${GVR_DOXYGEN_DESTINATION_LOCATION}" GVR_DOXYGEN_DESTINATION_LOCATION)
            list(LENGTH GVR_DOXYGEN_PROPERTIES GVR_DOXYGEN_PROPERTIES_LENGTH)
            while (GVR_DOXYGEN_PROPERTIES_LENGTH GREATER 1)
              list(GET GVR_DOXYGEN_PROPERTIES 0 GVR_DOXYGEN_PROPERTY_NAME)
              list(GET GVR_DOXYGEN_PROPERTIES 1 GVR_DOXYGEN_PROPERTY_VALUE)
              set(${GVR_DOXYGEN_PROPERTY_NAME} ${GVR_DOXYGEN_PROPERTY_VALUE})
              list(REMOVE_AT GVR_DOXYGEN_PROPERTIES 0 1)
              list(LENGTH GVR_DOXYGEN_PROPERTIES GVR_DOXYGEN_PROPERTIES_LENGTH)
            endwhile (GVR_DOXYGEN_PROPERTIES_LENGTH GREATER 1)
            if (GVR_DOXYGEN_PROPERTIES_LENGTH)
              message(FATAL_ERROR "GVR_DOCUMENT_WITH_DOXYGEN called with invalid property value format. PROPERTIES have to be set in KEY VALUE pairs.")
            endif (GVR_DOXYGEN_PROPERTIES_LENGTH)

            set(GVR_DOXYGEN_REQUIREMENT REQUIRED)
            if (GVR_DOXYGEN_OPTIONAL)
              set(GVR_DOXYGEN_REQUIREMENT OPTIONAL)
            endif (GVR_DOXYGEN_OPTIONAL)

            set(GVR_DOXYGEN_IN_ALL "")
            if (GVR_DOXYGEN_ALL)
              set(GVR_DOXYGEN_IN_ALL ALL)
            endif (GVR_DOXYGEN_ALL)

            set(DOXYGEN_SKIP_DOT ${GVR_DOXYGEN_SKIP_DOT})
            find_package(Doxygen ${GVR_DOXYGEN_VERSION} ${GVR_DOXYGEN_REQUIREMENT})
            if (DOXYGEN_FOUND)
              if (GVR_DOXYGEN_TEMPORARY_LOCATION)
                file(TO_CMAKE_PATH "${GVR_DOXYGEN_TEMPORARY_LOCATION}" GVR_DOXYGEN_TEMPORARY_LOCATION)
                if (NOT IS_ABSOLUTE ${GVR_DOXYGEN_TEMPORARY_LOCATION})
                  set(GVR_DOXYGEN_TEMPORARY_LOCATION "${CMAKE_CURRENT_BINARY_DIR}/${GVR_DOXYGEN_TEMPORARY_LOCATION}")
                endif (NOT IS_ABSOLUTE ${GVR_DOXYGEN_TEMPORARY_LOCATION})
                get_filename_component(GVR_DOXYGEN_TEMPORARY_LOCATION ${GVR_DOXYGEN_TEMPORARY_LOCATION} REALPATH)
              else (GVR_DOXYGEN_TEMPORARY_LOCATION)
                set(GVR_DOXYGEN_TEMPORARY_LOCATION "${CMAKE_CURRENT_BINARY_DIR}/GvrDoxygenTemporary")
              endif (GVR_DOXYGEN_TEMPORARY_LOCATION)
              file(REMOVE_RECURSE ${GVR_DOXYGEN_TEMPORARY_LOCATION})
              file(MAKE_DIRECTORY ${GVR_DOXYGEN_TEMPORARY_LOCATION})

              set(GVR_DOXYGEN_DOXYFILE_OUT_FILE "")
              string(REGEX REPLACE ".*/([^/]*)\\.in$"
                          "\\1"
                          GVR_DOXYGEN_DOXYFILE_OUT_FILE
                          ${GVR_DOXYGEN_DOXYFILE})
              set(GVR_DOXYGEN_DOXYFILE_OUT_FILE "${GVR_DOXYGEN_TEMPORARY_LOCATION}/${GVR_DOXYGEN_DOXYFILE_OUT_FILE}")
              configure_file(${GVR_DOXYGEN_DOXYFILE} ${GVR_DOXYGEN_DOXYFILE_OUT_FILE} @ONLY)

              if (NOT IS_ABSOLUTE ${GVR_DOXYGEN_DESTINATION_LOCATION})
                set(GVR_DOXYGEN_DESTINATION_LOCATION "${CMAKE_CURRENT_BINARY_DIR}/${GVR_DOXYGEN_DESTINATION_LOCATION}")
              endif (NOT IS_ABSOLUTE ${GVR_DOXYGEN_DESTINATION_LOCATION})
              get_filename_component(GVR_DOXYGEN_DESTINATION_LOCATION ${GVR_DOXYGEN_DESTINATION_LOCATION} REALPATH)

              set(GVR_DOXYGEN_DO_FIRST_COMMAND "")
              if (GVR_DOXYGEN_REMOVE_FIRST)
                set(GVR_DOXYGEN_DO_FIRST_COMMAND COMMAND ${CMAKE_COMMAND} -E remove_directory ${GVR_DOXYGEN_DESTINATION_LOCATION})
              endif (GVR_DOXYGEN_REMOVE_FIRST)
              set(GVR_DOXYGEN_DO_FIRST_COMMAND ${GVR_DOXYGEN_DO_FIRST_COMMAND} COMMAND ${CMAKE_COMMAND} -E make_directory ${GVR_DOXYGEN_DESTINATION_LOCATION})

              add_custom_target(${GVR_DOXYGEN_CUSTOM_TARGET} ${GVR_DOXYGEN_IN_ALL}
                  ${GVR_DOXYGEN_DO_FIRST_COMMAND}
                  COMMAND ${CMAKE_COMMAND} -E chdir ${GVR_DOXYGEN_DESTINATION_LOCATION} ${DOXYGEN_EXECUTABLE} ${GVR_DOXYGEN_DOXYFILE_OUT_FILE}
                  DEPENDS ${GVR_DOXYGEN_DEPENDS}
                  COMMENT "Generating documentation with Doxygen" VERBATIM)

              if (NOT TARGET doc)
                add_custom_target(doc)
              endif (NOT TARGET doc)
              add_dependencies(doc ${GVR_DOXYGEN_CUSTOM_TARGET})

            endif (DOXYGEN_FOUND)
          else (GVR_DOXYGEN_DESTINATION_LOCATION)
            if (NOT GVR_DOXYGEN_OPTIONAL)
              message(FATAL_ERROR "GVR_DOCUMENT_WITH_DOXYGEN called with no DESTINATION_LOCATION and no OPTIONAL flag was provided.")
            endif (NOT GVR_DOXYGEN_OPTIONAL)
          endif (GVR_DOXYGEN_DESTINATION_LOCATION)
        else (EXISTS ${GVR_DOXYGEN_DOXYFILE} AND NOT IS_DIRECTORY ${GVR_DOXYGEN_DOXYFILE} AND GVR_DOXYGEN_DOXYFILE_IS_IN)
          if (NOT GVR_DOXYGEN_OPTIONAL)
            message(FATAL_ERROR "GVR_DOCUMENT_WITH_DOXYGEN called with invalid SOURCE and no OPTIONAL flag was provided.")
          endif (NOT GVR_DOXYGEN_OPTIONAL)
        endif (EXISTS ${GVR_DOXYGEN_DOXYFILE} AND NOT IS_DIRECTORY ${GVR_DOXYGEN_DOXYFILE} AND GVR_DOXYGEN_DOXYFILE_IS_IN)
      else (GVR_DOXYGEN_DOXYFILE)
        if (NOT GVR_DOXYGEN_OPTIONAL)
          message(FATAL_ERROR "GVR_DOCUMENT_WITH_DOXYGEN called with no SOURCE and no OPTIONAL flag was provided.")
        endif (NOT GVR_DOXYGEN_OPTIONAL)
      endif (GVR_DOXYGEN_DOXYFILE)
    else (GVR_DOXYGEN_CUSTOM_TARGET)
      if (NOT GVR_DOXYGEN_OPTIONAL)
        message(FATAL_ERROR "GVR_DOCUMENT_WITH_DOXYGEN called with invalid CUSTOM_TARGET and no OPTIONAL flag was provided.")
      endif (NOT GVR_DOXYGEN_OPTIONAL)
    endif (GVR_DOXYGEN_CUSTOM_TARGET)
  endfunction (GVR_DOCUMENT_WITH_DOXYGEN)

endif (NOT DEFINED GVR_DOXYGEN_INCLUDED)
