##-#######################################################################-##
# Copyright © 2014 Gilbarco Inc.
# Confidential and Proprietary.
##
## @file GvrToolchain_win32.cmake
## @author pcantarini
## @date Tuesday, April 01, 2014
## @copyright Copyright © 2014 Gilbarco Inc. Confidential and Proprietary.
##-#######################################################################-##

# set the target system
set(CMAKE_SYSTEM_NAME Windows)

# optional
set(CMAKE_SYSTEM_VERSION 1)

# set the target system architecture
set(CMAKE_SYSTEM_PROCESSOR x86)
set(GVR_TARGET_PLATFORM win32)

# specify the cross compiler
# cross-compiler is searched based on a full path in order to allow multiple
# cross-compilers with the same name coexists in the same build environment
set(CMAKE_C_COMPILER   /opt/win32/bin/i486-mingw32-gcc)
set(CMAKE_CXX_COMPILER /opt/win32/bin/i486-mingw32-g++)
set(CMAKE_RC_COMPILER  /opt/win32/bin/i486-mingw32-windres)
set(ENV{PATH} "/opt/win32/bin:$ENV{PATH}")

# where is the target environment
set(CMAKE_FIND_ROOT_PATH /opt/win32 /opt/win32/i486-mingw32)

# search for programs in the build host directories
set(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM NEVER)
# for libraries and headers in the target directories
set(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY)
set(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY)

# automatically set to true
#set(CMAKE_CROSSCOMPILING ON)
