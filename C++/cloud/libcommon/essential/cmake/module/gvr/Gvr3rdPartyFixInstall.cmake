##-#######################################################################-##
# Copyright © 2014 Gilbarco Inc.
# Confidential and Proprietary.
#
## @file Gvr3rdPartyFixInstall.cmake
## @author pcantarini
## @date Monday, April 28, 2014
## @copyright Copyright © 2014 Gilbarco Inc. Confidential and Proprietary.
##-#######################################################################-##

cmake_minimum_required(VERSION 2.8.1 FATAL_ERROR)

if (NOT DEFINED GVR_3RDPARTY_FIX_INSTALL_INCLUDED)
  set(GVR_3RDPARTY_FIX_INSTALL_INCLUDED ON)

  ############### GVR Support Definitions ###############
  function(GVR_3RDPARTY_FIX_INSTALL)
    if (ARGN)
      set(GVR_3RDPARTY_FIX_INSTALL_STRIP_COMMAND "")
      if (CMAKE_C_COMPILER MATCHES "^.*gcc$")
        if (NOT CMAKE_STRIP)
          message(WARNING "Invalid CMAKE_STRIP command")
        endif (NOT CMAKE_STRIP)
      else (CMAKE_C_COMPILER MATCHES "^.*gcc$")
        message(WARNING "Invalid CMAKE_C_COMPILER command")
      endif (CMAKE_C_COMPILER MATCHES "^.*gcc$")

      find_program(GVR_3RDPARTY_FIX_INSTALL_FILE_COMMAND NAMES file)
      if (GVR_3RDPARTY_FIX_INSTALL_FILE_COMMAND)
        find_program(GVR_3RDPARTY_FIX_INSTALL_CHMOD_COMMAND NAMES chmod)
        if (GVR_3RDPARTY_FIX_INSTALL_CHMOD_COMMAND)
