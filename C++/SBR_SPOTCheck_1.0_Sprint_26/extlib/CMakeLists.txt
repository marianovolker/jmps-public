# Top level CMakeLists.txt file for Vanguard Libraries

cmake_minimum_required(VERSION 2.8)

project(vglib)

# Set platform specific variables/settings
if (WIN32 OR "${CMAKE_TOOLCHAIN_FILE}" MATCHES "windows")
    message("==== VGLIB configured for Windows ====")
elseif("${CMAKE_TOOLCHAIN_FILE}" MATCHES "sourcery")
    message("==== VGLIB configured for Vanguard ARM ====")
    add_definitions(-D__linux__)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pipe -Wall -w -mfpu=neon -fno-rtti")
elseif("${CMAKE_TOOLCHAIN_FILE}" MATCHES "uclinux")
    message("==== VGLIB configured for coldfire ====")
    add_definitions(-D__linux__)
else()
    message("==== VGLIB configured for Native Linux ====")
    add_definitions(-D__linux__)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pipe")
endif()

if(NOT EXCLUDE_GTEST_LIBRARY)
    set(OPTIONAL_GTEST_LIB opensource/gtest)
endif()

subdirs(
    opensource/pugixml
    ${OPTIONAL_GTEST_LIB}
)

