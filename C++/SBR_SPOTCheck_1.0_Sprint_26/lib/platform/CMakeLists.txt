# CMakeLists.txt for Vanguard platform library

if (WIN32)
	aux_source_directory(Windows PLATFORM_SRC)
	include_directories(Windows)
else()
	aux_source_directory(Linux PLATFORM_SRC)
	include_directories(Linux)
endif()

add_library(Platform
	${PLATFORM_SRC}
)

# Unit tests
if (DEFINED UNIT_TEST)
	subdirs(UnitTest)
else(DEFINED UNIT_TEST)
        message("Skipping unit tests in ./lib/platform/UnitTest")
endif(DEFINED UNIT_TEST)

