# CMakeLists file for building package pugixml

cmake_minimum_required(VERSION 2.8)

include(ExternalProject)

if(DEFINED CMAKE_TOOLCHAIN_FILE)
	set(USE_TOOLCHAIN -DCMAKE_TOOLCHAIN_FILE=${CMAKE_TOOLCHAIN_FILE})

# Print the platform to be built to
	if("${CMAKE_TOOLCHAIN_FILE}" MATCHES "uclinux")
		message("==== pugixml library configured for coldfire ====")
	elseif("${CMAKE_TOOLCHAIN_FILE}" MATCHES "sourcery")
		message("==== pugixml library configured for ARM ====")
	elseif(WIN32 OR "${CMAKE_TOOLCHAIN_FILE}" MATCHES "windows")
		message("==== pugixml library configured for Windows ====")
	endif()

endif()

ExternalProject_Add(pugixml
	URL ${CMAKE_CURRENT_SOURCE_DIR}/pugixml-1.0.tar.gz
	PATCH_COMMAND find ${CMAKE_CURRENT_SOURCE_DIR}/patches -name "*.patch" | xargs -n1 patch -p1 -i
	CMAKE_ARGS -DVGLIB_BINARY_DIR=${CMAKE_BINARY_DIR} ${USE_TOOLCHAIN}
)

