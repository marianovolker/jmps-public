##-#######################################################################-##
# Copyright © 2015 Gilbarco Inc.
# Confidential and Proprietary.
##
## @file GvrRules_m3.cmake
## @author pcantarini
## @date Monday, February 09, 2015
## @copyright Copyright © 2015 Gilbarco Inc. Confidential and Proprietary.
##-#######################################################################-##

# necessary flags for compilation and linking on this platform
# the 'CMAKE_*_FLAGS_INIT' version of these variables are used instead of the 'CMAKE_*_FLAGS' versions as these ones are overwritten and the _INIT ones are appended
set(CMAKE_C_FLAGS_INIT "${CMAKE_C_FLAGS_INIT}                                   -march=armv7-a -mcpu=cortex-a8 -fPIC -O2")
set(CMAKE_C_FLAGS_DEBUG_INIT "${CMAKE_C_FLAGS_DEBUG_INIT}                       -march=armv7-a -mcpu=cortex-a8 -fPIC -ggdb3")
set(CMAKE_C_FLAGS_MINSIZEREL_INIT "${CMAKE_C_FLAGS_MINSIZEREL_INIT}             -march=armv7-a -mcpu=cortex-a8 -fPIC -OS -DNDEBUG")
set(CMAKE_C_FLAGS_RELEASE_INIT "${CMAKE_C_FLAGS_RELEASE_INIT}                   -march=armv7-a -mcpu=cortex-a8 -fPIC -O2 -DNDEBUG")
set(CMAKE_C_FLAGS_RELWITHDEBINFO_INIT "${CMAKE_C_FLAGS_RELWITHDEBINFO_INIT}     -march=armv7-a -mcpu=cortex-a8 -fPIC -ggdb3 -DNDEBUG")
set(CMAKE_CXX_FLAGS_INIT "${CMAKE_CXX_FLAGS_INIT}                               -march=armv7-a -mcpu=cortex-a8 -fPIC -O2")
set(CMAKE_CXX_FLAGS_DEBUG_INIT "${CMAKE_CXX_FLAGS_DEBUG_INIT}                   -march=armv7-a -mcpu=cortex-a8 -fPIC -ggdb3")
set(CMAKE_CXX_FLAGS_MINSIZEREL_INIT "${CMAKE_CXX_FLAGS_MINSIZEREL_INIT}         -march=armv7-a -mcpu=cortex-a8 -fPIC -OS -DNDEBUG")
set(CMAKE_CXX_FLAGS_RELEASE_INIT "${CMAKE_CXX_FLAGS_RELEASE_INIT}               -march=armv7-a -mcpu=cortex-a8 -fPIC -O2 -DNDEBUG")
set(CMAKE_CXX_FLAGS_RELWITHDEBINFO_INIT "${CMAKE_CXX_FLAGS_RELWITHDEBINFO_INIT} -march=armv7-a -mcpu=cortex-a8 -fPIC -ggdb3 -DNDEBUG")
#set(CMAKE_EXE_LINKER_FLAGS_INIT "${CMAKE_EXE_LINKER_FLAGS_INIT} ")
