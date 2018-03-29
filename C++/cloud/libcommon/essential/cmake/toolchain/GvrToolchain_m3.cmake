##-#######################################################################-##
# Copyright © 2014, 2015 Gilbarco Inc.
# Confidential and Proprietary.
##
## @file GvrToolchain_m3.cmake
## @author pcantarini
## @date Tuesday, April 01, 2014
## @copyright Copyright © 2014 Gilbarco Inc. Confidential and Proprietary.
##-#######################################################################-##

# set the target system
set(CMAKE_SYSTEM_NAME Linux)

# optional
set(CMAKE_SYSTEM_VERSION 1)

# set the target system architecture
set(CMAKE_SYSTEM_PROCESSOR m68k)

# set the gilbarco target system architecture
set(GVR_TARGET_PLATFORM m3)

# specify the cross compiler
# cross-compiler is searched based on a full path in order to allow multiple
# cross-compilers with the same name coexists in the same build environment
set(CMAKE_C_COMPILER   /opt/m3/bin/m68k-uclinux-gcc)
set(CMAKE_CXX_COMPILER /opt/m3/bin/m68k-uclinux-g++)

set(ENV{PATH} "/opt/m3/bin:$ENV{PATH}")

# where is the target environment
set(CMAKE_FIND_ROOT_PATH /opt/m3 /opt/m3/m68k-uclinux)

# search for programs in the build host directories
set(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM NEVER)
# for libraries and headers in the target directories
set(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY)
set(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY)

# automatically set to true
#set(CMAKE_CROSSCOMPILING ON)

# force static linking for all targets
# this properties are not set as they seem to be not needed
#set_property(GLOBAL PROPERTY TARGET_SUPPORTS_SHARED_LIBS FALSE)
#set(CMAKE_FIND_LIBRARY_PREFIXES "lib")
#set(CMAKE_FIND_LIBRARY_SUFFIXES ".a")
#set(CMAKE_SHARED_LIBRARY_LINK_C_FLAGS "") 
#set(CMAKE_SHARED_LIBRARY_LINK_CXX_FLAGS "")

# Do not build shared libraries. Static libraries only.
set(BUILD_SHARED_LIBS OFF)

# Obtain the current .cmake file location so a relative path to the override rules can be created.
get_filename_component(CMAKE_CURRENT_LIST_DIR ${CMAKE_CURRENT_LIST_FILE} PATH)

# Set the Rules Override path in order to let CMake to use it.
set(CMAKE_USER_MAKE_RULES_OVERRIDE "${CMAKE_CURRENT_LIST_DIR}/override/GvrRules_m3.cmake")

