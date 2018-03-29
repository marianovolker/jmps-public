# CMAKE tool chain file for ucLinux coldfire cross-compiler
# this one is important
SET(CMAKE_SYSTEM_NAME Linux)
#this one not so much
SET(CMAKE_SYSTEM_VERSION 1)
SET(CMAKE_SYSTEM_PROCESSOR coldfire)

# specify the cross compiler for C and C++ 
include(CMakeForceCompiler)
CMAKE_FORCE_C_COMPILER(m68k-uclinux-gcc GNU)
CMAKE_FORCE_CXX_COMPILER(m68k-uclinux-g++ GNU)

# search for programs in the build host directories
SET(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM NEVER)
# for libraries and headers in the target directories
SET(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY)
SET(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY)