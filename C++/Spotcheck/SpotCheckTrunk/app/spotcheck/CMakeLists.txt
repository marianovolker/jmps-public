subdirs(
	Config
	Menus
	Reports
	Services
	TestHelper
	Version
)

# Main chooses the correct platform and sets MAIN_SRC.
add_subdirectory(Main)

# Build spotcheck executable
include_directories(
	${CMAKE_CURRENT_SOURCE_DIR}
	Main
)

add_executable(spotcheck
	${MAIN_SRC}
)

if("${CMAKE_TOOLCHAIN_FILE}" MATCHES "uclinux")
	set(CMAKE_EXE_LINKER_FLAGS "-Wl,-elf2flt='-s 32768 -z' -Wl,-move-rodata -lc -m5307 -static -lpthread")
endif()

target_link_libraries(spotcheck
	SPOT
	Common
	Config_Spotcheck
	Menus
	Reports
	Services
	Platform
	Version
	${EXTRA_LIBS}
)

install(TARGETS spotcheck 
	RUNTIME DESTINATION opt/spotcheck/bin
)
