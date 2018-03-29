# Top level CMakeLists.txt file for Spotcheck

cmake_minimum_required(VERSION 2.8)

project(spotcheck)

# Project level variables
# Allow EXTLIB_PATH to be overriden by setting it as an environment variable
set(EXTLIB_PATH $ENV{EXTLIB_PATH})
if (NOT DEFINED EXTLIB_PATH)
	set(EXTLIB_PATH ${CMAKE_SOURCE_DIR}/extlib)
endif()
# Target platform variable for later use in subordinated makefiles
set(TARGET_PLATFORM)


# Set platform specific variables/settings
if (WIN32 OR "${CMAKE_TOOLCHAIN_FILE}" MATCHES "windows")
	message("==== Spotcheck configured for Windows ====")
	set(EXTLIB_PATH ${EXTLIB_PATH}/.build_windows)
	set(CMAKE_EXE_LINKER_FLAGS -static)
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DWINDOWS_PLATFORM -Wall -s -O3 -w -fno-rtti")
	set(EXTRA_INCS ${CMAKE_SOURCE_DIR}/lib/platform/Windows)
	set(EXTRA_LIBS ws2_32)
	set(TARGET_PLATFORM windows)
	
elseif("${CMAKE_TOOLCHAIN_FILE}" MATCHES "codesourcery")
	message("==== Spotcheck configured for SPOTM5 ARM ====")
	set(EXTLIB_PATH ${EXTLIB_PATH}/.build_spotm5)
	add_definitions(-D__linux__)
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DSPOT_M5 -pipe -Wall -w -mfpu=neon -fno-rtti")	
	set(CMAKE_EXE_LINKER_FLAGS -static)
	set(EXTRA_INCS ${CMAKE_SOURCE_DIR}/lib/platform/Linux)
	set(EXTRA_LIBS -Wl,--whole-archive pthread -Wl,--no-whole-archive)
	set(TARGET_PLATFORM spotm5)

elseif("${CMAKE_TOOLCHAIN_FILE}" MATCHES "jibe")
	message("==== Spotcheck configured for VGD JIBE ====")
	set(EXTLIB_PATH ${EXTLIB_PATH}/.build_vgd)
	add_definitions(-D__linux__)
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DSPOT_M5 -pipe -Wall -w -fno-rtti -march=armv5te -mcpu=arm926ej-s")	
	set(CMAKE_EXE_LINKER_FLAGS -static)
	set(EXTRA_INCS ${CMAKE_SOURCE_DIR}/lib/platform/Linux)
	set(EXTRA_LIBS -Wl,--whole-archive pthread -Wl,--no-whole-archive)
	set(TARGET_PLATFORM vgd)

elseif("${CMAKE_TOOLCHAIN_FILE}" MATCHES "uclinux")
	message("==== Spotcheck configured for SPOTM3 coldfire ====")
	set(EXTLIB_PATH ${EXTLIB_PATH}/.build_spotm3)
	add_definitions(-D__linux__)
	# Necessary flag re-definition to eliminate "generic flags" only needed in CMake compiler tests.
	set(CMAKE_C_FLAGS "-Wstrict-prototypes -Wall -Wno-trigraphs -fno-strict-aliasing -mno-strict-align -fno-builtin -w -msoft-float -Os -m5307 -fno-common -fno-rtti -fexceptions" )
	set(CMAKE_CXX_FLAGS "-DSPOT_M3 -Wall -Wno-trigraphs -fno-strict-aliasing -mno-strict-align -fno-builtin -w -msoft-float -Os -m5307 -fno-common -fno-rtti -fexceptions ")
	set(CMAKE_EXE_LINKER_FLAGS -m5307 -static)
	set(EXTRA_INCS ${CMAKE_SOURCE_DIR}/lib/platform/Linux)
	set(EXTRA_LIBS -Wl,--whole-archive pthread -Wl,--no-whole-archive)
	set(TARGET_PLATFORM spotm3)
	
else()
	message("==== Spotcheck configured for Native Linux ====")
	set(EXTLIB_PATH ${EXTLIB_PATH}/.build_linux)
	add_definitions(-D__linux__)
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DLINUX_PLATFORM -g -pipe")
	set(CMAKE_EXE_LINKER_FLAGS -static-libgcc)
	set(EXTRA_INCS ${CMAKE_SOURCE_DIR}/lib/platform/Linux)
	set(EXTRA_LIBS pthread)
	set(TARGET_PLATFORM linux)
	
endif()

# Project level include paths
include_directories(
	${EXTLIB_PATH}/include
	${EXTLIB_PATH}/include/pugixml
	${CMAKE_SOURCE_DIR}/lib/platform/Include
	${CMAKE_SOURCE_DIR}/lib	
	${EXTRA_INCS}
)

link_directories(
	${EXTLIB_PATH}/lib
)

# Build these subdirectories
subdirs(
	app
	lib
)

# Project level flags
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fexceptions")

# Pull in CTest support
enable_testing()

# Macro for allowing individual projects to create clean targets
macro(add_clean_target dir)
	add_custom_target(clean-${dir} 
		COMMAND ${CMAKE_MAKE_PROGRAM} clean WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR})
endmacro(add_clean_target)


