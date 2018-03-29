##-#######################################################################-##
# Copyright © 2015 Gilbarco Inc.
# Confidential and Proprietary.
##
## @file GvrToolchain_m7.cmake
## @author pcantarini
## @date Wednesday, February 18, 2015
## @copyright Copyright © 2015 Gilbarco Inc. Confidential and Proprietary.
##-#######################################################################-##

# set the target system
set(CMAKE_SYSTEM_NAME Linux)

# optional
set(CMAKE_SYSTEM_VERSION 1)

# set the target system architecture
set(CMAKE_SYSTEM_PROCESSOR arm)
set(GVR_TARGET_PLATFORM m7)

# specify the cross compiler
# cross-compiler is searched based on a full path in order to allow multiple
# cross-compilers with the same name coexists in the same build environment
set(CMAKE_C_COMPILER   /opt/m7/bin/arm-none-linux-gnueabi-gcc)
set(CMAKE_CXX_COMPILER /opt/m7/bin/arm-none-linux-gnueabi-g++)
set(ENV{PATH} "/opt/m7/bin:$ENV{PATH}")

# where is the target environment
set(CMAKE_FIND_ROOT_PATH /opt/m7 /opt/m7/arm-none-linux-gnueabi)

# search for programs in the build host directories
set(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM NEVER)
# for libraries and headers in the target directories
set(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY)
set(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY)

# automatically set to true
#set(CMAKE_CROSSCOMPILING ON)

# Obtain the current .cmake file location so a relative path to the override rules can be created.
get_filename_component(CMAKE_CURRENT_LIST_DIR ${CMAKE_CURRENT_LIST_FILE} PATH)

# Set the Rules Override path in order to let CMake to use it.
set(CMAKE_USER_MAKE_RULES_OVERRIDE "${CMAKE_CURRENT_LIST_DIR}/override/GvrRules_m7.cmake")