## ---------- INSTALL CODE - BEGIN ----------
          install(CODE "##GVR_INSTALL - BEGIN - Code for stripping 3rdParty elf binaries and fixing file permissions
  set(GVR_3RDPARTY_FIX_INSTALL_RESOURCES ${ARGN})
  set(GVR_3RDPARTY_FIX_INSTALL_STRIP_COMMAND ${CMAKE_STRIP})
  set(GVR_3RDPARTY_FIX_INSTALL_FILE_COMMAND ${GVR_3RDPARTY_FIX_INSTALL_FILE_COMMAND})
  set(GVR_3RDPARTY_FIX_INSTALL_CHMOD_COMMAND ${GVR_3RDPARTY_FIX_INSTALL_CHMOD_COMMAND})
  file(TO_CMAKE_PATH \"\${CMAKE_INSTALL_PREFIX}\" CMAKE_INSTALL_PREFIX)
  get_filename_component(CMAKE_INSTALL_PREFIX \"\${CMAKE_INSTALL_PREFIX}\" REALPATH)
  if (GVR_3RDPARTY_FIX_INSTALL_FILE_COMMAND)
    if (GVR_3RDPARTY_FIX_INSTALL_CHMOD_COMMAND)
      foreach (GVR_3RDPARTY_FIX_INSTALL_RESOURCE \${GVR_3RDPARTY_FIX_INSTALL_RESOURCES})
        file(TO_CMAKE_PATH \"\${GVR_3RDPARTY_FIX_INSTALL_RESOURCE}\" GVR_3RDPARTY_FIX_INSTALL_RESOURCE)
        if (NOT IS_ABSOLUTE \${GVR_3RDPARTY_FIX_INSTALL_RESOURCE})
          set(GVR_3RDPARTY_FIX_INSTALL_RESOURCE \"\${CMAKE_INSTALL_PREFIX}/\${GVR_3RDPARTY_FIX_INSTALL_RESOURCE}\")
        endif (NOT IS_ABSOLUTE \${GVR_3RDPARTY_FIX_INSTALL_RESOURCE})
        if (IS_DIRECTORY \${GVR_3RDPARTY_FIX_INSTALL_RESOURCE})
          file(GLOB_RECURSE GVR_3RDPARTY_FIX_INSTALL_RESOURCE_FILES \"\${GVR_3RDPARTY_FIX_INSTALL_RESOURCE}/*\")
        else (IS_DIRECTORY \${GVR_3RDPARTY_FIX_INSTALL_RESOURCE})
          if (EXISTS \${GVR_3RDPARTY_FIX_INSTALL_RESOURCE})
            set(GVR_3RDPARTY_FIX_INSTALL_RESOURCE_FILES \${GVR_3RDPARTY_FIX_INSTALL_RESOURCE})
          endif (EXISTS \${GVR_3RDPARTY_FIX_INSTALL_RESOURCE})
        endif (IS_DIRECTORY \${GVR_3RDPARTY_FIX_INSTALL_RESOURCE})

        foreach(GVR_3RDPARTY_FIX_INSTALL_RESOURCE_FILE \${GVR_3RDPARTY_FIX_INSTALL_RESOURCE_FILES})
          if (NOT IS_DIRECTORY \${GVR_3RDPARTY_FIX_INSTALL_RESOURCE_FILE})
            execute_process(COMMAND \${GVR_3RDPARTY_FIX_INSTALL_FILE_COMMAND} -b \"\${GVR_3RDPARTY_FIX_INSTALL_RESOURCE_FILE}\"
                            RESULT_VARIABLE GVR_3RDPARTY_FIX_INSTALL_FILE_COMMAND_RESULT
                            OUTPUT_VARIABLE GVR_3RDPARTY_FIX_INSTALL_FILE_COMMAND_OUTPUT
                            ERROR_VARIABLE GVR_3RDPARTY_FIX_INSTALL_FILE_COMMAND_ERROR)
            if (GVR_3RDPARTY_FIX_INSTALL_FILE_COMMAND_RESULT EQUAL 0)
              if (CMAKE_INSTALL_DO_STRIP)
                if (GVR_3RDPARTY_FIX_INSTALL_FILE_COMMAND_OUTPUT MATCHES \"ELF [^,]*(executable|shared object),.* not stripped\")
                  if (GVR_3RDPARTY_FIX_INSTALL_STRIP_COMMAND)
                    execute_process(COMMAND \${GVR_3RDPARTY_FIX_INSTALL_STRIP_COMMAND} -s \"\${GVR_3RDPARTY_FIX_INSTALL_RESOURCE_FILE}\"
                                    OUTPUT_QUIET
                                    ERROR_QUIET)
                  else (GVR_3RDPARTY_FIX_INSTALL_STRIP_COMMAND)
                    message(WARNING \"Unable to strip resource '\${GVR_3RDPARTY_FIX_INSTALL_RESOURCE_FILE}'. Invalid strip command.\")
                  endif (GVR_3RDPARTY_FIX_INSTALL_STRIP_COMMAND)
                endif (GVR_3RDPARTY_FIX_INSTALL_FILE_COMMAND_OUTPUT MATCHES \"ELF [^,]*(executable|shared object),.* not stripped\")
              endif (CMAKE_INSTALL_DO_STRIP)

              if (NOT GVR_3RDPARTY_FIX_INSTALL_FILE_COMMAND_OUTPUT MATCHES \"executable\")
                execute_process(COMMAND \${GVR_3RDPARTY_FIX_INSTALL_CHMOD_COMMAND} a-x \"\${GVR_3RDPARTY_FIX_INSTALL_RESOURCE_FILE}\"
                                OUTPUT_QUIET
                                ERROR_QUIET)
              endif (NOT GVR_3RDPARTY_FIX_INSTALL_FILE_COMMAND_OUTPUT MATCHES \"executable\")
            endif (GVR_3RDPARTY_FIX_INSTALL_FILE_COMMAND_RESULT EQUAL 0)
          endif (NOT IS_DIRECTORY \${GVR_3RDPARTY_FIX_INSTALL_RESOURCE_FILE})
        endforeach(GVR_3RDPARTY_FIX_INSTALL_RESOURCE_FILE)
      endforeach (GVR_3RDPARTY_FIX_INSTALL_RESOURCE)
    else (GVR_3RDPARTY_FIX_INSTALL_CHMOD_COMMAND)
      message(FATAL_ERROR \"Invalid chmod command\")
    endif (GVR_3RDPARTY_FIX_INSTALL_CHMOD_COMMAND)
  else (GVR_3RDPARTY_FIX_INSTALL_FILE_COMMAND)
    message(FATAL_ERROR \"Invalid file command\")
  endif (GVR_3RDPARTY_FIX_INSTALL_FILE_COMMAND)
##GVR_INSTALL - END - Code for stripping 3rdParty elf binaries and fixing file permissions"
                  COMPONENT ThirdParty)
## ---------- INSTALL CODE - END ----------
        else (GVR_3RDPARTY_FIX_INSTALL_CHMOD_COMMAND)
          message(FATAL_ERROR "Couldn't find chmod command")
        endif (GVR_3RDPARTY_FIX_INSTALL_CHMOD_COMMAND)
      else (GVR_3RDPARTY_FIX_INSTALL_FILE_COMMAND)
        message(FATAL_ERROR "Couldn't find file command")
      endif (GVR_3RDPARTY_FIX_INSTALL_FILE_COMMAND)
    endif (ARGN)
  endfunction(GVR_3RDPARTY_FIX_INSTALL)
endif (NOT DEFINED GVR_3RDPARTY_FIX_INSTALL_INCLUDED)
