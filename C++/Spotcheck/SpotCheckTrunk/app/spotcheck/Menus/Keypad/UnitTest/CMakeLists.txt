aux_source_directory(${CMAKE_CURRENT_SOURCE_DIR} TEST_SRC)

add_executable(GKeypadManagerUT
	${TEST_SRC}
)

target_link_libraries(GKeypadManagerUT
	TestHelper
	Services
	Reports
	Menus
	SPOT
	Common
	Platform
	Version
	gtest
	${EXTRA_LIBS}
)

add_clean_target(GKeypadManagerUT)

add_test(GKeypadManagerUT GKeypadManagerUT "")

