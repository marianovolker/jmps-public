# CMakeLists file for building package gtest

cmake_minimum_required(VERSION 2.8)

include(ExternalProject)

if(WIN32)
	# gtest fails to compile under MinGW with pthread support turned on
	set(DISABLE_WIN32_PTHREAD -Dgtest_disable_pthreads=ON)
	set(FORCE_SHARED_CRT -Dgtest_force_shared_crt=ON)
endif()

if(DEFINED CMAKE_TOOLCHAIN_FILE)
	set(USE_TOOLCHAIN -DCMAKE_TOOLCHAIN_FILE=${CMAKE_TOOLCHAIN_FILE})
endif()

ExternalProject_Add(gtest
	URL ${CMAKE_CURRENT_SOURCE_DIR}/gtest-1.6.0.tar.gz
	PATCH_COMMAND find ${CMAKE_CURRENT_SOURCE_DIR}/patches -name "*.patch" | xargs -n1 patch -p1 -i
	CMAKE_ARGS -DVGLIB_BINARY_DIR=${CMAKE_BINARY_DIR} ${DISABLE_WIN32_PTHREAD} ${USE_TOOLCHAIN} ${FORCE_SHARED_CRT}
)
